/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#include <stlink2.h>

int stlink2_log_printf(enum stlink2_loglevel level, struct stlink2 *st, const char *restrict format, ...)
{
	if (level > st->log.level)
		return 0;

	int ret;
	va_list args;

	va_start(args, format);
	ret = vfprintf(st->log.fp, format, args);
	va_end(args);

	return ret;
}
