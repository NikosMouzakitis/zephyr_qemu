/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <string.h>

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

struct printk_data_t {
	void *fifo_reserved; /* 1st word reserved for use by fifo */
	uint32_t led;
	uint32_t cnt;
	unsigned char character;
};

K_FIFO_DEFINE(printk_fifo);


void blink(uint32_t sleep_ms, uint32_t id,unsigned char ch)
{
	int cnt = 0;
	printk("Blink called with id: %d and char: %c\n", id, ch);
	

	while (1) {
		struct printk_data_t tx_data = { .led = id, .cnt = cnt, .character = ch };

		size_t size = sizeof(struct printk_data_t);
		char *mem_ptr = k_malloc(size);
		__ASSERT_NO_MSG(mem_ptr != 0);

		memcpy(mem_ptr, &tx_data, size);
		printk("---------------> id: %d puts character: %c\n",id, ch);
		k_fifo_put(&printk_fifo, mem_ptr);
		k_msleep(sleep_ms);
		cnt++;
		ch++;
		printk("---------------> id: %d updated cnt to: %d\n",id, cnt);

		if(cnt == 4) 
		{
			printk("stopping thread: %d\n",id);
			while(1)
			{
				//important so the other thread has the chance to be executed!
				k_msleep(200);
			}

		}
	}
}

void blink0(void)
{
	printk("blinky0 initialized\n");
	blink(8000000,0,'a');

}

void blink1(void)
{
	printk("blinky1 initialized\n");
	blink(2000000,1,'A');
}

void uart_out(void)
{
	printk("uart_out starts\n");
	while (1) {
		printk("uart_out run while()\n");
		struct printk_data_t *rx_data = k_fifo_get(&printk_fifo, K_FOREVER);
		printk("************ ******************  **************** *****************>Toggled led%d, counter=%d, character= %c\n", rx_data->led, rx_data->cnt, rx_data->character);
		k_free(rx_data);
	}
}

K_THREAD_DEFINE(uart_out_id, STACKSIZE, uart_out, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(blink0_id, STACKSIZE, blink0, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(blink1_id, STACKSIZE, blink1, NULL, NULL, NULL,	PRIORITY, 0, 0);
