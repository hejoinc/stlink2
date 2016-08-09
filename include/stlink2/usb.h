/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#ifndef STLINK2_USB_H_
#define STLINK2_USB_H_

#include <stlink2.h>
#include <libusb.h>

#define STLINK2_USB_VID_ST   0x0483
#define STLINK2_USB_PID_V1   0x3744
#define STLINK2_USB_PID_V2   0x3748
#define STLINK2_USB_PID_V2_1 0x374b

void stlink2_usb_claim(struct stlink2 *st);
void stlink2_usb_config_endpoints(struct stlink2 *dev);
char *stlink2_usb_read_serial(libusb_device_handle *handle, struct libusb_device_descriptor *desc);
ssize_t stlink2_usb_send_recv(struct stlink2 *dev,
			      uint8_t *txbuf, size_t txsize,
			      uint8_t *rxbuf, size_t rxsize);

#endif /* STLINK2_USB_H_ */
