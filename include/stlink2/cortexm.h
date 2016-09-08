/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#ifndef STLINK2_CORTEXM_H_
#define STLINK2_CORTEXM_H_

#include <stdint.h>

/* Private peripheral bus base address */
#define STLINK2_CORTEXM_PPB_BASE	0xE0000000

#define STLINK2_CORTEXM_SCS_BASE	(STLINK2_CORTEXM_PPB_BASE + 0xE000)

#define STLINK2_CORTEXM_AIRCR		(STLINK2_CORTEXM_SCS_BASE + 0xD0C)
#define STLINK2_CORTEXM_CFSR		(STLINK2_CORTEXM_SCS_BASE + 0xD28)
#define STLINK2_CORTEXM_HFSR		(STLINK2_CORTEXM_SCS_BASE + 0xD2C)
#define STLINK2_CORTEXM_DFSR		(STLINK2_CORTEXM_SCS_BASE + 0xD30)
#define STLINK2_CORTEXM_CPACR		(STLINK2_CORTEXM_SCS_BASE + 0xD88)
#define STLINK2_CORTEXM_DHCSR		(STLINK2_CORTEXM_SCS_BASE + 0xDF0)
#define STLINK2_CORTEXM_DCRSR		(STLINK2_CORTEXM_SCS_BASE + 0xDF4)
#define STLINK2_CORTEXM_DCRDR		(STLINK2_CORTEXM_SCS_BASE + 0xDF8)
#define STLINK2_CORTEXM_DEMCR		(STLINK2_CORTEXM_SCS_BASE + 0xDFC)

/* Debug Halting Control and Status Register (DHCSR) */
/* This key must be written to bits 31:16 for write to take effect */
#define STLINK2_CORTEXM_DHCSR_DBGKEY		0xA05F0000
/* Bits 31:26 - Reserved */
#define STLINK2_CORTEXM_DHCSR_S_RESET_ST	(1 << 25)
#define STLINK2_CORTEXM_DHCSR_S_RETIRE_ST	(1 << 24)
/* Bits 23:20 - Reserved */
#define STLINK2_CORTEXM_DHCSR_S_LOCKUP		(1 << 19)
#define STLINK2_CORTEXM_DHCSR_S_SLEEP		(1 << 18)
#define STLINK2_CORTEXM_DHCSR_S_HALT		(1 << 17)
#define STLINK2_CORTEXM_DHCSR_S_REGRDY		(1 << 16)
/* Bits 15:6 - Reserved */
#define STLINK2_CORTEXM_DHCSR_C_SNAPSTALL	(1 << 5)	/* v7m only */
/* Bit 4 - Reserved */
#define STLINK2_CORTEXM_DHCSR_C_MASKINTS	(1 << 3)
#define STLINK2_CORTEXM_DHCSR_C_STEP		(1 << 2)
#define STLINK2_CORTEXM_DHCSR_C_HALT		(1 << 1)
#define STLINK2_CORTEXM_DHCSR_C_DEBUGEN		(1 << 0)

/* Debug Core Register Selector Register (DCRSR) */
#define STLINK2_CORTEXM_DCRSR_REGWnR		0x00010000
#define STLINK2_CORTEXM_DCRSR_REGSEL_MASK	0x0000001F
#define STLINK2_CORTEXM_DCRSR_REGSEL_XPSR	0x00000010
#define STLINK2_CORTEXM_DCRSR_REGSEL_MSP	0x00000011
#define STLINK2_CORTEXM_DCRSR_REGSEL_PSP	0x00000012

/* Debug Exception and Monitor Control Register (DEMCR) */
/* Bits 31:25 - Reserved */
#define STLINK2_CORTEXM_DEMCR_TRCENA		(1 << 24)
/* Bits 23:20 - Reserved */
#define STLINK2_CORTEXM_DEMCR_MON_REQ		(1 << 19)	/* v7m only */
#define STLINK2_CORTEXM_DEMCR_MON_STEP		(1 << 18)	/* v7m only */
#define STLINK2_CORTEXM_DEMCR_VC_MON_PEND	(1 << 17)	/* v7m only */
#define STLINK2_CORTEXM_DEMCR_VC_MON_EN		(1 << 16)	/* v7m only */
/* Bits 15:11 - Reserved */
#define STLINK2_CORTEXM_DEMCR_VC_HARDERR	(1 << 10)
#define STLINK2_CORTEXM_DEMCR_VC_INTERR		(1 << 9)	/* v7m only */
#define STLINK2_CORTEXM_DEMCR_VC_BUSERR		(1 << 8)	/* v7m only */
#define STLINK2_CORTEXM_DEMCR_VC_STATERR	(1 << 7)	/* v7m only */
#define STLINK2_CORTEXM_DEMCR_VC_CHKERR		(1 << 6)	/* v7m only */
#define STLINK2_CORTEXM_DEMCR_VC_NOCPERR	(1 << 5)	/* v7m only */
#define STLINK2_CORTEXM_DEMCR_VC_MMERR		(1 << 4)	/* v7m only */
/* Bits 3:1 - Reserved */
#define STLINK2_CORTEXM_DEMCR_VC_CORERESET	(1 << 0)

#define STLINK2_CORTEXM_CPUID_REG 0xE000ED00

enum stlink2_cortexm_cpuid_partno {
	STLINK2_CORTEXM_CPUID_PARTNO_M0      = 0xc20,
	STLINK2_CORTEXM_CPUID_PARTNO_M0_PLUS = 0xc60,
	STLINK2_CORTEXM_CPUID_PARTNO_M1      = 0xc21,
	STLINK2_CORTEXM_CPUID_PARTNO_M3      = 0xc23,
	STLINK2_CORTEXM_CPUID_PARTNO_M4      = 0xc24,
	STLINK2_CORTEXM_CPUID_PARTNO_M7      = 0xc27
};

enum stlink2_cortexm_cpuid_arch {
	STLINK2_CORTEXM_CPUID_ARCH_V6M = 0xc,
	STLINK2_CORTEXM_CPUID_ARCH_V7M = 0xf
};

enum stlink2_cortexm_cpuid_partno stlink2_cortexm_cpuid_get_partno(uint32_t cpuid);
const char *stlink2_cortexm_cpuid_partno_str(uint32_t cpuid);

enum stlink2_cortexm_cpuid_arch stlink2_cortexm_cpuid_get_arch(uint32_t cpuid);

#endif /* STLINK2_CORTEXM_H_ */
