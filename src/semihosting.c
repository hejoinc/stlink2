/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#include <stlink2.h>
#include <stdio.h>
#include <stdbool.h>

void stlink2_semihosting_op_sys_open(struct stlink2 *dev)
{
	(void)dev;
}

void stlink2_semihosting_op_sys_close(struct stlink2 *dev)
{
	(void)dev;
}

void stlink2_semihosting_op_sys_writec(struct stlink2 *dev)
{
	ssize_t rc;
	uint32_t data;

	stlink2_read_reg(dev, 1, &data);
	stlink2_read_debug32(dev, data, &data);
	rc = fwrite(&data, 1, 1, stdout);

	if (rc < 0)
		printf("error in write\n");
}

void stlink2_semihosting_op_sys_write0(struct stlink2 *dev)
{
	ssize_t rc;
	uint32_t data;
	uint32_t addr;

	stlink2_read_reg(dev, 1, &data);
	stlink2_read_debug32(dev, data, &addr);

	do {
		stlink2_read_debug32(dev, addr, &data);
		for (size_t n = 0; n < 4; n++) {
			if (((char *)&data)[n] == 0) {
				data = 0;
				break;
			}

			rc = fwrite(&((char *)&data)[n], 1, 1, stdout);
			if (rc < 0) {
				data = 0;
				break;
			}
		}
		addr += 4;
	} while (data != 0);
}

bool stlink2_semihosting(struct stlink2 *dev)
{
	bool ret = false;
	uint32_t data;

	uint32_t pc;
	uint32_t r0;

	stlink2_read_reg(dev, 15, &pc);
	STLINK2_LOG(DEBUG, dev, "pc: 0x%08x\n", pc);
	stlink2_read_debug32(dev, pc, &data);

	if (((data & 0xffff0000) >> 16) == 0xbeab) {
		stlink2_read_reg(dev, 0, &r0);
		switch (r0) {
		case STLINK2_SEMIHOSTING_OP_SYS_WRITEC:
			stlink2_semihosting_op_sys_writec(dev);
			break;
		case STLINK2_SEMIHOSTING_OP_SYS_WRITE0:
			stlink2_semihosting_op_sys_write0(dev);
			break;
		case STLINK2_SEMIHOSTING_OP_SYS_WRITE:
			break;
		case STLINK2_SEMIHOSTING_OP_SYS_FLEN:
			STLINK2_LOG(DEBUG, dev, "SYS_FLEN\n");
			break;
		case STLINK2_SEMIHOSTING_EXCEPTION:
			stlink2_read_reg(dev, 1, &data);
			printf("Exception: %08x\n", data);
			break;
		default:
			break;
		}
		ret = true;
	}

	return ret;
}
