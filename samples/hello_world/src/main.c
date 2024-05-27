/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>

static volatile int JATW = 0;
static volatile int counter = 100;
int main(void)
{
	printf("AZTW Getting into Main Loop\n");

	while(true) {
		printf("%d - AZTW Infinite Main Loop %s\n", JATW++, CONFIG_BOARD_TARGET);
		k_msleep(1000);
	}

	return 0;
}
