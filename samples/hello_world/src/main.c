/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>

static volatile int JATW = 0;
static volatile const int counter = 100;
int main(void)
{
	printf("Getting into Main Loop\n");
	while(JATW < counter) {
		JATW++;
		printf("%d - Hello %s\n", JATW, CONFIG_BOARD_TARGET);
		k_msleep(1000);
	}
	while(true) {
		printf("Out of the Main Loop\n");
		k_msleep(1000);
	}

	return 0;
}
