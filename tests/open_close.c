#include <stdlib.h>
#include <stlink2.h>

static void test_open_close(const char *serial)
{
	stlink2_t dev;

	dev = stlink2_open(serial);
	if (dev) {
		stlink2_log_set_level(dev, STLINK2_LOGLEVEL_DEBUG);
		printf("  serial: %s\n",  stlink2_get_serial(dev));
		printf("    name: %s\n",  stlink2_get_name(dev));
		printf(" version: %s\n",  stlink2_get_version(dev));
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
