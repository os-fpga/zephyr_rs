/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/counter.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/drivers/dma.h>
#include <zephyr/crypto/crypto_otp_mem.h>
#include <zephyr/crypto/crypto.h>

#include <scu.h>

#define ATTR_INF	"\x1b[32;1m" // ANSI_COLOR_GREEN
#define ATTR_ERR	"\x1b[31;1m" // ANSI_COLOR_RED
#define ATTR_RST	"\x1b[37;1m" // ANSI_COLOR_RESET
#define ATTR_HIL	"\x1b[33;1m" // ANSI_COLOR_HIGHLIGHT

#define FLASH_RW_SIZE 255
#define FLASH_BASE_ADDR 0xB0000000

void Flash_Test(const struct device *flash, const struct device *dma, uint32_t FLASH_ADDR, uint8_t EVEN_ODD_MUL, uint8_t FORMATTER) {
	int errorcode = 0;
	uint8_t flash_data_write[FLASH_RW_SIZE] = {0};
	uint8_t flash_data_read[FLASH_RW_SIZE] = {0};
	errorcode = flash_read(flash, FLASH_ADDR, flash_data_read, FLASH_RW_SIZE);
	if(errorcode < 0) {
		printf("%s Error reading from flash with code:%d%s\n", ATTR_ERR, errorcode,ATTR_RST);
	} 
	else {
		printf("%s Reading Back Before Erasing Flash%s\n", ATTR_INF,ATTR_RST);
		// for(uint8_t i = 0; i < FLASH_RW_SIZE; i++) {
		// 	printf("%s %d%s", ATTR_INF, flash_data_read[i],i%FORMATTER==0?"\n":"");
		// } printf("\n");
	}
	errorcode = flash_erase(flash, FLASH_ADDR, 0x1000);
	if(errorcode < 0) {
		printf("%s\nError Erasing the flash at 0x%08x offset errorcode:%d%s\n", ATTR_ERR, FLASH_ADDR, errorcode, ATTR_RST);
	} else {
		printf("%s\nSuccessfully Erased Flash\n", ATTR_RST);
		errorcode = flash_read(flash, FLASH_ADDR, flash_data_read, FLASH_RW_SIZE);
		if(errorcode < 0) {
			printf("%s Error reading from flash with code:%d%s\n", ATTR_ERR, errorcode,ATTR_RST);
		} else {
			printf("%s Reading Back After Erasing Area of Flash%s\n", ATTR_INF,ATTR_RST);
			for(uint8_t i = 0; i < FLASH_RW_SIZE; i++) {
				if(flash_data_read[i] != 0xff) {
					errorcode = -1;
				}
			}
		}
		if(errorcode == -1) {
			printf("%s\nFlash erase at 0x%08x did not produce correct results%s\n", ATTR_ERR, FLASH_ADDR,ATTR_RST);
			// for(uint8_t i = 0; i < FLASH_RW_SIZE; i++) {
			// 	printf("%s 0x%02x%s", ATTR_INF, flash_data_read[i],i%FORMATTER==0?"\n":"");			
			// } printf("\n");
		} else {
			printf("%s\nSuccessfully performed erase to flash with code:%d%s\n", ATTR_INF, errorcode,ATTR_RST);
		}
		errorcode = 0;
	}

	if(errorcode == 0) {
		printf("%s Writing the following data After Erasing Flash%s\n", ATTR_INF,ATTR_RST);
		for(uint8_t i = 0; i < FLASH_RW_SIZE; i++) { 
			flash_data_write[i] = ((rand() % (FLASH_RW_SIZE - 1 + 1)) + 1)+((rand() % (FLASH_RW_SIZE - 1 + 1)) + 1);
			// printf("%s %d%s", ATTR_INF, flash_data_write[i],i%FORMATTER==0?"\n":"");
		} 
		// printf("\n");
		errorcode = flash_write(flash, FLASH_ADDR, flash_data_write, FLASH_RW_SIZE);
	}

	if(errorcode < 0) {
		printf("%s \nError writing to flash with code:%d%s\n", ATTR_ERR, errorcode,ATTR_RST);
	} else {
		printf("%s \nSuccessfully written to flash with code:%d Resetting the reading buffer....%s\n", ATTR_INF, errorcode,ATTR_RST);
		memset(flash_data_read, 0, FLASH_RW_SIZE);
		errorcode = flash_read(flash, FLASH_ADDR, flash_data_read, FLASH_RW_SIZE);
		if(errorcode < 0) {
			printf("%s Error reading from flash with code:%d%s\n", ATTR_ERR, errorcode,ATTR_RST);
		} else {
			printf("%s Successfully Read from flash with code:%d%s\n", ATTR_INF, errorcode,ATTR_RST);
			bool Data_Validated = true; uint8_t Mismatch_count = 0;
			for(uint8_t i = 0; i < FLASH_RW_SIZE; i++) {
				if(flash_data_read[i] != flash_data_write[i]) {
					Data_Validated = false; Mismatch_count++;
					printf("%s %d - Read:%d != Write:%d%s\n", ATTR_ERR, i, flash_data_read[i], flash_data_write[i],ATTR_RST);
				} 
			}
			if(!Data_Validated) {
				printf("%s Flash Integrity Check Failed With %d Mismatched Entries%s\n", ATTR_ERR,Mismatch_count,ATTR_RST);
			} else{
				printf("%s Flash Integrity Check Passed!!!%s\n", ATTR_INF,ATTR_RST);
			}
		}
	}

	if(errorcode == 0)
	{
		uint8_t *MemMapReadAddr = (uint8_t*)(FLASH_BASE_ADDR + FLASH_ADDR);
		uint8_t MemMapReadDestAddr[FLASH_RW_SIZE];
		{		
			printf("CPU Memory Mapped Reading Test from address:%p\n", MemMapReadAddr);
			bool memmaptestfail = false;
			for(uint8_t i = 0; i < FLASH_RW_SIZE; i++) { 
				if(MemMapReadAddr[i] != flash_data_write[i]) {
					printf("%s %d%s", ATTR_ERR, MemMapReadAddr[i],i%FORMATTER==0?"\n":"");
					memmaptestfail = true;
				} 
				// else {
					// printf("%s %d%s", ATTR_INF, MemMapReadAddr[i],i%FORMATTER==0?"\n":"");
				// }
			} printf("\n");
			if(memmaptestfail) {
				printf("%s CPU Memory mapped read failed\n", ATTR_ERR);	
			} else {
				printf("%s CPU Memory mapped read passed\n", ATTR_INF);	
			}
		} 

		if(dma != NULL) {
			printf("%s DMA Memory Mapped Reading Test from address:%p\n", ATTR_RST, MemMapReadAddr);
			// Configure DMA Block
			struct dma_block_config lv_dma_block_config = {
				.block_size = FLASH_RW_SIZE,
				.dest_addr_adj = DMA_ADDR_ADJ_INCREMENT,
				.dest_address = (uint32_t)&MemMapReadDestAddr[0],
				.dest_reload_en = 0,
				.dest_scatter_count = 0,
				.dest_scatter_en = 0,
				.dest_scatter_interval = 0,
				.fifo_mode_control = 0,
				.flow_control_mode = 0,
				.next_block = NULL,
				.source_addr_adj = DMA_ADDR_ADJ_INCREMENT,
				.source_address = (uint32_t)&MemMapReadAddr[0],
				.source_gather_count = 0,
				.source_gather_en = 0,
				.source_gather_interval = 0,
				.source_reload_en = 0,
				._reserved = 0
			};
			// Configure DMA
			struct dma_config lv_Dma_Config = {
				.block_count = 1,
				.channel_direction = MEMORY_TO_MEMORY,
				.channel_priority = 1,
				.complete_callback_en = 0,
				.cyclic = 0,
				.dest_burst_length = 1,
				.dest_chaining_en = 0,
				.dest_data_size = 1,
				.dest_handshake = 1,
				.dma_callback = NULL,
				.dma_slot = 0,
				.error_callback_dis = 1,
				.head_block = &lv_dma_block_config,
				.linked_channel = 0,
				.source_burst_length = 1,
				.source_chaining_en = 0,
				.source_data_size = 1,
				.user_data = NULL
			};		
			struct dma_status stat = {0};
			printf("%s DMA Configuration...\n", ATTR_RST);
			int dma_error = dma_config(dma, 1, &lv_Dma_Config);
			k_msleep(10);
			if(dma_error) {
				printf("%s Error %d Configuration...\n", ATTR_RST, dma_error);
			} else {			
				// Call DMA Read
				printf("%s DMA Starting...\n", ATTR_INF);
				dma_error = dma_start(dma, 1);
				k_msleep(10);
			}
			(void)dma_get_status(dma, 1, &stat);
			while(stat.pending_length > 1) {
				dma_error = dma_get_status(dma, 1, &stat);
				if(dma_error) {
					printf("Error %d dma_status...\n", dma_error);
					break;
				} else {
					printf("dma_pending_length:%d \n", stat.pending_length);
				}
			}

			if(!dma_error) {
				bool memmaptestfail = false;
				for(uint8_t i = 0; i < FLASH_RW_SIZE; i++) { 
					if(MemMapReadDestAddr[i] != flash_data_write[i]) {
						printf("%s %d%s", ATTR_ERR, MemMapReadDestAddr[i],i%FORMATTER==0?"\n":"");
						memmaptestfail = true;
					} 
					// else {
						// printf("%s %d%s", ATTR_INF, MemMapReadDestAddr[i],i%FORMATTER==0?"\n":"");
					// }
				} 
				printf("\n");
				if(memmaptestfail) {
					printf("%s DMA Memory mapped read failed\n", ATTR_ERR);	
				} else {
					printf("%s DMA Memory mapped read passed\n", ATTR_INF);	
				}
			} else {
				printf("Error %d DMA Start...\n", dma_error);
			}
		}
	}
}

