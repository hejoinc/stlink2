/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#ifndef STLINK2_UTILS_HEXSTR_H_
#define STLINK2_UTILS_HEXSTR_H_

#include <stddef.h>

/**
 * Convert a hex-string to binary
 * @note The function will silently skip any non-hexadecimal characters it encounters
 * @param dst     Binary output buffer
 * @param dst_len Binary output buffer size
 * @param src     Buffer of input string (may exclude null character)
 * @param src_len Size of src (excluding null character)
 */
void stlink2_hexstr_to_bin(
	void *dst, size_t dst_len,
	const void *src, size_t src_len);

/**
 * Convert binary data to hex-string
 * @param dst     Buffer of output string
 * @param dst_len Size of dst (excluding null character)
 * @param src     Binary input buffer
 * @param src_len Binary input buffer length
 */
void stlink2_hexstr_from_bin(
	void *dst, size_t dst_len,
	const void *src, size_t src_len);

#endif /* STLINK2_UTILS_HEXSTR_H_ */
