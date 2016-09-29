/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#include <stlink2.h>

const char *stlink2_stm32_devid_str(uint32_t devid)
{
	switch (devid) {
#define STLINK2_STM32_DEV(name, string, devid, flash_size_reg) \
	case STLINK2_STM32_DEVID_##name: \
		return string;
STLINK2_STM32_DEV_LIST
#undef  STLINK2_STM32_DEV
	default:
		break;
	}

	return "unknown";
}

uint32_t stlink2_stm32_flash_size_reg(uint32_t devid)
{
	switch (devid) {
#define STLINK2_STM32_DEV(name, string, devid, flash_size_reg) \
	case STLINK2_STM32_DEVID_##name: \
		return flash_size_reg;
STLINK2_STM32_DEV_LIST
#undef  STLINK2_STM32_DEV
	default:
		break;
	}

	return 0x00000000;
}
