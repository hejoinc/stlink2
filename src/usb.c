/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#include <stlink2.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define STLINK2_USB_RX_EP      (1 | LIBUSB_ENDPOINT_IN)  /**< USB RX endpoint */
#define STLINK2_USB_V2_TX_EP   (2 | LIBUSB_ENDPOINT_OUT) /**< USB TX endpoint for Stlink2 */
#define STLINK2_USB_V2_1_TX_EP (1 | LIBUSB_ENDPOINT_OUT) /**< USB TX endpoint for Stlink2-1 */

/** @todo move to aux */
static char *stlink2_strdup(const char *s)
{
	const size_t n = strlen(s);
	char *p = malloc(n + 1);

	if (p)
		memcpy(p, s, n);

	return p;
}

/**
 * Read binary or hex encoded serial from usb handle
 * @note The pointer must be freed by the callee when != NULL
 * @return hex encoded string
 */
char *stlink2_usb_read_serial(libusb_device_handle *handle, struct libusb_device_descriptor *desc)
{
	bool ishexserial = true;
	int ret;
	char serial[256];

	memset(serial, 0, sizeof(serial));

	ret = libusb_get_string_descriptor_ascii(handle, desc->iSerialNumber, (unsigned char *)&serial, sizeof(serial));
	if (ret < 0)
		return NULL;

	for (int n = 0; n < ret; n++) {
		if (!isxdigit(serial[n])) {
			ishexserial = false;
			break;
		}
	}

	if (ishexserial) {
		printf("     serial: %s\n", serial);
	} else {
		printf("     serial: ");

		for (int n = 0; n < ret; n++)
			printf("%02X", serial[n]);

		printf("\n");
	}

	return stlink2_strdup(serial);
}

void stlink2_usb_claim(struct stlink2 *st)
{
	int ret;
	int config;

	ret = libusb_kernel_driver_active(st->usb.dev, 0);
	if (ret) {
		ret = libusb_detach_kernel_driver(st->usb.dev, 0);
		if (ret) {
			printf("Unable to detach\n");
			return;
		}
	}

	ret = libusb_get_configuration(st->usb.dev, &config);
	if (ret) {
		printf("Unable to get configuration\n");
		return;
	}

	ret = libusb_set_configuration(st->usb.dev, 1);
	if (ret) {
		printf("Unable to set configuration\n");
		return;
	}

	ret = libusb_claim_interface(st->usb.dev, 0);
	if (ret) {
		printf("unable to claim\n");
		return;
	}
}

void stlink2_usb_config_endpoints(struct stlink2 *dev)
{
	dev->usb.tx_ep = STLINK2_USB_RX_EP;
	if (dev->usb.pid == STLINK2_USB_PID_V2)
		dev->usb.rx_ep = STLINK2_USB_V2_TX_EP;
	else if (dev->usb.pid == STLINK2_USB_PID_V2_1)
		dev->usb.rx_ep = STLINK2_USB_V2_1_TX_EP;
}

ssize_t stlink2_usb_send_recv(struct stlink2 *dev,
			      uint8_t *txbuf, size_t txsize,
			      uint8_t *rxbuf, size_t rxsize)
{
	int ret;
	int res;

	ret = libusb_bulk_transfer(dev->usb.dev, dev->usb.rx_ep,
				   txbuf,
				   (int)txsize,
				   &res,
				   3000);
	if (ret < 0) {
		printf("error in tx: %s\n", libusb_error_name(ret));
		return 0;
	}

	stlink2_log_printf(STLINK2_LOGLEVEL_TRACE, dev, "req: ");
	for (size_t n = 0; n < txsize; n++)
		stlink2_log_printf(STLINK2_LOGLEVEL_TRACE, dev, "%02x ", txbuf[n]);
	stlink2_log_printf(STLINK2_LOGLEVEL_TRACE, dev, "\n");

	if (!rxbuf && !rxsize)
		return 0;

	ret = libusb_bulk_transfer(dev->usb.dev, dev->usb.tx_ep,
				   rxbuf,
				   (int)rxsize,
				   &res,
				   3000);
	if (ret < 0) {
		printf("error in rx: %s\n", libusb_error_name(ret));
		return 0;
	}

	stlink2_log_printf(STLINK2_LOGLEVEL_TRACE, dev, "rep: ");
	for (size_t n = 0; n < rxsize; n++)
		stlink2_log_printf(STLINK2_LOGLEVEL_TRACE, dev, "%02x ", rxbuf[n]);
	stlink2_log_printf(STLINK2_LOGLEVEL_TRACE, dev, "\n");

	return 0;
}
