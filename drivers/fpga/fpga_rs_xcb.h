/*
 * Copyright (c) 2024 Rapid Silicon
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef ZEPHYR_DRIVERS_FPGA_RS_XCB_H_
#define ZEPHYR_DRIVERS_FPGA_RS_XCB_H_

#include <zephyr/drivers/fpga.h>

#define RS_PCB_BITSTR_HEADER_SIZE 7  // words
#define RS_ICB_PACKET_HEADER_SIZE 5  // words
#define RS_FCB_BITSTR_HEADER_SIZE 4  // words

#define xCB_DEBUG 0
#define xCB_BITS_IN_A_BYTE 8
#define xCB_BITS_IN_A_WORD 32
#define xCB_BYTES_IN_A_WORD 4

enum xCB_ACTIONS { xCB_DISABLE = 0, xCB_ENABLE = 1 };

enum xCB_BIT_VALUE { xCB_RESET = 0, xCB_SET = 1 };

/* *********************************************
 * ERROR_CODE enum contains list of descriptive
 * error codes to be used in Configuration
 * controller drivers.
 * ********************************************/
enum xCB_ERROR_CODE {
  xCB_SUCCESS = 0,
  xCB_ERROR = 1,
  xCB_ERROR_NULL_POINTER = 2,
  xCB_ERROR_WRITE_ERROR = 3,
  xCB_ERROR_READ_ERROR = 4,
  xCB_ERROR_INVALID_BIT_VALUE = 5,
  xCB_ERROR_INVALID_DATA = 6,
  xCB_ERROR_DATA_CORRUPTED = 7,
  xCB_ERROR_CHECKSUM_MATCH_FAILED = 8,
  xCB_ERROR_OUT_OF_LIMIT = 9,
  xCB_ERROR_TIMEOUT = 10,
  xCB_ERROR_FCB_CONF_FAILED = 11,
  xCB_ERROR_FCB_BITSTREAM_TX_FAILED = 12,
  xCB_ERROR_FCB_BITSTREAM_RX_FAILED = 13,
  xCB_ERROR_ICB_CONF_FAILED = 14,
  xCB_ERROR_ICB_TRANSFER_FAILED = 15,
  xCB_ERROR_PCB_CONF_FAILED = 16,
  xCB_ERROR_PCB_BITSTREAM_TX_FAILED = 17,
  xCB_ERROR_PCB_BITSTREAM_RX_FAILED = 18,
  xCB_ERROR_INVALID_DATA_LENGTH = 19,
  xCB_ERROR_DATA_MISMATCH = 20,
  xCB_ERROR_UNEXPECTED_VALUE = 21
};

/* *********************************************
 * The enum can be used to specify read or write
 * ********************************************/
enum TRANSFER_TYPE {
  TRANSFER_TYPE_TX = FPGA_TRANSFER_TYPE_TX,
  TRANSFER_TYPE_RX = FPGA_TRANSFER_TYPE_RX,
  TRANSFER_TYPE_UNDEFINED = FPGA_TRANSFER_TYPE_UNDEFINED
};

/* **************************************************
 * @brief: This function returns textual description
 * of the provided error code.
 *
 * @param [in]: enum xCB_ERROR_CODE
 * @return:     descriptive error text
 * **************************************************/
const char *Err_to_Str(enum xCB_ERROR_CODE inErrorCode);

/* *********************************************
 *              Error Printing Macro
 * ********************************************/
#define PRINT_ERROR(lvErrorCode) \
  LOG_ERR("%s(%d) xCB_Error_Code:%d Msg:%s\r\n\n", __func__, __LINE__, \
          lvErrorCode, Err_to_Str(lvErrorCode));

#endif
