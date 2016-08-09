/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#include <stlink2/utils/bconv.h>

uint32_t stlink2_bconv_u32_le_to_h(const uint8_t *buf)
{
	return (uint32_t)(buf[0] | buf[1] << 8 | buf[2] << 16 | buf[3] << 24);
}

void stlink2_bconv_u32_h_to_le(uint8_t *buf, uint32_t val)
{
	buf[3] = (uint8_t)(val >> 24);
	buf[2] = (uint8_t)(val >> 16);
	buf[1] = (uint8_t)(val >> 8);
	buf[0] = (uint8_t)(val >> 0);
}
