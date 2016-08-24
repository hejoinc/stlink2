#include <stdlib.h>
#include <stlink2.h>
#include <stlink2/cortexm.h>

static void test_open_close(const char *serial)
{
	stlink2_t dev;

	dev = stlink2_open(serial);
	if (dev) {
		uint32_t cpuid;

		stlink2_log_set_level(dev, STLINK2_LOGLEVEL_DEBUG);
		stlink2_set_mode_swd(dev);

		cpuid = stlink2_get_cpuid(dev);

		printf("  serial: %s\n",    stlink2_get_serial(dev));
		printf("    name: %s\n",    stlink2_get_name(dev));
		printf(" version: %s\n",    stlink2_get_version(dev));
		printf(" voltage: %f\n",    stlink2_get_target_voltage(dev));
		printf("   cpuid: %08x\n",  cpuid);
		printf("     partno: %03x (%s)\n", stlink2_cortexm_cpuid_get_partno(cpuid), stlink2_cortexm_cpuid_get_partno_str(cpuid));
		printf("  coreid: %08x\n",  stlink2_get_coreid(dev));
	}
	stlink2_close(&dev);
}

int main(void)
{
	stlink2_init();

	test_open_close("503f7206506752553329033f");
	test_open_close("066DFF485550755187254525");

	stlink2_exit();
}
