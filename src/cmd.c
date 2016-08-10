/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#include <stlink2/cmd.h>

uint32_t stlink2_cmd_get_chipid(struct stlink2 *dev)
{
	uint32_t chipid = 0x0;

	/** @todo get rid of magic values and second read */
	stlink2_read_debug32(dev, 0xE0042000, &chipid);
	if (!chipid)
		stlink2_read_debug32(dev, 0x40015800, &chipid);

	return chipid;
}
