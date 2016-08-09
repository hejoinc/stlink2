/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#ifndef STLINK2_LOG_H_
#define STLINK2_LOG_H_

int stlink2_log_printf(enum stlink2_loglevel level, struct stlink2 *st, const char * restrict format, ...);

#endif /* STLINK2_LOG_H_ */
