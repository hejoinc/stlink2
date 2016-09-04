/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#include <stlink2/log.h>
#include <stlink2/build.h>
#include <stlink2-internal.h>

static const char *stlink2_loglevel_str(enum stlink2_loglevel level)
{
	switch (level) {
	case STLINK2_LOGLEVEL_ERROR:
		return "[ERROR]";
	case STLINK2_LOGLEVEL_WARN:
		return "[WARN] ";
	case STLINK2_LOGLEVEL_INFO:
		return "[INFO] ";
	case STLINK2_LOGLEVEL_DEBUG:
		return "[DEBUG]";
	case STLINK2_LOGLEVEL_TRACE:
		return "[TRACE]";
	default:
		break;
	}

	return "";
}

static const char *stlink2_log_file_strip_prefix(const char *file, const char *prefix)
{
	const char *_file = file;

	for (size_t n = 0; ; n++) {
		if (file[n] == 0 || prefix[n] == 0) {
			_file = &file[n];
			break;
		}
	}

	return _file;
}

void stlink2_log(enum stlink2_loglevel level, const char *file, unsigned int line, stlink2_t dev,
		 const char *format, ...)
{
	if (!dev->log.fp || level > dev->log.level)
		return;

	va_list args;

	if (file && line) {
		file = stlink2_log_file_strip_prefix(file, STLINK2_BUILD_SOURCE_DIR);
		fprintf(dev->log.fp, "%s %s:%d : ", stlink2_loglevel_str(level), file, line);
	}

	va_start(args, format);
	vfprintf(dev->log.fp, format, args);
	va_end(args);
}

void stlink2_log_set_file(stlink2_t dev, FILE *file)
{
	dev->log.fp = file;
}

void stlink2_log_set_level(stlink2_t dev, enum stlink2_loglevel level)
{
	dev->log.level = level;
}
