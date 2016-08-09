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

enum stlink2_cmd {
	STLINK2_CMD_GET_VERSION      = 0xf1,
	STLINK2_CMD_DEBUG            = 0xf2,
	STLINK2_CMD_DFU              = 0xf3,
	STLINK2_CMD_DFU_EXIT         = 0x07,
	STLINK2_CMD_DFU_GET_VERSION  = 0x08,
	STLINK2_CMD_GET_CURRENT_MODE = 0xf5,
	STLINK2_CMD_RESET            = 0xf7
};

enum stlink2_cmd_debug {
	STLINK2_CMD_DEBUG_GET_STATUS     = 0x01,
	STLINK2_CMD_DEBUG_FORCE          = 0x02,
	STLINK2_CMD_DEBUG_READ_MEM32     = 0x07,
	STLINK2_CMD_DEBUG_WRITE_MEM32    = 0x08,
	STLINK2_CMD_DEBUG_WRITE_MEM8     = 0x0d,
	STLINK2_CMD_DEBUG_ENTER_MODE     = 0x20,
	STLINK2_CMD_DEBUG_ENTER_SWD      = 0xa3,
	STLINK2_CMD_DEBUG_ENTER_JTAG     = 0x00,
	STLINK2_CMD_DEBUG_EXIT           = 0x21,
	STLINK2_CMD_DEBUG_READ_COREID    = 0x22,
	STLINK2_CMD_DEBUG_RESETSYS       = 0x03,
	STLINK2_CMD_DEBUG_READALLREGS    = 0x04,
	STLINK2_CMD_DEBUG_READ_REG       = 0x33,
	STLINK2_CMD_DEBUG_WRITE_REG      = 0x34,
	STLINK2_CMD_DEBUG_RUN_CORE       = 0x09,
	STLINK2_CMD_DEBUG_STEP_CORE      = 0x0a,
	STLINK2_CMD_DEBUG_WRITE_REGPC    = 0x34,
	STLINK2_CMD_DEBUG_HARD_RESET     = 0x3c,
	STLINK2_CMD_DEBUG_READCOREREGS   = 0x3a,
	STLINK2_CMD_DEBUG_SETFP          = 0x0b,
	STLINK2_CMD_JTAG_WRITEDEBUG_32BIT = 0x35,
	STLINK2_CMD_JTAG_READDEBUG_32BIT = 0x36,
	STLINK2_CMD_SWD_SET_FREQ         = 0x43
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

stlink2_t stlink2_open(const char *serial);
void stlink2_close(stlink2_t *dev);

void stlink2_probe(void);
void stlink2_free(stlink2_t dev);
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

uint32_t stlink2_get_chipid(struct stlink2 *dev);
void stlink2_read_debug32(struct stlink2 *dev, uint32_t addr, uint32_t *val);

#endif /* STLINK2_H_ */
