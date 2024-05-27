/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>

static volatile int JATW = 0;

int main(void)
{
	while(true) {
		printf("%d - My Port Checking in to the %s\n", JATW++, CONFIG_BOARD_TARGET);
		k_msleep(1000);
	}

	return 0;
}
