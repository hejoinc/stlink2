/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#include <stlink2.h>

#ifdef STLINK2_HAVE_WINDOWS

#include <windows.h>

void stlink2_msleep(int milliseconds)
{
	Sleep(milliseconds);
}

#else

#include <time.h>

void stlink2_msleep(int milliseconds)
{
	struct timespec ts;

	ts.tv_sec = milliseconds / 1000;
	ts.tv_nsec = milliseconds % 1000 * 1000000;
	nanosleep(&ts, NULL);
}

#endif
