#include <stlink2.h>

#ifdef STLINK2_HAVE_WINDOWS

#include <windows.h>

void stlink2_msleep(int milliseconds)
{
	Sleep(milliseconds);
}

#else

#define _GNU_SOURCE
#include <time.h>

void stlink2_msleep(int milliseconds)
{
	struct timespec ts;

	ts.tv_sec = milliseconds / 1000;
	ts.tv_nsec = milliseconds % 1000 * 1000000;
	nanosleep(&ts, NULL);
}

#endif