void CounterCallBack(const struct device *dev, void *UserData)
{
	uint32_t *lvData = ((uint32_t*)UserData);
	printf("\nandestech_atcpit100 %s # %d\n", __func__, *lvData);
	*lvData += 1;
}

void CounterAlarmCallBack(const struct device *dev,
					 uint8_t chan_id, uint32_t ticks,
					 void *UserData)
{
	uint32_t *lvData = ((uint32_t*)UserData);
	printf("\nandestech_atcpit100 %s # %d\n", __func__, *lvData);
}

static uint32_t s_CallBackData = 0;

void CounterTest(const struct device *pit)
{
	struct counter_alarm_cfg lvCntAlarmCfg = {0};
	lvCntAlarmCfg.callback = CounterAlarmCallBack;
	lvCntAlarmCfg.flags = COUNTER_ALARM_CFG_EXPIRE_WHEN_LATE;
	lvCntAlarmCfg.ticks = 24000;
	lvCntAlarmCfg.user_data = (void*)&s_CallBackData;

	struct counter_top_cfg lvCntTopCfg = {0};

	lvCntTopCfg.callback = CounterCallBack;
	lvCntTopCfg.user_data = (void*)&s_CallBackData;
	lvCntTopCfg.flags = COUNTER_TOP_CFG_DONT_RESET;
	lvCntTopCfg.ticks = 6000;

	int lvErrorCode = 0;
	
	lvErrorCode = counter_set_channel_alarm(pit, 0, &lvCntAlarmCfg);
	if(lvErrorCode < 0) {
		printf("%s%s(%d) counter Alarm set error code:%d %s\n", \
		ATTR_ERR,__func__,__LINE__,lvErrorCode,ATTR_RST);
	} 
	/*
	lvErrorCode = counter_set_top_value(pit, &lvCntTopCfg);
	if(lvErrorCode < 0) {
		printf("%s%s(%d) counter setting top error code:%d %s\n", \
		ATTR_ERR,__func__,__LINE__,lvErrorCode,ATTR_RST);
	} else {
		int lvErrorCode = counter_start(pit);
		if(lvErrorCode < 0) {
			printf("%s%s(%d) counter start error code:%d %s\n", \
			ATTR_ERR,__func__,__LINE__,lvErrorCode,ATTR_RST);
		}
	}
	*/
}

