/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#ifndef STLINK2_H_
#define STLINK2_H_

#include <stdint.h>

#define STLINK2_ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))

enum stlink2_status {
	STLINK2_STATUS_UNKNOWN      = -1,
	STLINK2_STATUS_CORE_RUNNING = 0x80,
	STLINK2_STATUS_CORE_HALTED  = 0x81
};

enum stlink2_mode {
	STLINK2_MODE_UNKNOWN = -1,
	STLINK2_MODE_DFU     = 0x00,
	STLINK2_MODE_MASS    = 0x01,
	STLINK2_MODE_DEBUG   = 0x02
};

enum stlink2_swdclk {
	STLINK2_SWDCLK_4000KHZ = 0U,
	STLINK2_SWDCLK_1800KHZ = 1U,
	STLINK2_SWDCLK_1200KHZ = 2U,
	STLINK2_SWDCLK_950KHZ  = 3U,
	STLINK2_SWDCLK_480KHZ  = 7U,
	STLINK2_SWDCLK_240KHZ  = 15U,
	STLINK2_SWDCLK_125KHZ  = 31U,
	STLINK2_SWDCLK_100KHZ  = 40U,
	STLINK2_SWDCLK_50KHZ   = 79U,
	STLINK2_SWDCLK_25KHZ   = 158U,
	STLINK2_SWDCLK_15KHZ   = 265U,
	STLINK2_SWDCLK_5KHZ    = 798U
};

typedef struct stlink2 *stlink2_t;

void stlink2_init(void);
void stlink2_exit(void);

stlink2_t stlink2_open(const char *serial);
void stlink2_close(stlink2_t *dev);

const char *stlink2_get_name(stlink2_t dev);
const char *stlink2_get_serial(stlink2_t dev);
const char *stlink2_get_version(stlink2_t dev);
float stlink2_get_target_voltage(stlink2_t dev);
enum stlink2_mode stlink2_get_mode(struct stlink2 *dev);
void stlink2_set_mode_swd(struct stlink2 *dev);
enum stlink2_status stlink2_get_status(struct stlink2 *dev);
void stlink2_mcu_halt(stlink2_t dev);
uint32_t stlink2_get_coreid(stlink2_t dev);
uint32_t stlink2_get_chipid(stlink2_t dev);
uint32_t stlink2_get_cpuid(stlink2_t dev);
uint16_t stlink2_get_devid(stlink2_t dev);
uint32_t stlink2_get_flash_size(stlink2_t dev);

void stlink2_read_reg(stlink2_t dev, uint8_t idx, uint32_t *val);

#include <stlink2/log.h>
#include <stlink2/usb.h>
#include <stlink2/stm32.h>
#include <stlink2/semihosting.h>

/** @todo internal... */
struct stlink2 {
	char *serial;
	const char *name;
	struct {
		uint8_t stlink;
		uint8_t jtag;
		uint8_t swim;
		char *version;
	} fw;
	struct {
		enum stlink2_loglevel level;
		FILE *fp;
	} log;
	struct {
		uint16_t pid;
		libusb_device_handle *dev;
		uint8_t rx_ep;
		uint8_t tx_ep;
	} usb;
};

void stlink2_read_debug32(struct stlink2 *dev, uint32_t addr, uint32_t *val);

#endif /* STLINK2_H_ */
