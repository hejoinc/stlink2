/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#include <stlink2-internal.h>
#include <stlink2/log.h>
#include <stlink2/cmd.h>
#include <stlink2/cortexm.h>
#include <stlink2/utils/bconv.h>
#include <stlink2/utils/hexstr.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum stlink2_mode stlink2_get_mode(struct stlink2 *dev)
{
	uint8_t cmd[STLINK2_USB_CMD_SIZE];
	uint8_t rep[2];

	memset(cmd, 0, sizeof(cmd));
	cmd[0] = STLINK2_CMD_GET_CURRENT_MODE;

	stlink2_usb_send_recv(dev, cmd, STLINK2_USB_CMD_SIZE, rep, sizeof(rep));

	switch (rep[0]) {
	case STLINK2_MODE_DFU:
		STLINK2_LOG(DEBUG, dev, "stlink2_get_mode: STLINK2_MODE_DFU\n");
		break;
	case STLINK2_MODE_MASS:
		STLINK2_LOG(DEBUG, dev, "stlink2_get_mode: STLINK2_MODE_MASS\n");
		break;
	case STLINK2_MODE_DEBUG:
		STLINK2_LOG(DEBUG, dev, "stlink2_get_mode: STLINK2_MODE_DEBUG\n");
		break;
	default:
		return STLINK2_MODE_UNKNOWN;
	}

	return rep[0];
}

static void stlink2_command(struct stlink2 *dev, uint8_t cmd, uint8_t param, uint8_t *buf, size_t bufsize)
{
	uint8_t _cmd[STLINK2_USB_CMD_SIZE];

	memset(_cmd, 0, sizeof(_cmd));
	_cmd[0] = cmd;
	_cmd[1] = param;

	stlink2_usb_send_recv(dev, _cmd, STLINK2_USB_CMD_SIZE, buf, bufsize);
}

static void stlink2_debug_command(struct stlink2 *dev, uint8_t cmd, uint8_t param, uint8_t *buf, size_t bufsize)
{
	uint8_t _cmd[STLINK2_USB_CMD_SIZE];

	memset(_cmd, 0, sizeof(_cmd));
	_cmd[0] = STLINK2_CMD_DEBUG;
	_cmd[1] = cmd;
	_cmd[2] = param;

	stlink2_usb_send_recv(dev, _cmd, STLINK2_USB_CMD_SIZE, buf, bufsize);
}

static void stlink2_debug_command_u32(struct stlink2 *dev, uint8_t cmd, uint32_t param, uint8_t *buf, size_t bufsize)
{
	uint8_t _cmd[1 + sizeof(cmd) + sizeof(param)];

	memset(_cmd, 0, sizeof(_cmd));
	_cmd[0] = STLINK2_CMD_DEBUG;
	_cmd[1] = cmd;
	stlink2_bconv_u32_h_to_le(&_cmd[2], param);

	stlink2_usb_send_recv(dev, _cmd, sizeof(_cmd), buf, bufsize);
}

void stlink2_read_debug32(struct stlink2 *dev, uint32_t addr, uint32_t *val)
{
	(void)val;
	uint8_t _rep[8];

	memset(_rep, 0, sizeof(_rep));

	stlink2_debug_command_u32(dev, STLINK2_CMD_JTAG_READDEBUG_32BIT, addr, _rep, sizeof(_rep));

	*val = stlink2_bconv_u32_le_to_h(&_rep[4]);
}

static void stlink2_write_debug32(struct stlink2 *dev, uint32_t addr, uint32_t val)
{
	uint8_t _rep[8];
	uint8_t _cmd[16];

	_cmd[0] = STLINK2_CMD_DEBUG;
	_cmd[1] = STLINK2_CMD_JTAG_WRITEDEBUG_32BIT;
	stlink2_bconv_u32_h_to_le(&_cmd[2], addr);
	stlink2_bconv_u32_h_to_le(&_cmd[3], val);

	stlink2_usb_send_recv(dev, _cmd, sizeof(_cmd), _rep, sizeof(_rep));
}

