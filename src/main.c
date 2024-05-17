/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "retained.h"

#include <inttypes.h>
#include <stdio.h>

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/pm/device.h>
#include <zephyr/sys/poweroff.h>
#include <zephyr/sys/util.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(sys_off);

static const struct gpio_dt_spec sw0 = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);

int main(void)
{
	int rc;
	const struct device *const cons = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

	if (!device_is_ready(cons)) {
		LOG_ERR("%s: device not ready.", cons->name);
		return 0;
	}

	LOG_INF("%s system-off demo", CONFIG_BOARD);

	if (IS_ENABLED(CONFIG_APP_RETENTION)) {
		bool retained_ok = retained_validate();

		/* Increment for this boot attempt and update. */
		retained.boots += 1;
		retained_update();

		LOG_INF("Retained data: %s", retained_ok ? "valid" : "INVALID");
		LOG_INF("Boot count: %u", retained.boots);
		LOG_INF("Off count: %u", retained.off_count);
		LOG_INF("Active Ticks: %" PRIu64, retained.uptime_sum);
	} else {
		LOG_INF("Retained data not supported");
	}

	/* configure sw0 as input, interrupt as level active to allow wake-up */
	rc = gpio_pin_configure_dt(&sw0, GPIO_INPUT);
	if (rc < 0) {
		LOG_ERR("Could not configure sw0 GPIO (%d)", rc);
		return 0;
	}

	rc = gpio_pin_interrupt_configure_dt(&sw0, GPIO_INT_LEVEL_ACTIVE);
	if (rc < 0) {
		LOG_ERR("Could not configure sw0 GPIO interrupt (%d)", rc);
		return 0;
	}

	LOG_INF("Entering system off; press sw0 to restart");

	rc = pm_device_action_run(cons, PM_DEVICE_ACTION_SUSPEND);
	if (rc < 0) {
		LOG_ERR("Could not suspend console (%d)", rc);
		return 0;
	}

	if (IS_ENABLED(CONFIG_APP_RETENTION)) {
		/* Update the retained state */
		retained.off_count += 1;
		retained_update();
	}

	sys_poweroff();

	return 0;
}