void pufs_otp_test(const struct device *pufs_otp)
{
	uint8_t slot_data[32] = {0};
	enum crypto_otp_lock lvLock = CRYPTO_OTP_RW; 
	uint16_t totalSlots = 0, bytesPerSlot = 0;

	if(otp_info(pufs_otp, &totalSlots, &bytesPerSlot) != 0) {
		printf("%s%s(%d) OTP_Info Failed%s\n", \
		ATTR_ERR,__func__,__LINE__,ATTR_RST);
	} else {
		printf("%s%s(%d) OTP_Info... Slots:%d Bytes Per Slot:%d%s\n", \
		ATTR_INF,__func__,__LINE__, totalSlots, bytesPerSlot, ATTR_RST);
	}
	
	if(otp_get_lock(pufs_otp, 12, &lvLock) != 0) {
		printf("%s%s(%d) otp_get_lock Failed%s\n", \
		ATTR_ERR,__func__,__LINE__,ATTR_RST);
	} else {
		printf("%s%s(%d) otp_lock: %d %s\n", \
		ATTR_INF,__func__,__LINE__,lvLock,ATTR_RST);
	}

	for(uint8_t i = 0; i < bytesPerSlot; i++) {
		slot_data[i] = (i+i)*2;
	}
	if(otp_write(pufs_otp, 12, slot_data, 18) != 0) {
		if(lvLock != CRYPTO_OTP_RW) {
			printf("%s%s(%d) otp_write Failed as Expected %s\n", \
			ATTR_RST,__func__,__LINE__,ATTR_RST);
		} else {
			printf("%s%s(%d) otp_write Failed unExpectedly %s\n", \
			ATTR_ERR,__func__,__LINE__,ATTR_RST);
		}
	} else {
		printf("%s%s(%d) otp_write Passed %s\n", \
		ATTR_RST,__func__,__LINE__,ATTR_RST);
		memset(slot_data, 0, bytesPerSlot);
		if(otp_read(pufs_otp, 12, slot_data, 18) != 0) {
			if(lvLock == CRYPTO_OTP_NA) {
				printf("%s%s(%d) otp_read Failed as Expected %s\n", \
				ATTR_RST,__func__,__LINE__,ATTR_RST);
			} else {
				printf("%s%s(%d) otp_read Failed unExpectedly %s\n", \
				ATTR_ERR,__func__,__LINE__,ATTR_RST);
			}
		} else {
			printf("%s",ATTR_INF);
			for(uint8_t i = 0; i < bytesPerSlot; i++) {
				printf("%d ", slot_data[i]);
			} printf("\n");
			printf("%s",ATTR_RST);
		}
	}
	if(otp_set_lock(pufs_otp, 12, 18, CRYPTO_OTP_RO) != 0) {
		printf("%s%s(%d) otp_set_lock Failed%s\n", \
		ATTR_ERR,__func__,__LINE__,ATTR_RST);
	} else {
		if(otp_get_lock(pufs_otp, 12, &lvLock) != 0) {
			printf("%s%s(%d) otp_get_lock Failed%s\n", \
			ATTR_ERR,__func__,__LINE__,ATTR_RST);
		} else {
			printf("%s%s(%d) otp_lock: %d %s\n", \
			ATTR_INF,__func__,__LINE__,lvLock,ATTR_RST);
		}
		for(uint8_t i = 0; i < bytesPerSlot; i++) {
		slot_data[i] = (i+i)*2;
		}
		if(otp_write(pufs_otp, 12, slot_data, 18) != 0) {
			if(lvLock != CRYPTO_OTP_RW) {
				printf("%s%s(%d) otp_write Failed as Expected %s\n", \
				ATTR_RST,__func__,__LINE__,ATTR_RST);
			} else {
				printf("%s%s(%d) otp_write Failed unExpectedly %s\n", \
				ATTR_ERR,__func__,__LINE__,ATTR_RST);
			}
		} else {
			printf("%s%s(%d) otp_write Passed UnExpectedly %s\n", \
			ATTR_ERR,__func__,__LINE__,ATTR_RST);
		}
	}
}