const char *stlink2_get_version(stlink2_t dev)
{
	uint8_t rep[6];

	stlink2_command(dev, STLINK2_CMD_GET_VERSION, 0x80, rep, sizeof(rep));

	dev->fw.stlink = (rep[0] & 0xf0) >> 4;
	dev->fw.jtag   = ((rep[0] & 0x0f) << 2) | ((rep[1] & 0xc0) >> 6);
	dev->fw.swim   = rep[1] & 0x3f;

	/** @todo there should be a better way to malloc */
	if (!dev->fw.version)
		dev->fw.version = malloc(32);
	if (dev->fw.version)
		snprintf(dev->fw.version, 32, "V%uJ%uS%u", dev->fw.stlink, dev->fw.jtag, dev->fw.swim);

	return dev->fw.version;
}

enum stlink2_status stlink2_get_status(struct stlink2 *dev)
{
	uint8_t rep[2];

	stlink2_debug_command(dev, STLINK2_CMD_DEBUG_GET_STATUS, 0, rep, sizeof(rep));

	switch (rep[0]) {
	case STLINK2_STATUS_CORE_RUNNING:
		STLINK2_LOG(INFO, dev, "     status: core running\n");
		break;
	case STLINK2_STATUS_CORE_HALTED:
		STLINK2_LOG(INFO, dev, "     status: core halted\n");
		break;
	default:
		STLINK2_LOG(INFO, dev, "     status: unknown\n");
		return STLINK2_STATUS_UNKNOWN;
	}

	return rep[0];
}

static void stlink2_set_exitmode_dfu(struct stlink2 *dev)
{
	stlink2_command(dev, STLINK2_CMD_DFU, STLINK2_CMD_DFU_EXIT, NULL, 0);
}

void stlink2_set_mode_swd(struct stlink2 *dev)
{
	stlink2_set_exitmode_dfu(dev);
	stlink2_debug_command(dev, STLINK2_CMD_DEBUG_ENTER_MODE, STLINK2_CMD_DEBUG_ENTER_SWD, NULL, 0);
	stlink2_get_mode(dev);
}

void stlink2_mcu_halt(stlink2_t dev)
{
	uint8_t rep[2];

	stlink2_debug_command(dev, STLINK2_CMD_DEBUG_STEP_CORE, 0, rep, sizeof(rep));
}

void stlink2_force_debug(stlink2_t dev)
{
	uint8_t rep[2];

	stlink2_debug_command(dev, STLINK2_CMD_DEBUG_FORCE, 0, rep, sizeof(rep));
}

void stlink2_mcu_reset(stlink2_t dev)
{
	uint8_t rep[2];

	stlink2_debug_command(dev, STLINK2_CMD_DEBUG_RESETSYS, 0, rep, sizeof(rep));
}

void stlink2_mcu_hardreset(stlink2_t dev)
{
	stlink2_debug_command(dev, STLINK2_CMD_DEBUG_HARD_RESET, 0, NULL, 0);
}

void stlink2_mcu_run(stlink2_t dev)
{
	uint8_t rep[2];

	stlink2_debug_command(dev, STLINK2_CMD_DEBUG_RUN_CORE, 0, rep, sizeof(rep));
}

void stlink2_read_all_regs(stlink2_t dev)
{
	uint8_t rep[84];

	stlink2_debug_command(dev, STLINK2_CMD_DEBUG_READALLREGS, 0, rep, sizeof(rep));

	for (size_t n = 0; n < 21; n++) {
		if (n < 16)
			printf("r%zu = 0x%08x\n", n, stlink2_bconv_u32_le_to_h(&rep[n * 4]));
		else if (n == 16)
			printf("xPSR = 0x%08x\n", stlink2_bconv_u32_le_to_h(&rep[n * 4]));
		else if (n == 17)
			printf("MSP = 0x%08x\n", stlink2_bconv_u32_le_to_h(&rep[n * 4]));
		else if (n == 18)
			printf("PSP = 0x%08x\n", stlink2_bconv_u32_le_to_h(&rep[n * 4]));
		else if (n == 19)
			printf("RW = 0x%08x\n", stlink2_bconv_u32_le_to_h(&rep[n * 4]));
		else if (n == 20)
			printf("RW1 = 0x%08x\n", stlink2_bconv_u32_le_to_h(&rep[n * 4]));
	}
}

