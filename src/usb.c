/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#include <stlink2.h>
#include <stlink2/utils/hexstr.h>

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define STLINK2_USB_RX_EP      (1 | LIBUSB_ENDPOINT_IN)  /**< USB RX endpoint */
#define STLINK2_USB_V2_TX_EP   (2 | LIBUSB_ENDPOINT_OUT) /**< USB TX endpoint for Stlink2 */
#define STLINK2_USB_V2_1_TX_EP (1 | LIBUSB_ENDPOINT_OUT) /**< USB TX endpoint for Stlink2-1 */

/** @todo move to aux */
static char *stlink2_strdup(const char *s)
{
	const size_t n = strlen(s);
	char *p = malloc(n + 1);

	if (p) {
		memcpy(p, s, n);
		p[n] = 0;
	}

	return p;
}

static bool stlink2_usb_claim(struct stlink2 *dev)
{
	int ret;
	int config;

	ret = libusb_kernel_driver_active(dev->usb.dev, 0);
	if (ret) {
		ret = libusb_detach_kernel_driver(dev->usb.dev, 0);
		if (ret) {
			STLINK2_LOG(ERROR, dev, "libusb_detach_kernel_driver failed\n");
			return false;
		}
	}

	ret = libusb_get_configuration(dev->usb.dev, &config);
	if (ret) {
		STLINK2_LOG(ERROR, dev, "libusb_get_configuration failed\n");
		return false;
	}

	ret = libusb_set_configuration(dev->usb.dev, 1);
	if (ret) {
		STLINK2_LOG(ERROR, dev, "libusb_set_configuration failed\n");
		return false;
	}

	ret = libusb_claim_interface(dev->usb.dev, 0);
	if (ret) {
		STLINK2_LOG(ERROR, dev, "libusb_claim_interface failed\n");
		return false;
	}

	return true;
}

bool stlink2_usb_probe_dev(libusb_device *dev, struct stlink2 *st)
{
	int ret = 0;
	struct libusb_device_descriptor desc;
	libusb_device_handle *devh;

	ret = libusb_get_device_descriptor(dev, &desc);
	if (ret < 0)
		return false;

	if (desc.idProduct != STLINK2_USB_PID_V2 &&
	    desc.idProduct != STLINK2_USB_PID_V2_1)
		return false;

	ret = libusb_open(dev, &devh);
	if (ret < 0)
		return false;

	st->usb.dev   = devh;
	stlink2_log_set_file(st, stdout);
	stlink2_log_set_level(st, STLINK2_LOGLEVEL_INFO);
	st->usb.pid   = desc.idProduct;
	st->serial    = stlink2_usb_read_serial(st->usb.dev, &desc);

	stlink2_usb_claim(st);
	stlink2_usb_config_endpoints(st);
	stlink2_usb_set_name_from_pid(st);

	return true;
}

/**
 * Set programmer name based on USB PID
 */
void stlink2_usb_set_name_from_pid(struct stlink2 *dev)
{
	static const char *stlinkv2   = "st-link/v2";
	static const char *stlinkv2_1 = "st-link/v2-1";

	if (dev->usb.pid == STLINK2_USB_PID_V2)
		dev->name = stlinkv2;
	else if (dev->usb.pid == STLINK2_USB_PID_V2_1)
		dev->name = stlinkv2_1;
}

char *stlink2_usb_read_binary_serial(const char *serial, size_t len)
{
	/** @todo need to get rid of this weird calculation... + 1, - 1 */
	const size_t size = (len * 2) + 1;
	char *_serial = malloc(size);

	if (_serial) {
		stlink2_hexstr_from_bin(_serial, size - 1, serial, len);
		_serial[size - 1] = 0;
	}

	return _serial;
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
		if (isxdigit(serial[n]))
			continue;

		ishexserial = false;
		break;
	}

	if (!ishexserial)
		return stlink2_usb_read_binary_serial(serial, ret);

	return stlink2_strdup(serial);
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

	STLINK2_LOG(TRACE, dev, "USB > ");
	for (size_t n = 0; n < txsize; n++)
		STLINK2_LOG_WRITE(TRACE, dev, "%02x ", txbuf[n]);
	STLINK2_LOG_WRITE(TRACE, dev, "\n");

	if (!rxbuf || !rxsize)
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

	STLINK2_LOG(TRACE, dev, "USB < ");
	for (size_t n = 0; n < rxsize; n++)
		STLINK2_LOG_WRITE(TRACE, dev, "%02x ", rxbuf[n]);
	STLINK2_LOG_WRITE(TRACE, dev, "\n");

	return 0;
}
