/*
 * Copyright (c) 2024 Rapid Silicon.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * VIRGO SoC specific interrupt map and mask functions
 */

#include <zephyr/kernel.h>

#define SOC_IRQ_MASK_OFFSET 0
#define SOC_IRQ_MAP_OFFSET  16

#define SOC_IRQ_DISABLE     0x0
#define SOC_IRQ_ENABLE      0x1

#define SOC_IRQ_MAP_TO_NONE 0x0
#define SOC_IRQ_MAP_TO_BCPU 0x1
#define SOC_IRQ_MAP_TO_RSVD 0x2
#define SOC_IRQ_MAP_TO_FPGA 0x4
#define SOC_IRQ_MAP_MASK    0x7

enum map_mask_control_irq_id {
  IRQ_ID_BCPU_WDT = -1,
  IRQ_ID_GPT = 0,
  IRQ_ID_RESERVED1 = 1,
  IRQ_ID_RESERVED2 = 2,
  IRQ_ID_UART0 = 3,
  IRQ_ID_RESERVED4 = 4,
  IRQ_ID_SPI = 5,
  IRQ_ID_I2C = 6,
  IRQ_ID_GPIO = 7,
  IRQ_ID_SYSTEM_DMA = 8,
  IRQ_ID_RESERVED9 = 9,
  // Mailboxes
  IRQ_ID_RESERVED10 = 10,
  IRQ_ID_RESERVED11 = 11,
  IRQ_ID_RESERVED12 = 12,
  IRQ_ID_RESERVED13 = 13,
  // Encryption engine
  IRQ_ID_PUFCC = 14,
  // FPGA GENERATED
  IRQ_ID_FPGAGEN0 = 15,   // Not mapped to FPGA
  IRQ_ID_FPGAGEN1 = 16,   // Not mapped to FPGA
  IRQ_ID_FPGAGEN2 = 17,   // Not mapped to FPGA
  IRQ_ID_FPGAGEN3 = 18,   // Not mapped to FPGA
  IRQ_ID_FPGAGEN4 = 19,   // Not mapped to FPGA
  IRQ_ID_FPGAGEN5 = 20,   // Not mapped to FPGA
  IRQ_ID_FPGAGEN6 = 21,   // Not mapped to FPGA
  IRQ_ID_FPGAGEN7 = 22,   // Not mapped to FPGA
  IRQ_ID_FPGAGEN8 = 23,   // Not mapped to FPGA
  IRQ_ID_FPGAGEN9 = 24,   // Not mapped to FPGA
  IRQ_ID_FPGAGEN10 = 25,  // Not mapped to FPGA
  IRQ_ID_FPGAGEN11 = 26,  // Not mapped to FPGA
  IRQ_ID_FPGAGEN12 = 27,  // Not mapped to FPGA
  IRQ_ID_FPGAGEN13 = 28,  // Not mapped to FPGA
  IRQ_ID_FPGAGEN14 = 29,  // Not mapped to FPGA
  IRQ_ID_FPGAGEN15 = 30,  // Not mapped to FPGA
};

#define SOC_IRQ_CTRL_REG_OFFSET IRQ_ID_BCPU_WDT*(-1)

void soc_irq_map(enum map_mask_control_irq_id IRQn, uint8_t SubSystem);
void soc_irq_unmap(enum map_mask_control_irq_id IRQn);
void soc_irq_enable(enum map_mask_control_irq_id IRQn);
void soc_irq_disable(enum map_mask_control_irq_id IRQn);

#define SOC_CHIP_ID_OFFSET      16
#define SOC_CHIP_ID_MASK        0x00FF0000

#define SOC_VENDOR_ID_OFFSET    24
#define SOC_VENDOR_ID_MASK      0xFF000000

void soc_get_id(uint8_t *chip_id, uint8_t *vendor_id);