void pufs_hash_sg_test(const struct device *pufs)
{
	int status = 0;
	static uint8_t pufs_sample_data1[] = {
										 "My name is Junaid and I work at the Rapid Silicon and WorkFromH."
										 };
	static uint8_t pufs_sample_data2[] = {
										 "My name is Junaid and I work at the Rapid Silicon and WorkFromH."
										 };
	static uint8_t pufs_sample_data3[] = {
										 "123456789123456789"
										 };										 										 

	static uint8_t pufs_sample_data_sha256_out[32] = {0};
	static const uint8_t pufs_sample_data_sha256[] = {
		0xb2,0x69,0xec,0xc6,0x0f,0x13,0x37,0xf3,
		0xf3,0xf8,0x29,0xc1,0x0e,0xc5,0x83,0xa6,
		0x82,0x25,0x97,0x32,0x71,0x78,0xd1,0xb8,
		0xe5,0x95,0x05,0xd1,0xbe,0xe7,0x43,0xca
	};
	const uint8_t pufs_sample_data1_len = strlen(pufs_sample_data1);
	const uint8_t pufs_sample_data2_len = strlen(pufs_sample_data2);
	const uint8_t pufs_sample_data3_len = strlen(pufs_sample_data3);

	printf("%s%s(%d) Length of sample data to operate on:%d %s\n", \
	ATTR_INF,__func__,__LINE__,pufs_sample_data1_len+pufs_sample_data2_len+pufs_sample_data3_len,ATTR_RST);

	struct hash_ctx lvHashCtx = {
		.device = pufs,
		.drv_sessn_state = NULL,
		.flags = CAP_SYNC_OPS | CAP_SEPARATE_IO_BUFS,
		.started = true
	};		

	struct hash_pkt lvHashPkt1 = {0}, lvHashPkt2 = {0}, lvHashPkt3 = {0};

	lvHashPkt1.ctx = &lvHashCtx;
	lvHashPkt1.head = true;
	lvHashPkt1.tail = true;
	lvHashPkt1.in_buf = pufs_sample_data1;
	lvHashPkt1.in_hash = NULL;
	lvHashPkt1.in_hash_len = 0;
	lvHashPkt1.in_len = pufs_sample_data1_len;
	lvHashPkt1.next = &lvHashPkt2;
	lvHashPkt1.out_buf = pufs_sample_data_sha256_out;
	lvHashPkt1.out_len = 0;
	lvHashPkt1.prev_len = 0;

	lvHashPkt2.ctx = &lvHashCtx;
	lvHashPkt2.head = true;
	lvHashPkt2.tail = true;
	lvHashPkt2.in_buf = pufs_sample_data2;
	lvHashPkt2.in_hash = NULL;
	lvHashPkt2.in_hash_len = 0;
	lvHashPkt2.in_len = pufs_sample_data2_len;
	lvHashPkt2.next = &lvHashPkt3;
	lvHashPkt2.out_buf = pufs_sample_data_sha256_out;
	lvHashPkt2.out_len = 0;
	lvHashPkt2.prev_len = 0;

	lvHashPkt3.ctx = &lvHashCtx;
	lvHashPkt3.head = true;
	lvHashPkt3.tail = true;
	lvHashPkt3.in_buf = pufs_sample_data3;
	lvHashPkt3.in_hash = NULL;
	lvHashPkt3.in_hash_len = 0;
	lvHashPkt3.in_len = pufs_sample_data3_len;
	lvHashPkt3.next = NULL;
	lvHashPkt3.out_buf = pufs_sample_data_sha256_out;
	lvHashPkt3.out_len = 0;
	lvHashPkt3.prev_len = 0;

	printf("Pkt1.rd_addr:0x%08x Pkt2.rd_addr:0x%08x Pkt3.rd_addr:0x%08x\r\n", 
	(uint32_t)lvHashPkt1.in_buf, (uint32_t)lvHashPkt2.in_buf, (uint32_t)lvHashPkt3.in_buf);
	printf("Size struct hash pkt:%d\r\n", sizeof(struct hash_pkt));
	printf("data1_size:%d data2_size:%d data3_size:%d\r\n", \
	pufs_sample_data1_len, pufs_sample_data2_len, pufs_sample_data3_len);

	status = hash_begin_session(pufs, &lvHashCtx, CRYPTO_HASH_ALGO_SHA256);
	if(status != 0) {
		printf("%s%s(%d) hash_begin_session status = %d %s\n", \
			ATTR_ERR,__func__,__LINE__,status,ATTR_RST);
	} else {
		status = hash_compute(&lvHashCtx, &lvHashPkt1);
	}
	if(status != 0) {
		printf("%s%s(%d) hash_compute status = %d %s\n", \
			ATTR_ERR,__func__,__LINE__,status,ATTR_RST);
	} else {
		for(uint8_t i = 0; i < lvHashPkt1.out_len; i++) {			
			if(lvHashPkt1.out_buf[i] != pufs_sample_data_sha256[i]) {
				// printf("%s%s(%d) out_buf[%d]0x%02x != in_buf[%d]:0x%02x %s\n", 
				// ATTR_ERR,__func__,__LINE__,
				// i,lvHashPkt1.out_buf[i],i,pufs_sample_data_sha256[i],ATTR_RST);
				status = -EINVAL;
			} else {
				// printf("%s%s(%d) out_buf[%d]0x%02x == in_buf[%d]:0x%02x %s\n", 
				// ATTR_INF,__func__,__LINE__,
				// i,lvHashPkt1.out_buf[i],i,pufs_sample_data_sha256[i],ATTR_RST);
			}
		}
		if(status != 0) {
			printf("%s%s(%d) hash_comparison failed%s\n", \
			ATTR_ERR,__func__,__LINE__,ATTR_RST);
		} else {
			printf("%s%s(%d) hash_comparison Passed = %d %s\n", \
			ATTR_INF,__func__,__LINE__,status,ATTR_RST);			
		}
		status = hash_free_session(pufs, &lvHashCtx);
		if(status != 0) {
			printf("%s%s(%d) hash_free_session status = %d %s\n", \
			ATTR_ERR,__func__,__LINE__,status,ATTR_RST);
		} else {
			printf("%s%s(%d) hash_free_session Passed = %d %s\n", \
			ATTR_INF,__func__,__LINE__,status,ATTR_RST);
		}
	}
}

