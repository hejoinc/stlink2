/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#ifndef STLINK2_UTILS_BCONV_H
#define STLINK2_UTILS_BCONV_H

#include <stdint.h>

uint32_t stlink2_bconv_u32_le_to_h(const uint8_t *buf);
void stlink2_bconv_u32_h_to_le(uint8_t *buf, uint32_t val);

#endif /* STLINK2_UTILS_BCONV_H */
