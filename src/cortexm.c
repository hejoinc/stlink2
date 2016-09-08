/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#include <stlink2/cortexm.h>

enum stlink2_cortexm_cpuid_partno stlink2_cortexm_cpuid_get_partno(uint32_t cpuid)
{
	return (cpuid >> 4) & 0xfff;
}

const char *stlink2_cortexm_cpuid_partno_str(uint32_t cpuid)
{
	enum stlink2_cortexm_cpuid_partno partno;

	partno = stlink2_cortexm_cpuid_get_partno(cpuid);
	switch (partno) {
	case STLINK2_CORTEXM_CPUID_PARTNO_M0:
		return "Cortex-M0";
	case STLINK2_CORTEXM_CPUID_PARTNO_M0_PLUS:
		return "Cortex-M0+";
	case STLINK2_CORTEXM_CPUID_PARTNO_M1:
		return "Cortex-M1";
	case STLINK2_CORTEXM_CPUID_PARTNO_M3:
		return "Cortex-M3";
	case STLINK2_CORTEXM_CPUID_PARTNO_M4:
		return "Cortex-M4";
	case STLINK2_CORTEXM_CPUID_PARTNO_M7:
		return "Cortex-M7";
	default:
		break;
	}

	return "unknown";
}
