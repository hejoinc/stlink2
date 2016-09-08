#include <stdlib.h>
#include <stlink2.h>

void dump_hex(const void * data, size_t size)
{
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			printf(" ");
			if ((i+1) % 16 == 0) {
				printf("|  %s \n", ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					printf("   ");
				}
				printf("|  %s \n", ascii);
			}
		}
	}
}

static void test_open_close(const char *serial)
{
	stlink2_t dev;

	dev = stlink2_open(serial);
	if (dev) {
		uint32_t cpuid;
		uint16_t devid;

		stlink2_log_set_level(dev, STLINK2_LOGLEVEL_INFO);

		printf("  serial: %s\n",    stlink2_get_serial(dev));
		printf("    name: %s\n",    stlink2_get_name(dev));
		printf(" version: %s\n",    stlink2_get_version(dev));

		stlink2_get_mode(dev);
		stlink2_set_mode_swd(dev);
		stlink2_mcu_halt(dev);

		cpuid = stlink2_get_cpuid(dev);
		devid = stlink2_get_devid(dev);

		printf(" voltage: %f\n",    stlink2_get_target_voltage(dev));
		printf("   cpuid: %08x\n",  cpuid);
		printf("     partno: %03x (%s)\n", stlink2_cortexm_cpuid_get_partno(cpuid), stlink2_cortexm_cpuid_partno_str(cpuid));
		printf("  coreid: %08x\n",  stlink2_get_coreid(dev));
		printf("  chipid: %08x\n",  stlink2_get_chipid(dev));
		printf("   devid: %s (0x%03x)\n", stlink2_stm32_devid_str(devid), devid);
		printf("  sram size: %dKiB\n", stlink2_get_sram_size(dev));
		printf("  flash size: %dKiB\n", stlink2_get_flash_size(dev));
		printf("  unique id: %s\n", stlink2_get_unique_id(dev));

#ifdef BLA
		uint8_t *flash = malloc(1024);
		stlink2_read_mem(dev, 0x08000000, flash, 1024);
		dump_hex(flash, 1024);
#endif
#ifdef SEMIHOSTING
		while (true) {
			if (stlink2_get_status(dev) == STLINK2_STATUS_CORE_HALTED)
				stlink2_mcu_run(dev);
			stlink2_semihosting(dev);
			stlink2_msleep(1);
		}
#endif
	}
	stlink2_close(&dev);
}

int main(void)
{
	stlink2_init();

	test_open_close(NULL);
	//test_open_close("503f7206506752553329033f");
	//test_open_close("066DFF485550755187254525");

	stlink2_exit();
}
