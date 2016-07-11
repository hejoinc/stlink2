#include <stdlib.h>
#include <stlink2.h>

int main(void)
{
	stlink2_t dev;

	dev = stlink2_open(NULL);
	stlink2_close(&dev);
}