void pufs_hash_test(const struct device *pufs)
{
	int status = 0;
	static uint8_t *pufs_sample_data = "This is Rapid Silicon'z Zephyr Port";
	static uint8_t pufs_sample_data_sha256_out[32] = {0};
	static const uint8_t pufs_sample_data_sha256[] = {
		0xfa,0x71,0xc2,0x19,0xea,0x58,0x4d,0xac,
		0x36,0xd5,0x3e,0xca,0xe4,0x2a,0x8c,0x14,
		0x4b,0xc1,0xc0,0x03,0xfd,0x36,0x3f,0x71,
		0xd9,0x30,0x96,0xc4,0xaa,0x64,0xe0,0x4c
	};
	const uint8_t pufs_sample_data_len = strlen(pufs_sample_data);
	printf("%s%s(%d) Length of sample data to operate on:%d %s\n", \
									ATTR_INF,__func__,__LINE__,pufs_sample_data_len,ATTR_RST);
	struct hash_ctx lvHashCtx = {
		.device = pufs,
		.drv_sessn_state = NULL,
		.flags = CAP_SYNC_OPS | CAP_SEPARATE_IO_BUFS,
		.started = false
	};		

	struct hash_pkt lvHashPkt = {
		.ctx = &lvHashCtx,
		.head = true,
		.tail = true,
		.in_buf = pufs_sample_data,
		.in_hash = NULL,
		.in_len = pufs_sample_data_len,
		.next = NULL,
		.out_buf = pufs_sample_data_sha256_out,
		.prev_len = 0,
		.out_len = 0
	};	
	status = hash_begin_session(pufs, &lvHashCtx, CRYPTO_HASH_ALGO_SHA256);
	if(status != 0) {
		printf("%s%s(%d) hash_begin_session status = %d %s\n", \
			ATTR_ERR,__func__,__LINE__,status,ATTR_RST);
	} else {
		status = hash_compute(&lvHashCtx, &lvHashPkt);
	}
	if(status != 0) {
		printf("%s%s(%d) hash_compute status = %d %s\n", \
			ATTR_ERR,__func__,__LINE__,status,ATTR_RST);
	} else {
		for(uint8_t i = 0; i < lvHashPkt.out_len; i++) {			
			if(lvHashPkt.out_buf[i] != pufs_sample_data_sha256[i]) {
				// printf("%s%s(%d) out_buf[%d]0x%02x != in_buf[%d]:0x%02x %s\n",
				// ATTR_ERR,__func__,__LINE__,
				// i,lvHashPkt.out_buf[i],i,pufs_sample_data_sha256[i],ATTR_RST);
				status = -EINVAL;
			} else {
				// printf("%s%s(%d) out_buf[%d]0x%02x == in_buf[%d]:0x%02x %s\n",
				// ATTR_INF,__func__,__LINE__,
				// i,lvHashPkt.out_buf[i],i,pufs_sample_data_sha256[i],ATTR_RST);
			}
		}
		if(status != 0) {
			printf("%s%s(%d) hash_comparison failed%s\n", \
			ATTR_ERR,__func__,__LINE__,ATTR_RST);
		} else {
			printf("%s%s(%d) hash_comparison Passed = %d %s\n", \
			ATTR_INF,__func__,__LINE__,status,ATTR_RST);			
		}
		status = hash_free_session(pufs, &lvHashCtx);
		if(status != 0) {
			printf("%s%s(%d) hash_free_session status = %d %s\n", \
			ATTR_ERR,__func__,__LINE__,status,ATTR_RST);
		} else {
			printf("%s%s(%d) hash_free_session Passed = %d %s\n", \
			ATTR_INF,__func__,__LINE__,status,ATTR_RST);
		}
	}
}

