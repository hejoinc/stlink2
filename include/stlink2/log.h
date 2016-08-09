/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#ifndef STLINK2_LOG_H_
#define STLINK2_LOG_H_

#include <stlink2.h>
#include <stdio.h>
#include <stdarg.h>

enum stlink2_loglevel {
	STLINK2_LOGLEVEL_QUIET,
	STLINK2_LOGLEVEL_ERROR,
	STLINK2_LOGLEVEL_WARN,
	STLINK2_LOGLEVEL_INFO,
	STLINK2_LOGLEVEL_DEBUG,
	STLINK2_LOGLEVEL_TRACE
};

int stlink2_log_printf(enum stlink2_loglevel level, struct stlink2 *st, const char *format, ...);

#endif /* STLINK2_LOG_H_ */
