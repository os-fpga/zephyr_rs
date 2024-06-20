/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/flash.h>
#include <scu.h>

#define ATTR_INF	"\x1b[32;1mINF " // ANSI_COLOR_GREEN
#define ATTR_ERR	"\x1b[31;1mERR " // ANSI_COLOR_RED

int main(void)
{
	int Cnt = 0;
	struct sensor_value lvTemp = {0}, lvVolt = {0};
	uint8_t chip_id = 0, vendor_id = 0, errorcode = 0;
	uint8_t flash_data_write[8] = {0x1, 0x3, 0x5, 0x9, 0x7, 0x5, 0x3, 0x1};
	uint8_t flash_data_read[8] = {0};
	soc_get_id(&chip_id, &vendor_id);	
	const struct device *pvt = DEVICE_DT_GET(DT_NODELABEL(pvt0));
	const struct device *flash = DEVICE_DT_GET(DT_NODELABEL(mx25u16));

	if((pvt == NULL) || (!device_is_ready(pvt))) {
		printf("%s pvt has status disabled or driver is not initialized...\n", ATTR_ERR);
	} else {
		printf("%s pvt Object is Created\n", ATTR_INF);
				errorcode = sensor_channel_get(pvt, SENSOR_CHAN_DIE_TEMP, &lvTemp);
		if(errorcode) {
			printf("%s Error fetching temperature value. Error code:%u\n", ATTR_ERR, errorcode);
		}

		errorcode = sensor_channel_get(pvt, SENSOR_CHAN_VOLTAGE, &lvVolt);
		if(errorcode) {
			printf("%s Error fetching Voltage value. Error code:%u\n", ATTR_ERR, errorcode);
		}

		printf("%s Die Temperature:%d Voltage:%d\n", ATTR_INF, lvTemp.val1, lvVolt.val1);
	}

	if((flash == NULL) || (!device_is_ready(flash))) {
		printf("%s flash has status disabled or driver is not initialized...\n", ATTR_ERR);
	} else {
		printf("%s flash Object is Created\n", ATTR_INF);
		errorcode = flash_erase(flash, 0x10000, 4096);
		if(errorcode) {
			printf("%s Error Erasing the flash at 0x1000 offset\n", ATTR_ERR);
		} else {
			printf("%s Successfully Erased Flash\n", ATTR_INF);
			errorcode = flash_read(flash, 0x1000, flash_data_read, 8);
			if(errorcode) {
				printf("%s Error reading from flash with code:%d\n", ATTR_ERR, errorcode);
			} else {
				for(uint8_t i = 0; i < 8; i++) {
					printf("%s 0x%02x\n", ATTR_INF, flash_data_read[i]);
				}
			}
		}
		errorcode = flash_write(flash, 0x1000, flash_data_write, 8);
		if(errorcode) {
			printf("%s Error writing to flash with code:%d\n", ATTR_ERR, errorcode);
		} else {
			printf("%s Successfully written to flash with code:%d\n", ATTR_INF, errorcode);
			errorcode = flash_read(flash, 0x1000, flash_data_read, 8);
			if(errorcode) {
				printf("%s Error reading from flash with code:%d\n", ATTR_ERR, errorcode);
			} else {
				printf("%s Successfully Read from flash with code:%d\n", ATTR_INF, errorcode);
				bool Data_Validated = true;
				for(uint8_t i = 0; i < 8; i++) {
					if(flash_data_read[i] != flash_data_write[i]) {
						Data_Validated = false;
						printf("%s %d - Read:0x%02x != Write:0x%02x\n", ATTR_ERR, i, flash_data_read[i], flash_data_write[i]);
					} else {
						printf("%s %d - Read:0x%02x == Write:0x%02x\n", ATTR_INF, i, flash_data_read[i], flash_data_write[i]);
					}
				}
				if(!Data_Validated) {
					printf("%s Flash Integrity Check Failed...\n", ATTR_ERR);
				} else{
					printf("%s Flash Integrity Check Passed!!!\n", ATTR_INF);
				}
			}
		}
	}

	while(true) {		
		printf(
				"%s%d - %s [CHIP_ID:0x%02x VENDOR_ID:0x%02x] Die[Temp:%d Volt:%d] mTimerClock = %d Hz\r", 
				ATTR_INF, Cnt++, 
				CONFIG_BOARD_TARGET, 
				chip_id, vendor_id, lvTemp.val1, lvVolt.val1,
				CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC
			  );
		k_msleep(1000);
	}

	return 0;
}
