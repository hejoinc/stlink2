/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#ifndef STLINK2_H_
#define STLINK2_H_

#include <stdint.h>

#define STLINK2_ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))

enum stlink2_loglevel {
	STLINK2_LOGLEVEL_QUIET,
	STLINK2_LOGLEVEL_ERROR,
	STLINK2_LOGLEVEL_WARN,
	STLINK2_LOGLEVEL_INFO,
	STLINK2_LOGLEVEL_DEBUG,
	STLINK2_LOGLEVEL_TRACE
};

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

/* Private peripheral bus base address */
#define CORTEXM_PPB_BASE	0xE0000000

#define CORTEXM_SCS_BASE	(CORTEXM_PPB_BASE + 0xE000)

#define CORTEXM_AIRCR		(CORTEXM_SCS_BASE + 0xD0C)
#define CORTEXM_CFSR		(CORTEXM_SCS_BASE + 0xD28)
#define CORTEXM_HFSR		(CORTEXM_SCS_BASE + 0xD2C)
#define CORTEXM_DFSR		(CORTEXM_SCS_BASE + 0xD30)
#define CORTEXM_CPACR		(CORTEXM_SCS_BASE + 0xD88)
#define CORTEXM_DHCSR		(CORTEXM_SCS_BASE + 0xDF0)
#define CORTEXM_DCRSR		(CORTEXM_SCS_BASE + 0xDF4)
#define CORTEXM_DCRDR		(CORTEXM_SCS_BASE + 0xDF8)
#define CORTEXM_DEMCR		(CORTEXM_SCS_BASE + 0xDFC)

/* Debug Halting Control and Status Register (DHCSR) */
/* This key must be written to bits 31:16 for write to take effect */
#define CORTEXM_DHCSR_DBGKEY		0xA05F0000
/* Bits 31:26 - Reserved */
#define CORTEXM_DHCSR_S_RESET_ST	(1 << 25)
#define CORTEXM_DHCSR_S_RETIRE_ST	(1 << 24)
/* Bits 23:20 - Reserved */
#define CORTEXM_DHCSR_S_LOCKUP		(1 << 19)
#define CORTEXM_DHCSR_S_SLEEP		(1 << 18)
#define CORTEXM_DHCSR_S_HALT		(1 << 17)
#define CORTEXM_DHCSR_S_REGRDY		(1 << 16)
/* Bits 15:6 - Reserved */
#define CORTEXM_DHCSR_C_SNAPSTALL	(1 << 5)	/* v7m only */
/* Bit 4 - Reserved */
#define CORTEXM_DHCSR_C_MASKINTS	(1 << 3)
#define CORTEXM_DHCSR_C_STEP		(1 << 2)
#define CORTEXM_DHCSR_C_HALT		(1 << 1)
#define CORTEXM_DHCSR_C_DEBUGEN		(1 << 0)

/* Debug Core Register Selector Register (DCRSR) */
#define CORTEXM_DCRSR_REGWnR		0x00010000
#define CORTEXM_DCRSR_REGSEL_MASK	0x0000001F
#define CORTEXM_DCRSR_REGSEL_XPSR	0x00000010
#define CORTEXM_DCRSR_REGSEL_MSP	0x00000011
#define CORTEXM_DCRSR_REGSEL_PSP	0x00000012

/* Debug Exception and Monitor Control Register (DEMCR) */
/* Bits 31:25 - Reserved */
#define CORTEXM_DEMCR_TRCENA		(1 << 24)
/* Bits 23:20 - Reserved */
#define CORTEXM_DEMCR_MON_REQ		(1 << 19)	/* v7m only */
#define CORTEXM_DEMCR_MON_STEP		(1 << 18)	/* v7m only */
#define CORTEXM_DEMCR_VC_MON_PEND	(1 << 17)	/* v7m only */
#define CORTEXM_DEMCR_VC_MON_EN		(1 << 16)	/* v7m only */
/* Bits 15:11 - Reserved */
#define CORTEXM_DEMCR_VC_HARDERR	(1 << 10)
#define CORTEXM_DEMCR_VC_INTERR		(1 << 9)	/* v7m only */
#define CORTEXM_DEMCR_VC_BUSERR		(1 << 8)	/* v7m only */
#define CORTEXM_DEMCR_VC_STATERR	(1 << 7)	/* v7m only */
#define CORTEXM_DEMCR_VC_CHKERR		(1 << 6)	/* v7m only */
#define CORTEXM_DEMCR_VC_NOCPERR	(1 << 5)	/* v7m only */
#define CORTEXM_DEMCR_VC_MMERR		(1 << 4)	/* v7m only */
/* Bits 3:1 - Reserved */
#define CORTEXM_DEMCR_VC_CORERESET	(1 << 0)

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

/** INTERNAL */
#include <libusb.h>
#include <stdbool.h>
#include <stdio.h>

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

void stlink2_msleep(int milliseconds);

/* stlink2.c */
uint32_t stlink2_get_chipid(struct stlink2 *dev);
void stlink2_read_debug32(struct stlink2 *dev, uint32_t addr, uint32_t *val);

/* stm32.c */
void stlink2_stm32_info(struct stlink2 *dev, char *buf, int buf_size);

#endif /* STLINK2_H_ */
