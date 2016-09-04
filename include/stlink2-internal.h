/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#ifndef STLINK2_INTERNAL_H_
#define STLINK2_INTERNAL_H_

#include <stdint.h>
#include <stlink2/log.h>
#include <stlink2/usb.h>

struct stlink2 {
	char *serial;
	const char *name;
	struct {
		enum stlink2_loglevel level;
		FILE *fp;
	} log;
	struct {
		uint32_t coreid;
		uint32_t chipid;
		uint32_t cpuid;
		uint32_t flash_size;
		char *unique_id;
	} mcu;
	struct {
		uint8_t stlink;
		uint8_t jtag;
		uint8_t swim;
		char *version;
	} fw;
	struct {
		uint16_t pid;
		libusb_device_handle *dev;
		uint8_t rx_ep;
		uint8_t tx_ep;
		unsigned int timeout;
	} usb;
};

#endif /* STLINK2_INTERNAL_H_ */
