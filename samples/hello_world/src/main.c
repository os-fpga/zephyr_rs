/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <soc.h>

int main(void)
{
	int Cnt = 0;
	uint8_t chip_id = 0, vendor_id = 0;
	soc_get_id(&chip_id, &vendor_id);
	struct sensor_value lvTemp = {0}, lvVolt = {0};
	const struct device *pvt = DEVICE_DT_GET(DT_NODELABEL(pvt0));

	if(pvt == NULL) {
		perror("PVT0 has status disabled or driver is not initialized...\n");
	} else {
		printf("PVT0 Object is Created\n");
	}

	if(!device_is_ready(pvt)) {
		perror("Error with device initialization\n");
		return -ENODEV;
	} else {
		sensor_channel_get(pvt, SENSOR_CHAN_DIE_TEMP, &lvTemp);
		sensor_channel_get(pvt, SENSOR_CHAN_VOLTAGE, &lvVolt);
		printf("Temperature Value %d Voltage %d\n", lvTemp.val1, lvVolt.val1);
	}

	while(true) {		
		printf(
				"%d - %s [CHIP_ID:0x%02x VENDOR_ID:0x%02x] mTimerClock = %d Hz\n", 
				Cnt++, 
				CONFIG_BOARD_TARGET, 
				chip_id, vendor_id,
				CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC
			  );
		k_msleep(1000);
	}

	return 0;
}
