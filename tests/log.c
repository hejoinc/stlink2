/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#include <stdlib.h>
#include <stlink2.h>
#include <stlink2-internal.h>

int main(void)
{
	struct stlink2 dev;

	stlink2_log_set_file(&dev, stdout);
	stlink2_log_set_level(&dev, STLINK2_LOGLEVEL_TRACE);

	STLINK2_LOG(ERROR, &dev, "error msg\n");
	STLINK2_LOG(WARN, &dev,  "warning msg\n");
	STLINK2_LOG(INFO, &dev,  "info msg\n");
	STLINK2_LOG(DEBUG, &dev, "debug msg\n");
	STLINK2_LOG(TRACE, &dev, "trace msg\n");
}
