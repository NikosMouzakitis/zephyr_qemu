/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

//#include <stdio.h>
#include <zephyr/kernel.h>
//#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
//#include <errno.h>
//#include <zephyr/types.h>
//#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
//#include <inttypes.h>
#include <zephyr/drivers/gpio/gpio_emul.h>

#define SLEEP_TIME_MS  276447232

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
//static const struct gpio_dt_spec led =   GPIO_DT_SPEC_GET_OR(DT_NODELABEL(led0), gpios, {0});

int main(void)
{

	int ret,my_val;
	bool led_state = true;
	uint32_t *ra;	
	
	if (!gpio_is_ready_dt(&led)) {
		printk("Failed gpio_is_ready(&led)\n");
		return 0;
	}
	ret = gpio_pin_configure(led.port,0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("error on led configure: %d\n",ret);
		return 0;
	}
	//check if the device is ready
	if (led.port && !gpio_is_ready_dt(&led)) {
		printk("Error %d: LED device %s is not ready\n",ret, led.port->name);
		return 0;
	} 

	const struct device *dev;
	dev = device_get_binding(led.port->name);
	if (dev == NULL) {
		printk("dev == NULL!\n");
		return 0;
	} 

	while (1) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			printk("gpio_pin_toggle_dt() failed.\n");
			return 0;
		} 

		led_state = !led_state;
		printf("LED state: %s\n", led_state ? "ON" : "OFF");
                my_val = gpio_emul_output_get(dev, 0);	
		printk(" [ %d ]  \n",my_val);
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
