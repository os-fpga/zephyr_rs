/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
volatile int I_am_imp = 0;
volatile int counter = 100;
int main(void)
{
	while(I_am_imp < counter) {
		I_am_imp++;
	}
	printf("Hi Rahul we just printed Hello World to our %s\n", CONFIG_BOARD_TARGET);

	return 0;
}