void pufs_decryption_test(const struct device *pufs)
{
	/*
	Plain Text: This is Junaid Aslam from Rapid Silicon doing Decryption Test...
	IV: 0xaa,0x34,0x55,0x32,0xe9,0x58,0xaa,0xf3,0xc0,0xde,0x20,0xa6,0xa9,0x21,0x44,0xe8
	Key: 
		0x30,0x13,0x53,0x33,0x44,0x12,0xff,0x22,0x31,0x53,0xbb,0x13,0x07,0x19,0x84,0x0d,
		0xd5,0x3c,0x3f,0xc0,0x09,0x83,0x11,0x35,0x30,0x2d,0xdc,0xc0,0x0b,0xb9,0x4e,0x30

	Cipher Text: 
				0xd0,0x07,0x29,0x7b,0x56,0x2c,0x2c,0xa7,
				0x43,0x24,0x68,0xd8,0x58,0xb4,0xc1,0x9f,
				0x7c,0x05,0x7c,0x98,0x03,0x28,0x19,0x9d,
				0x93,0xff,0x85,0xd7,0xdb,0x8f,0xf1,0x07,
				0x18,0xa5,0x1f,0x99,0x04,0x73,0x84,0x4f,
				0x4f,0x79,0xba,0x2e,0x70,0x82,0xb1,0x39,
				0x5d,0x3f,0x3e,0xf8,0x44,0x81,0xf6,0x89,
				0x17,0x3a,0x29,0x43,0x0a,0x41,0x24,0xff,
				0x57,0xa8,0xbb,0x0b,0x0e,0xcf,0xb3,0xed,
				0x8e,0x1d,0x49,0x03,0x86,0x77,0x61,0x68,
				0x59,0x7c,0xf0,0x61,0x98,0x11,0x1b,0xe5,
				0xe3,0x95,0x6d,0x81,0x49,0xdc,0xa0,0x4b,
				0x97,0xb4,0xe6,0x40,0xd5,0x26,0xb6,0xb7,
				0xd8,0xab,0x61,0xc1,0xa7,0x5a,0xdc,0x5e,
				0xa9,0x25,0x6f,0xcd,0x25,0xb6,0xd0,0x42,
				0xa1,0xd8,0xd5,0xdc,0xcf,0xc6,0xaa,0xdf
	*/
	int lvStatus = 0;

	struct cipher_ctx lvCipherCtx = {0};
	struct cipher_pkt lvCipherPkt = {0};

	uint8_t enc32_key[] = {
									0x30,0x13,0x53,0x33,0x44,0x12,0xff,0x22,
									0x31,0x53,0xbb,0x13,0x07,0x19,0x84,0x0d,
									0xd5,0x3c,0x3f,0xc0,0x09,0x83,0x11,0x35,
									0x30,0x2d,0xdc,0xc0,0x0b,0xb9,0x4e,0x30
								};
	uint8_t iv[] = {
							0xaa,0x34,0x55,0x32,0xe9,0x58,0xaa,0xf3,
							0xc0,0xde,0x20,0xa6,0xa9,0x21,0x44,0xe8
						 };

	uint8_t cipher_text[] = {
									0xd0,0x07,0x29,0x7b,0x56,0x2c,0x2c,0xa7,
									0x43,0x24,0x68,0xd8,0x58,0xb4,0xc1,0x9f,
									0x7c,0x05,0x7c,0x98,0x03,0x28,0x19,0x9d,
									0x93,0xff,0x85,0xd7,0xdb,0x8f,0xf1,0x07,
									0x18,0xa5,0x1f,0x99,0x04,0x73,0x84,0x4f,
									0x4f,0x79,0xba,0x2e,0x70,0x82,0xb1,0x39,
									0x5d,0x3f,0x3e,0xf8,0x44,0x81,0xf6,0x89,
									0x17,0x3a,0x29,0x43,0x0a,0x41,0x24,0xff,
									0x57,0xa8,0xbb,0x0b,0x0e,0xcf,0xb3,0xed,
									0x8e,0x1d,0x49,0x03,0x86,0x77,0x61,0x68,
									0x59,0x7c,0xf0,0x61,0x98,0x11,0x1b,0xe5,
									0xe3,0x95,0x6d,0x81,0x49,0xdc,0xa0,0x4b,
									0x97,0xb4,0xe6,0x40,0xd5,0x26,0xb6,0xb7,
									0xd8,0xab,0x61,0xc1,0xa7,0x5a,0xdc,0x5e,
									0xa9,0x25,0x6f,0xcd,0x25,0xb6,0xd0,0x42,
									0xa1,0xd8,0xd5,0xdc,0xcf,0xc6,0xaa,0xdf
								  };

	uint8_t *plain_text = "Plain Text: This is Junaid Aslam from Rapid Silicon doing Decryption Test...\n";
	uint8_t decrypted_text[256] = {0};

	lvCipherCtx.app_sessn_state = NULL;
	lvCipherCtx.device = pufs;
	lvCipherCtx.flags = ( 
							CAP_RAW_KEY | 
							CAP_SEPARATE_IO_BUFS | 
							CAP_SYNC_OPS | 
							CAP_NO_IV_PREFIX | 
							CAP_NO_ENCRYPTION 
						);						
	lvCipherCtx.key.bit_stream =  enc32_key;
	lvCipherCtx.key_source = CRYPTO_KEY_SW;
	lvCipherCtx.keylen = 32;
	lvCipherCtx.mode_params.ctr_info.ctr_len = 16;
	lvCipherCtx.mode_params.ctr_info.readback_ctr = false;
	lvCipherCtx.ops.cipher_mode = CRYPTO_CIPHER_MODE_CTR;

	lvCipherPkt.auto_increment = true;
	lvCipherPkt.ctx = &lvCipherCtx;
	lvCipherPkt.in_buf = cipher_text;
	lvCipherPkt.in_len = (sizeof(cipher_text)/sizeof(cipher_text[0]));
	lvCipherPkt.out_buf = decrypted_text;
	lvCipherPkt.prev_len = 0;

	printf("%s Decrypting %d bytes %s\n", ATTR_INF, lvCipherPkt.in_len, ATTR_RST);

	lvStatus = cipher_begin_session(
									pufs, 
									&lvCipherCtx, 
									CRYPTO_CIPHER_ALGO_AES, 
									CRYPTO_CIPHER_MODE_CTR, 
									CRYPTO_CIPHER_OP_DECRYPT
								   );
	if(lvStatus != 0) {
		printf("%s cipher_begin_session Failed! %s\n", ATTR_ERR, ATTR_RST);
	} else {
		printf("%s cipher_begin_session Success! %s\n", ATTR_INF, ATTR_RST);		
	}

	lvStatus = 	cipher_ctr_op(&lvCipherCtx, &lvCipherPkt, (uint8_t*)iv);		
	if(lvStatus != 0) {
		printf("%s cipher_ctr_op Failed! Status:%d %s\n", ATTR_ERR, lvStatus, ATTR_RST);
	} else {
		printf("%s cipher_ctr_op Success! %s\n", ATTR_INF, ATTR_RST);
		printf("%s Original: %s%s", ATTR_HIL,plain_text,ATTR_RST);
		printf("%s Result:   %s%s", ATTR_HIL,lvCipherPkt.out_buf,ATTR_RST);
		bool comparison_ok = true;
		for(int i = 0; i < strlen(lvCipherPkt.out_buf); i++) {
			if(lvCipherPkt.out_buf[i] != plain_text[i]) {
				// printf("Orig[%d]:0x%02x(%c) != Res[%d]:0x%02x(%c)\n", i, plain_text[i], plain_text[i], i, lvCipherPkt.out_buf[i], lvCipherPkt.out_buf[i]);
				comparison_ok = false; break;
			} 
			// else {
				// printf("Orig[%d]:0x%02x(%c) == Res[%d]:0x%02x(%c)\n", i, plain_text[i], plain_text[i], i, lvCipherPkt.out_buf[i], lvCipherPkt.out_buf[i]);
			// }
		}
		if(!comparison_ok) {
			printf("%s cipher decryption Failed! %s\n", ATTR_ERR, ATTR_RST);
		} else {
			printf("%s cipher decryption Passed! %s\n", ATTR_INF, ATTR_RST);
		}
	}

	lvStatus = cipher_free_session(pufs, &lvCipherCtx);
	if(lvStatus != 0) {
		printf("%s cipher_free_session Failed! %s\n", ATTR_ERR, ATTR_RST);
	} else {
		printf("%s cipher_free_session Success! %s\n", ATTR_INF, ATTR_RST);
	}	
}

