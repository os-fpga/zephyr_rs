/*
 * Copyright (c) 2024 Rapid Silicon
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "fpga_rs_xcb.h"
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(rs_fpga_ofe);

/* **************************************************
 * @brief: This function returns textual description
 * of the provided error code.
 *
 * @param [in]: enum xCB_ERROR_CODE
 * @return:     descriptive error text
 * **************************************************/
const char *Err_to_Str(enum xCB_ERROR_CODE inErrorCode) {
  switch (inErrorCode) {
    case (xCB_SUCCESS): {
      return "SUCCESS";
    } break;
    case (xCB_ERROR): {
      return "ERROR";
    } break;
    case (xCB_ERROR_NULL_POINTER): {
      return "NULL_POINTER";
    } break;
    case (xCB_ERROR_WRITE_ERROR): {
      return "WRITE_ERROR";
    } break;
    case (xCB_ERROR_READ_ERROR): {
      return "READ_ERROR";
    } break;
    case (xCB_ERROR_INVALID_BIT_VALUE): {
      return "INVALID_BIT_VALUE";
    } break;
    case (xCB_ERROR_INVALID_DATA): {
      return "INVALID_DATA";
    } break;
    case (xCB_ERROR_DATA_CORRUPTED): {
      return "DATA_CORRUPTED";
    } break;
    case (xCB_ERROR_CHECKSUM_MATCH_FAILED): {
      return "CHECKSUM_MISMATCH";
    } break;
    case (xCB_ERROR_OUT_OF_LIMIT): {
      return "OUT_OF_LIMIT";
    } break;
    case (xCB_ERROR_TIMEOUT): {
      return "TIMEOUT";
    } break;
    case (xCB_ERROR_FCB_CONF_FAILED): {
      return "FCB_CONF_FAILED";
    } break;
    case (xCB_ERROR_FCB_BITSTREAM_TX_FAILED): {
      return "FCB_TX_FAILED";
    } break;
    case (xCB_ERROR_FCB_BITSTREAM_RX_FAILED): {
      return "FCB_RX_FAILED";
    } break;
    case (xCB_ERROR_ICB_CONF_FAILED): {
      return "ICB_CONF_FAILED";
    } break;
    case (xCB_ERROR_ICB_TRANSFER_FAILED): {
      return "ICB_TRANSFER_FAILED";
    } break;
    case (xCB_ERROR_PCB_CONF_FAILED): {
      return "PCB_CONF_FAILED";
    } break;
    case (xCB_ERROR_PCB_BITSTREAM_TX_FAILED): {
      return "PCB_TX_FAILED";
    } break;
    case (xCB_ERROR_PCB_BITSTREAM_RX_FAILED): {
      return "PCB_RX_FAILED";
    } break;
    case (xCB_ERROR_INVALID_DATA_LENGTH): {
      return "INVALID_DATA_LENGTH";
    } break;
    case (xCB_ERROR_DATA_MISMATCH): {
      return "DATA_MISMATCH";
    } break;
    case (xCB_ERROR_UNEXPECTED_VALUE): {
      return "UNEXPECTED_VALUE";
    } break;
  }
  return "Unknown_Error_Code";
}

