/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#ifndef STLINK2_CMD_H_
#define STLINK2_CMD_H_

#include <stlink2.h>

enum stlink2_cmd {
	STLINK2_CMD_GET_VERSION        = 0xf1,
	STLINK2_CMD_DEBUG              = 0xf2,
	STLINK2_CMD_DFU                = 0xf3,
	STLINK2_CMD_DFU_EXIT           = 0x07,
	STLINK2_CMD_DFU_GET_VERSION    = 0x08,
	STLINK2_CMD_GET_CURRENT_MODE   = 0xf5,
	STLINK2_CMD_GET_TARGET_VOLTAGE = 0xf7,
	STLINK2_CMD_RESET              = 0xf7 /** @todo verify...remove */
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

uint32_t stlink2_cmd_get_chipid(struct stlink2 *dev);

#endif /* STLINK2_CMD_H_ */
