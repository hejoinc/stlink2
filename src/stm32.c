/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#include <stlink2.h>

const char *stlink2_stm32_devid_str(uint32_t chipid)
{
	const uint16_t devid = chipid & 0xfff;

	static const struct devid_str {
		uint16_t devid;
		const char *str;
	} _devid_str[] = {
		{STLINK2_STM32_DEVID_STM32F2XX,          "STM32F2xx"},
		{STLINK2_STM32_DEVID_STM32L1XX_CAT3_MED, "STM32L1xx (Cat.3 - Medium+ Density)"},
		{STLINK2_STM32_DEVID_STM32L0XX_CAT1,     "STM32L0xx (Cat.1)"}
	};

	for (size_t n = 0; n < STLINK2_ARRAY_SIZE(_devid_str); n++) {
		if (devid == _devid_str[n].devid)
			return _devid_str[n].str;
	}

	return NULL;
}

void stlink2_stm32_info(struct stlink2 *dev, char *buf, int buf_size)
{
	uint32_t dbgmcu_idcode;

	/* read stm32 device id register */
	dbgmcu_idcode = stlink2_get_chipid(dev);

	uint16_t device_id = dbgmcu_idcode & 0xfff;
	uint16_t rev_id = dbgmcu_idcode >> 16;
	const char *device_str = stlink2_stm32_devid_str(dbgmcu_idcode);
	const char *rev_str = NULL;

	switch (device_id) {
	case STLINK2_STM32_DEVID_STM32L1XX_CAT3_MED:
		device_str = "STM32L1xx (Cat.3 - Medium+ Density)";

		break;
	case STLINK2_STM32_DEVID_STM32F2XX:
		switch (rev_id) {
		case STLINK2_STM32_REVID_STM32F2XX_REV_A:
			rev_str = "A";
			break;
		case STLINK2_STM32_REVID_STM32F2XX_REV_B:
			rev_str = "B";
			break;
		case STLINK2_STM32_REVID_STM32F2XX_REV_Z:
			rev_str = "Z";
			break;
		case STLINK2_STM32_REVID_STM32F2XX_REV_Y:
			rev_str = "Y";
			break;
		case STLINK2_STM32_REVID_STM32F2XX_REV_X:
			rev_str = "X";
			break;
		}
		break;
	case 0x413:
	case 0x419:
		device_str = "STM32F4xx";

		switch (rev_id) {
		case STLINK2_STM32_REVID_STM32F4XX_REV_A:
			rev_str = "A";
			break;
		case STLINK2_STM32_REVID_STM32F4XX_REV_Z:
			rev_str = "Z";
			break;
		case STLINK2_STM32_REVID_STM32F4XX_REV_Y:
			rev_str = "Y";
			break;
		case STLINK2_STM32_REVID_STM32F4XX_REV_1:
			rev_str = "1";
			break;
		case STLINK2_STM32_REVID_STM32F4XX_REV_3:
			rev_str = "3";
			break;
		}
		break;
	case 0x421:
		device_str = "STM32F446";

		switch (rev_id) {
		case 0x1000:
			rev_str = "A";
			break;
		}
		break;
	case 0x423:
	case 0x431:
	case 0x433:
	case 0x458:
	case 0x441:
		device_str = "STM32F4xx (Low Power)";

		switch (rev_id) {
		case 0x1000:
			rev_str = "A";
			break;

		case 0x1001:
			rev_str = "Z";
			break;
		}
		break;

	case 0x449:
		device_str = "STM32F7[4|5]x";

		switch (rev_id) {
		case 0x1000:
			rev_str = "A";
			break;

		case 0x1001:
			rev_str = "Z";
			break;
		}
		break;
	case 0x434:
		device_str = "STM32F46x/F47x";

		switch (rev_id) {
		case 0x1000:
			rev_str = "A";
			break;
		}
		break;
	default:
		break;
	}

	if (!device_str) {
		snprintf(buf, buf_size, "Cannot identify target as a STM32F2/4/7");
		return;
	}

	if (rev_str != NULL)
		snprintf(buf, buf_size, "%s - Rev: %s", device_str, rev_str);
	else
		snprintf(buf, buf_size, "%s - Rev: unknown (0x%04x)", device_str, rev_id);
}
