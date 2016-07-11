#include <stdio.h>
#include <stlink2.h>

int main(void)
{
/*
	stlink2_t st;

	stlink2_probe();

	printf("====== name: %s\n", st->name);
	stlink2_get_version(st);
	stlink2_get_mode(st);
	stlink2_set_mode_swd(st);
	stlink2_get_coreid(st);
	stlink2_get_chipid(st);

	stlink2_mcu_halt(st);
	stlink2_mcu_reset(st);
	stlink2_mcu_run(st);

	while (1) {
		while (stlink2_get_status(st) == STLINK2_STATUS_CORE_RUNNING)
			stlink2_msleep(10);

		if (stlink2_semihosting(st)) {
			uint32_t pc;

			stlink2_read_reg(st, 15, &pc);
			pc += 2;
			stlink2_write_reg(st, 15, pc);
			stlink2_read_reg(st, 15, &pc);

			stlink2_mcu_run(st);
		}
	}

#ifdef TEST
	stlink2_read_all_regs(st);
	stlink2_read_reg(st, 0, &val);
	stlink2_mcu_run(st);
	stlink2_stm32x_info(st, descr, sizeof(descr));
	printf("      descr: %s\n\n", descr);
#endif
	*/

	return 0;
}
