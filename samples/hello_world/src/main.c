/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/flash.h>
#include <scu.h>

#define ATTR_INF	"\x1b[32;1m" // ANSI_COLOR_GREEN
#define ATTR_ERR	"\x1b[31;1m" // ANSI_COLOR_RED
#define ATTR_RST	"\x1b[37;1m" // ANSI_COLOR_RESET

#define FLASH_ADDR 0x1000
#define FLASH_RW_SIZE 24

int main(void)
{
	int Cnt = 0;
	struct sensor_value lvTemp = {0}, lvVolt = {0};
	uint8_t chip_id = 0, vendor_id = 0;
	int errorcode = 0;
	// uint8_t flash_data_write[FLASH_RW_SIZE] = { 0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22,
	// 0x33, 0x33, 0x33, 0x33, 0x44, 0x44, 0x44, 0x44, 0x66, 0x66, 0x66, 0x66, 0x88, 0x88, 0x88, 0x88 };
	uint8_t flash_data_read[FLASH_RW_SIZE] = {0};

	soc_get_id(&chip_id, &vendor_id);

	const struct device *pvt = DEVICE_DT_GET(DT_NODELABEL(pvt0));
	const struct device *flash = DEVICE_DT_GET(DT_NODELABEL(mx25u16));

	if((pvt == NULL) || (!device_is_ready(pvt))) {
		printf("%s pvt has status disabled or driver is not initialized...\n", ATTR_ERR);
	} else {
		printf("%s pvt Object is Created\n", ATTR_INF);
				errorcode = sensor_channel_get(pvt, SENSOR_CHAN_DIE_TEMP, &lvTemp);
		if(errorcode == 0) {
			printf("%s Error fetching temperature value. Error code:%u\n", ATTR_ERR, errorcode);
		}

		errorcode = sensor_channel_get(pvt, SENSOR_CHAN_VOLTAGE, &lvVolt);
		if(errorcode == 0) {
			printf("%s Error fetching Voltage value. Error code:%u\n", ATTR_ERR, errorcode);
		}

		printf("%s Die Temperature:%d Voltage:%d\n", ATTR_INF, lvTemp.val1, lvVolt.val1);
	}

	if((flash == NULL) || (!device_is_ready(flash))) {
		printf("%s flash has status disabled or driver is not initialized...\n", ATTR_ERR);
	} else {
		printf("%s flash Object is Created\n", ATTR_INF);
			errorcode = flash_read(flash, FLASH_ADDR, flash_data_read, FLASH_RW_SIZE);
			if(errorcode < 0) {
				printf("%s Error reading from flash with code:%d\n", ATTR_ERR, errorcode);
			} else {
				printf("%s Reading Back Before Erasing Flash\n", ATTR_INF);
				for(uint8_t i = 0; i < FLASH_RW_SIZE; i++) {
					printf("%s 0x%02x", ATTR_INF, flash_data_read[i]);
					if(flash_data_read[i] != 0xff) {
						errorcode = -1;
					}
				}
			}
			errorcode = flash_erase(flash, FLASH_ADDR, 4*1024);
			if(errorcode < 0) {
				printf("%s\nError Erasing the flash at 0x%08x offset errorcode:%d\n", ATTR_ERR, FLASH_ADDR, errorcode);
			} else {
				printf("%s\nSuccessfully Erased Flash\n", ATTR_RST);
				errorcode = flash_read(flash, FLASH_ADDR, flash_data_read, FLASH_RW_SIZE);
				if(errorcode < 0) {
					printf("%s Error reading from flash with code:%d\n", ATTR_ERR, errorcode);
				} else {
					printf("%s Reading Back After Erasing Area of Flash\n", ATTR_INF);
					for(uint8_t i = 0; i < FLASH_RW_SIZE; i++) {
						printf("%s 0x%02x", ATTR_INF, flash_data_read[i]);
						if(flash_data_read[i] != 0xff) {
							errorcode = -1;
						}
					}
				}
				if(errorcode == -1) {
					printf("%s\nFlash erase at 0x%08x did not produce correct results\n", ATTR_ERR, FLASH_ADDR);
				} else {
					printf("%s\nSuccessfully performed erase to flash with code:%d\n", ATTR_INF, errorcode);
				}
				errorcode = 0;
			}
		/*
		if(errorcode == 0) {
			printf("%s Writing the following data After Erasing Flash\n", ATTR_INF);
			for(uint8_t i = 0; i < FLASH_RW_SIZE; i++) {
				printf("%s 0x%02x", ATTR_INF, flash_data_write[i]);
			}
			errorcode = flash_write(flash, FLASH_ADDR, flash_data_write, FLASH_RW_SIZE);
		}
		
		if(errorcode < 0) {
			printf("%s \nError writing to flash with code:%d\n", ATTR_ERR, errorcode);
		} else {
			printf("%s \nSuccessfully written to flash with code:%d\n", ATTR_INF, errorcode);
			errorcode = flash_read(flash, FLASH_ADDR, flash_data_read, FLASH_RW_SIZE);
			if(errorcode < 0) {
				printf("%s Error reading from flash with code:%d\n", ATTR_ERR, errorcode);
			} else {
				printf("%s Successfully Read from flash with code:%d\n", ATTR_INF, errorcode);
				bool Data_Validated = true;
				for(uint8_t i = 0; i < FLASH_RW_SIZE; i++) {
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
		*/
		// if(errorcode == 0) {
		// 	printf("%s Writing the 0x00 as erase data to Flash\n", ATTR_INF);
		// 	int8_t flash_data_erase[FLASH_RW_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		// 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		// 	uint8_t flash_data_read_aft[FLASH_RW_SIZE] = {0};
		// 	errorcode = flash_write(flash, FLASH_ADDR, flash_data_erase, FLASH_RW_SIZE);
		// 	if(errorcode == 0) {
		// 		printf("%s Reading Back After Writing erase data to Flash\n", ATTR_INF);
		// 		errorcode = flash_read(flash, FLASH_ADDR, flash_data_read_aft, FLASH_RW_SIZE);
		// 		if(errorcode == 0) {
		// 			for(uint8_t i = 0; i < FLASH_RW_SIZE; i++) {
		// 				printf("%s 0x%02x", ATTR_INF, flash_data_read_aft[i]);
		// 				if(flash_data_read[i] != 0x00) {
		// 					errorcode = -1;
		// 				}
		// 			}
		// 			if(errorcode == -1) {
		// 				printf("%s \nFlash Manual Erase Failed...\n", ATTR_ERR);
		// 			} else {
		// 				printf("%s \nFlash Manual Erase Passed!!!\n", ATTR_RST);
		// 			}
		// 			errorcode = 0;
		// 		}
		// 	}


		// 	printf("%s Writing the 0xFF data to Flash\n", ATTR_INF);
		// 	uint8_t flash_data_write_aft[FLASH_RW_SIZE] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		// 	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
		// 	uint8_t flash_data_read_after[FLASH_RW_SIZE];
		// 	errorcode = flash_write(flash, FLASH_ADDR, flash_data_write_aft, FLASH_RW_SIZE);
		// 	if(errorcode == 0) {
		// 		printf("%s Reading Back After Writing data to Flash\n", ATTR_INF);
		// 		errorcode = flash_read(flash, FLASH_ADDR, flash_data_read_after, FLASH_RW_SIZE);
		// 		if(errorcode < 0) {
		// 			printf("%s Error reading from flash with code:%d\n", ATTR_ERR, errorcode);
		// 		} else {
		// 			printf("%s Successfully Read from flash with code:%d\n", ATTR_INF, errorcode);
		// 			bool Data_Validated = true;
		// 			for(uint8_t i = 0; i < FLASH_RW_SIZE; i++) {
		// 				if(flash_data_read_after[i] != flash_data_write_aft[i]) {
		// 					Data_Validated = false;
		// 					printf("%s %d - Read:0x%02x != Write:0x%02x\n", ATTR_ERR, i, flash_data_read_after[i], flash_data_write_aft[i]);
		// 				} else {
		// 					printf("%s %d - Read:0x%02x == Write:0x%02x\n", ATTR_INF, i, flash_data_read_after[i], flash_data_write_aft[i]);
		// 				}
		// 			}
		// 			if(!Data_Validated) {
		// 				printf("%s Flash Integrity Check Failed...\n", ATTR_ERR);
		// 			} else{
		// 				printf("%s Flash Integrity Check Passed!!!\n", ATTR_INF);
		// 			}
		// 		}
		// 	}
		// } 		
	}

	while(true) {		
		printf(
				"%s%d - %s [CHIP_ID:0x%02x VENDOR_ID:0x%02x] Die[Temp:%d Volt:%d] mTimerClock = %d Hz\r", 
				ATTR_RST, Cnt++, 
				CONFIG_BOARD_TARGET, 
				chip_id, vendor_id, lvTemp.val1, lvVolt.val1,
				CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC
			  );
		k_msleep(1000);
	}

	return 0;
}