void stlink2_read_reg(stlink2_t dev, uint8_t idx, uint32_t *val)
{
	uint8_t rep[8];

	stlink2_debug_command(dev, STLINK2_CMD_DEBUG_READ_REG, idx, rep, sizeof(rep));
	*val = stlink2_bconv_u32_le_to_h(&rep[4]);
}

void stlink2_write_reg(stlink2_t dev, uint8_t idx, uint32_t val)
{
	uint8_t _cmd[STLINK2_USB_CMD_SIZE];
	uint8_t rep[8];

	memset(_cmd, 0, sizeof(_cmd));
	_cmd[0] = STLINK2_CMD_DEBUG;
	_cmd[1] = STLINK2_CMD_DEBUG_WRITE_REG;
	_cmd[2] = idx;
	stlink2_bconv_u32_h_to_le(&_cmd[3], val);

	stlink2_usb_send_recv(dev, _cmd, STLINK2_USB_CMD_SIZE, rep, sizeof(rep));
}

void stlink2_set_swdclk(stlink2_t dev, enum stlink2_swdclk clk)
{
	(void)dev;
	(void)clk;
}

void stlink2_halt_resume(stlink2_t dev)
{
	uint32_t dhcsr = STLINK2_CORTEXM_DHCSR_DBGKEY | STLINK2_CORTEXM_DHCSR_C_DEBUGEN;

	stlink2_write_debug32(dev, STLINK2_CORTEXM_DHCSR, dhcsr);
	stlink2_read_debug32(dev, STLINK2_CORTEXM_DHCSR, &dhcsr);
}

static struct stlink2 *stlink2_dev_alloc(void)
{
	return calloc(1, sizeof(struct stlink2));
}

static void stlink2_dev_free(struct stlink2 **dev)
{
	if (!dev)
		return;
	if (!*dev)
		return;

	struct stlink2 *_dev = *dev;

	free(_dev->serial);
	_dev->serial = NULL;

	free(_dev->fw.version);
	_dev->fw.version = NULL;

	if (_dev->usb.dev) {
		libusb_close(_dev->usb.dev);
		_dev->usb.dev = NULL;
	}

	free(_dev);
	*dev = NULL;
}

/** @todo Move into stlink2_context */
static libusb_context *ctx = NULL;

void stlink2_init(void)
{
	int ret;

	ret = libusb_init(&ctx);
	if (ret < 0)
		ctx = NULL;
}

void stlink2_exit(void)
{
	if (ctx) {
		libusb_exit(ctx);
		ctx = NULL;
	}
}

struct stlink2 *stlink2_open(const char *serial)
{
	bool found = false;
	ssize_t cnt;
	struct stlink2 *dev;

	libusb_device **devs;

	if (!ctx)
		return NULL;

	cnt = libusb_get_device_list(ctx, &devs);
	if (cnt < 0)
		return NULL;

	dev = stlink2_dev_alloc();

	/* Loop trough all libusb devices and probe stlink2 */
	for (ssize_t n = 0; n < cnt; n++) {
		if (!stlink2_usb_probe_dev(devs[n], dev))
			continue;

		/* When no specific serial is searched then we are done */
		if (!serial) {
			found = true;
			break;
		}

		/* Check if current stlink has matched serial */
		if (strcmp(serial, dev->serial) == 0) {
			found = true;
			break;
		}

		stlink2_dev_free(&dev);
		dev = stlink2_dev_alloc();
	}

