/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <stlink2/utils/hexstr.h>

/**
 * Binary to hex char map
 * 0 -> '0' .. 15 -> 'f'
 */
static char stlink2_hexstr_byte_map[16] = {
	'0', '1', '2', '3',
	'4', '5', '6', '7',
	'8', '9', 'a', 'b',
	'c', 'd', 'e', 'f'
};

/**
 * Convert ascii hex nibble character to binary
 *  e,g 'a' -> 0xa
 */
static uint8_t stlink2_hexstr_char_to_bin(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);

	return 0;
}

void stlink2_hexstr_to_bin(
	void *dst, size_t dst_len,
	const void *src, size_t src_len)
{
	size_t n;
	size_t nibble = 0;
	uint8_t *_dst = (uint8_t *)dst;
	const uint8_t *_dst_end = (uint8_t *)dst + dst_len;

	for (n = 0; n < src_len; n++) {
		if (isxdigit((int)((char *)src)[n])) {
			if (nibble == 2) {
				nibble = 0;
				_dst++;
				if (_dst == _dst_end)
					break;
			}

			if (nibble == 0)
				*_dst = stlink2_hexstr_char_to_bin(((char *)src)[n]) << 4;
			else
				*_dst |= stlink2_hexstr_char_to_bin(((char *)src)[n]);

			nibble++;
		}
	}
}

void stlink2_hexstr_from_bin(
	void *dst, size_t dst_len,
	const void *src, size_t src_len)
{
	size_t n;
	char *_dst = (char *)dst;
	const char *_dst_end = (char *)dst + dst_len;

	for (n = 0; n < src_len; n++) {
		*_dst = stlink2_hexstr_byte_map[(((const uint8_t *)src)[n] >> 4) & 0x0f];
		_dst++;
		if (_dst == _dst_end)
			break;

		*_dst = stlink2_hexstr_byte_map[((const uint8_t *)src)[n] & 0x0f];
		_dst++;
		if (_dst == _dst_end)
			break;
	}
}