void pufs_rsa2048_verify_test(const struct device *dev)
{

}

void pufs_ecdsa256_verify_test(const struct device *dev)
{

}

int main(void)
{
	int Cnt = 0;
	uint8_t chip_id = 0, vendor_id = 0;

	printf(
		"%s SPI_(%d)_IRQ_Reg_Val:[0x%08x]; DMA_(%d)_IRQ_Reg_Val:[0x%08x]\n", ATTR_RST, \
		IRQ_ID_SPI, scu_get_irq_reg_val(IRQ_ID_SPI), \
		IRQ_ID_SYSTEM_DMA, scu_get_irq_reg_val(IRQ_ID_SYSTEM_DMA)
	);

	soc_get_id(&chip_id, &vendor_id);

	int errorcode = 0;
	struct sensor_value lvTemp = {0}, lvVolt = {0};
	const struct device *pvt = DEVICE_DT_GET(DT_NODELABEL(pvt0));
	const struct device *spi = DEVICE_DT_GET(DT_NODELABEL(spi0));
	const struct device *flash = DEVICE_DT_GET(DT_NODELABEL(m25p32));
	const struct device *dma = DEVICE_DT_GET(DT_NODELABEL(dma0));
	const struct device *pit = DEVICE_DT_GET(DT_NODELABEL(pit0));
	const struct device *pufs = DEVICE_DT_GET(DT_NODELABEL(pufs));
	const struct device *pufs_otp = DEVICE_DT_GET(DT_NODELABEL(pufs_otp));

	if((pufs == NULL) || (!device_is_ready(pufs))) {
		printf("%s pufs has status disabled or driver is not initialized...%s\n", ATTR_ERR, ATTR_RST);
	} else {
		printf("%s pufs Object is Created %s\n", ATTR_INF, ATTR_RST);
		// pufs_rsa2048_verify_test(pufs); while(true); // TODO Remove while
		pufs_decryption_test(pufs);
		pufs_hash_test(pufs);
		pufs_hash_sg_test(pufs);
	}

	if((pufs_otp == NULL) || (!device_is_ready(pufs_otp))) {
		printf("%s pufs_otp has status disabled or driver is not initialized...%s\n", ATTR_ERR, ATTR_RST);
	} else {
		printf("%s pufs_otp Object is Created %s\n", ATTR_INF, ATTR_RST);
		pufs_otp_test(pufs_otp);
	}

	if((pvt == NULL) || (!device_is_ready(pvt))) {
		printf("%s pvt has status disabled or driver is not initialized...%s\n", ATTR_ERR, ATTR_RST);
	} else {
		printf("%s pvt Object is Created %s\n", ATTR_INF, ATTR_RST);
		errorcode = sensor_channel_get(pvt, SENSOR_CHAN_DIE_TEMP, &lvTemp);
		if(errorcode == 0) {
			printf("%s Error fetching temperature value. Error code:%u%s\n", ATTR_ERR, errorcode,ATTR_RST);
		}
		errorcode = sensor_channel_get(pvt, SENSOR_CHAN_VOLTAGE, &lvVolt);
		if(errorcode == 0) {
			printf("%s Error fetching Voltage value. Error code:%u%s\n", ATTR_ERR, errorcode,ATTR_RST);
		}
		printf("%s Die Temperature:%d Voltage:%d%s\n", ATTR_INF, lvTemp.val1, lvVolt.val1,ATTR_RST);
	}

	if(spi == NULL) {
		printf("%s spi has status disabled...%s\n", ATTR_ERR,ATTR_RST);
	} else {			
		printf("%s spi Object is Created. Test Via DMA\n", ATTR_INF);				
		if(flash == NULL) {
			printf("%s flash has status disabled or not initialized properly...%s\n", ATTR_ERR,ATTR_RST);
		} else {
			printf("%s flash Object is Created. Initialize to Test Via DMA%s\n", ATTR_INF,ATTR_RST);						
			Flash_Test(flash, dma, 0x1000, 0, 20);
		}
	}

	if((pit == NULL) || !device_is_ready(pit)) {
		printf("%s pit has status disabled or driver is not initialized...%s\n", ATTR_ERR,ATTR_RST);
	} else {
		CounterTest(pit);
	}

	while(true) {		
		printf(
				"%s%d - %s [CHIP_ID:0x%02x VENDOR_ID:0x%02x] Build[Date:%s Time:%s]\r", 
				ATTR_RST, Cnt++, 
				CONFIG_BOARD_TARGET, 
				chip_id, vendor_id,
				__DATE__, __TIME__
			  );
		k_msleep(1000);
	}

	return 0;
}
