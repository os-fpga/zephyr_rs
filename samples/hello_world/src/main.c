/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

int main(void)
{
	printf("Hi Rahul we just printed Hello World to our %s\n", CONFIG_BOARD_TARGET);

	return 0;
}
