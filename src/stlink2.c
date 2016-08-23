/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#include <stlink2.h>
#include <stlink2/cmd.h>
#include <stlink2/cortexm.h>
#include <stlink2/utils/bconv.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum stlink2_mode stlink2_get_mode(struct stlink2 *dev)
{
	uint8_t cmd[STLINK2_USB_CMD_SIZE];
	uint8_t rep[2];

	memset(cmd, 0, sizeof(cmd));
	cmd[0] = STLINK2_CMD_GET_CURRENT_MODE;

	stlink2_usb_send_recv(dev, cmd, STLINK2_USB_CMD_SIZE, rep, 2);

	switch (rep[0]) {
	case STLINK2_MODE_DFU:
		break;
	case STLINK2_MODE_MASS:
		break;
	case STLINK2_MODE_DEBUG:
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

void stlink2_get_version(struct stlink2 *dev)
{
	uint8_t rep[6];

	stlink2_command(dev, STLINK2_CMD_GET_VERSION, 0x80, rep, sizeof(rep));

	dev->fw.stlink = (rep[0] & 0xf0) >> 4;
	dev->fw.jtag   = ((rep[0] & 0x0f) << 2) | ((rep[1] & 0xc0) >> 6);
	dev->fw.swim   = rep[1] & 0x3f;

	printf("   firmware: V%uJ%uM%u\n", dev->fw.stlink, dev->fw.jtag, dev->fw.swim);
}

void stlink2_reset(stlink2_t dev)
{
	stlink2_command(dev, STLINK2_CMD_RESET, 0, NULL, 0);
}

enum stlink2_status stlink2_get_status(struct stlink2 *dev)
{
	uint8_t rep[2];

	stlink2_debug_command(dev, STLINK2_CMD_DEBUG_GET_STATUS, 0, rep, sizeof(rep));

	switch (rep[0]) {
	case STLINK2_STATUS_CORE_RUNNING:
		printf("     status: core running\n");
		break;
	case STLINK2_STATUS_CORE_HALTED:
		printf("     status: core halted\n");
		break;
	default:
		printf("     status: unknown\n");
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
}

void stlink2_mcu_halt(struct stlink2 *dev)
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
	return calloc(sizeof(struct stlink2), 1);
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
	if (_dev->usb.dev) {
		libusb_close(_dev->usb.dev);
		_dev->usb.dev = NULL;
	}

	free(_dev);
	*dev = NULL;
}

static libusb_context *ctx = NULL;

void stlink2_init(void)
{
	int ret;

	ret = libusb_init(&ctx);
	if (ret < 0)
		ctx = NULL;

	if (ctx)
		libusb_set_debug(ctx, LIBUSB_LOG_LEVEL_DEBUG);
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

	if (ctx == NULL)
		return NULL;

	cnt = libusb_get_device_list(ctx, &devs);
	if (cnt < 0)
		return NULL;

	dev = stlink2_dev_alloc();

	/* Loop trough all libusb devices and probe stlink2 */
	for (ssize_t n = 0; n < cnt; n++) {
		if (!stlink2_usb_probe_dev(devs[n], dev))
			continue;

		/* When no specific serial is search we are done here */
		if (!serial) {
			found = true;
			break;
		}

		/* Check if current stlink has matched serial */
		if (strncmp(serial, dev->serial, strlen(dev->serial)) == 0) {
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
	uint8_t rep[4];

	stlink2_debug_command(dev, STLINK2_CMD_DEBUG_READ_COREID, 0, rep, sizeof(rep));
	return stlink2_bconv_u32_le_to_h(rep);
}

uint32_t stlink2_get_chipid(stlink2_t dev)
{
	return stlink2_cmd_get_chipid(dev);
}
