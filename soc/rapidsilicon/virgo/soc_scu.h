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

#define IRQ_MASK_OFFSET 0
#define IRQ_MAP_OFFSET  16

#define IRQ_DISABLE     0
#define IRQ_ENABLE      1

#define IRQ_MAP_TO_BCPU 1
#define IRQ_MAP_TO_RSVD 2
#define IRQ_MAP_TO_FPGA 4

void soc_irq_map(uint8_t IRQn, uint8_t SubSystem);
void soc_irq_unmap(uint8_t IRQn);
void soc_irq_enable(uint8_t IRQn);
void soc_irq_disable(uint8_t IRQn);