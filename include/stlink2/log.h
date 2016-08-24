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

#define STLINK2_LOG_ERROR(dev, format, ...) \
	stlink2_log(STLINK2_LOGLEVEL_ERROR, __FILE__, __LINE__, dev, format, ##__VA_ARGS__)
#define STLINK2_LOG_WARN(dev, format, ...) \
	stlink2_log(STLINK2_LOGLEVEL_WARN, __FILE__, __LINE__, dev, format, ##__VA_ARGS__)
#define STLINK2_LOG_INFO(dev, format, ...) \
	stlink2_log(STLINK2_LOGLEVEL_INFO, __FILE__, __LINE__, dev, format, ##__VA_ARGS__)
#define STLINK2_LOG_DEBUG(dev, format, ...) \
	stlink2_log(STLINK2_LOGLEVEL_DEBUG, __FILE__, __LINE__, dev, format, ##__VA_ARGS__)
#define STLINK2_LOG_TRACE(dev, format, ...) \
	stlink2_log(STLINK2_LOGLEVEL_TRACE, __FILE__, __LINE__, dev, format, ##__VA_ARGS__)

void stlink2_log_set_file(struct stlink2 *dev, FILE *file);
void stlink2_log_set_level(stlink2_t dev, enum stlink2_loglevel level);
void stlink2_log(enum stlink2_loglevel, const char *file, unsigned int line, struct stlink2 *dev, const char *format, ...);

#endif /* STLINK2_LOG_H_ */
