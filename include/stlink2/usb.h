/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#ifndef STLINK2_USB_H_
#define STLINK2_USB_H_

#include <libusb.h>
#include <stdbool.h>

#define STLINK2_USB_VID_ST   0x0483
#define STLINK2_USB_PID_V1   0x3744
#define STLINK2_USB_PID_V2   0x3748
#define STLINK2_USB_PID_V2_1 0x374b

#define STLINK2_USB_CMD_SIZE 16 /**< USB command size in bytes */

bool stlink2_usb_probe_dev(libusb_device *dev, struct stlink2 *st);
void stlink2_usb_set_name_from_pid(struct stlink2 *dev);
void stlink2_usb_config_endpoints(struct stlink2 *dev);
ssize_t stlink2_usb_send_recv(struct stlink2 *dev,
			      uint8_t *txbuf, size_t txsize,
			      uint8_t *rxbuf, size_t rxsize);

#endif /* STLINK2_USB_H_ */
