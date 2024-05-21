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
	printf("Getting into Main Loop\n");

	while(true) {
		printf("%d - Infinite Main Loop %s\n", JATW++, CONFIG_BOARD_TARGET);
		for(unsigned int i = 0; i < 500000; i++) {
			counter++;
		} counter = 0;
	}

	return 0;
}