	libusb_free_device_list(devs, 1);

	if (!found)
		stlink2_dev_free(&dev);

	return dev;
}

void stlink2_close(stlink2_t *dev)
{
	stlink2_dev_free(dev);
}

const char *stlink2_get_serial(stlink2_t dev)
{
	return dev->serial;
}

const char *stlink2_get_name(stlink2_t dev)
{
	return dev->name;
}

uint32_t stlink2_get_coreid(stlink2_t dev)
{
	if (dev->mcu.coreid)
		return dev->mcu.coreid;

	uint8_t rep[4];

	stlink2_debug_command(dev, STLINK2_CMD_DEBUG_READ_COREID, 0, rep, sizeof(rep));
	dev->mcu.coreid = stlink2_bconv_u32_le_to_h(rep);

	return dev->mcu.coreid;
}

uint32_t stlink2_get_chipid(stlink2_t dev)
{
	if (dev->mcu.chipid)
		return dev->mcu.chipid;
	/** @todo move reg into macro */
	stlink2_read_debug32(dev, 0xE0042000, &dev->mcu.chipid);
	return dev->mcu.chipid;
}

uint32_t stlink2_get_cpuid(stlink2_t dev)
{
	if (dev->mcu.cpuid)
		return dev->mcu.cpuid;

	stlink2_read_debug32(dev, STLINK2_CORTEXM_CPUID_REG, &dev->mcu.cpuid);
	return dev->mcu.cpuid;
}

uint16_t stlink2_get_devid(stlink2_t dev)
{
	return stlink2_get_chipid(dev) & 0xfff;
}

uint32_t stlink2_get_flash_size(stlink2_t dev)
{
	if (dev->mcu.flash_size)
		return dev->mcu.flash_size;

	uint32_t reg = 0x1ff800cc; /** @todo hardcoded for devid 0x427 for now */

	stlink2_read_debug32(dev, reg, &dev->mcu.flash_size);
	return dev->mcu.flash_size;
}

const char *stlink2_get_unique_id(stlink2_t dev)
{
	if (dev->mcu.unique_id)
		return dev->mcu.unique_id;

	dev->mcu.unique_id = malloc(32);
	if (!dev->mcu.unique_id)
		return NULL;

	uint32_t addr = 0x1ff800d0; /**< @todo hardcoded reg */
	uint32_t unique_id[3]; /* 96-bit */

	for (size_t n = 0; n < 3; n++) {
		stlink2_read_debug32(dev, addr, &unique_id[n]);
		printf("[%08x] %08x\n", addr, unique_id[n]);
		unique_id[n] = htonl(unique_id[n]);
		addr += 4;
	}

	stlink2_hexstr_from_bin(dev->mcu.unique_id, 24, (void *)unique_id, 12);
	dev->mcu.unique_id[24] = 0;
	return dev->mcu.unique_id;
}

float stlink2_get_target_voltage(stlink2_t dev)
{
	uint8_t cmd[STLINK2_USB_CMD_SIZE];
	uint8_t rep[8];
	uint32_t adc_results[2];
	float voltage = 0;

	memset(cmd, 0, sizeof(cmd));
	cmd[0] = STLINK2_CMD_GET_TARGET_VOLTAGE;

	stlink2_usb_send_recv(dev, cmd, 1, rep, sizeof(rep));

	adc_results[0] = stlink2_bconv_u32_le_to_h(rep);
	adc_results[1] = stlink2_bconv_u32_le_to_h(rep + 4);

	if (adc_results[0])
		voltage = 2 * ((float)adc_results[1]) * (float)(1.2 / adc_results[0]);

	return voltage;
}

void stlink2_flush(stlink2_t dev)
{
	dev->mcu.coreid = 0;
	dev->mcu.cpuid  = 0;
	free(dev->mcu.unique_id);
}
