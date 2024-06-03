/* DTI Process, Voltage and Thermal Sensor Driver
 *
 * Copyright (c) 2024 Rapid Silicon
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#define DT_DRV_COMPAT dti_pvt

#include <zephyr/init.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "dti_pvt.h"

LOG_MODULE_REGISTER(DTI_PVT, CONFIG_SENSOR_LOG_LEVEL);

#define DTI_PVT_INIT(inst) 			\
{									\
									\
}

DT_INST_FOREACH_STATUS_OKAY(DTI_PVT_INIT)