#include <stdlib.h>
#include <stlink2.h>

int main(void)
{
	stlink2_t dev;

	stlink2_init();

	dev = stlink2_open(NULL);
	if (dev) {
		printf("serial: %s\n", stlink2_get_serial(dev));
		printf("  name: %s\n", stlink2_get_name(dev));
	}
	stlink2_close(&dev);

	dev = stlink2_open("066DFF485550755187254525");
	if (dev) {
		printf("serial: %s\n", stlink2_get_serial(dev));
		printf("  name: %s\n", stlink2_get_name(dev));
	}
	stlink2_close(&dev);

	stlink2_exit();
}
