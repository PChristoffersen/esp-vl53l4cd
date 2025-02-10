/**
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


#include <inttypes.h>
#include "vl53l4cd_platform.h"


#include <freertos/FreeRTOS.h>
#include <esp_err.h>
#include <esp_check.h>
#include <esp_log.h>
#include <driver/i2c_master.h>

static const char TAG[] = "vl53l4cd";


uint8_t VL53L4CD_RdDWord(VL53L5CX_Dev_t dev, uint16_t RegisterAdress, uint32_t *value)
{
	esp_err_t ret;
	uint8_t buffer[2];
	
	ESP_LOGV(TAG, "VL53L4CD_RdDWord <%04" PRIx16 "  %p", RegisterAdress, value);

	buffer[0] = RegisterAdress>>8;
	buffer[1] = RegisterAdress & 0xFF;

	ret = i2c_master_transmit_receive(dev->handle, buffer, sizeof(buffer), (uint8_t*)value, sizeof(*value), VL53L4CD_I2C_TIMEOUT_MS);
	if (ret<0)
		return -ret;
	return ret;
}

uint8_t VL53L4CD_RdWord(VL53L5CX_Dev_t dev, uint16_t RegisterAdress, uint16_t *value)
{
	esp_err_t ret;
	uint8_t buffer[2];
	
	ESP_LOGV(TAG, "VL53L4CD_RdWord <%04" PRIx16 "  %p", RegisterAdress, value);

	buffer[0] = RegisterAdress>>8;
	buffer[1] = RegisterAdress & 0xFF;

	ret = i2c_master_transmit_receive(dev->handle, buffer, sizeof(buffer), (uint8_t*)value, sizeof(*value), VL53L4CD_I2C_TIMEOUT_MS);
	if (ret<0)
		return -ret;
	return ret;
}

uint8_t VL53L4CD_RdByte(VL53L5CX_Dev_t dev, uint16_t RegisterAdress, uint8_t *value)
{
	esp_err_t ret;
	uint8_t buffer[2];
	
	ESP_LOGV(TAG, "VL53L4CD_RdByte <%04" PRIx16 "  %p", RegisterAdress, value);

	buffer[0] = RegisterAdress>>8;
	buffer[1] = RegisterAdress & 0xFF;

	ret = i2c_master_transmit_receive(dev->handle, buffer, sizeof(buffer), value, sizeof(*value), VL53L4CD_I2C_TIMEOUT_MS);
	if (ret<0)
		return -ret;
	return ret;
}

uint8_t VL53L4CD_WrByte(VL53L5CX_Dev_t dev, uint16_t RegisterAdress, uint8_t value)
{
	esp_err_t ret;
	uint8_t buffer[3];

	ESP_LOGV(TAG, "VL53L4CD_WrByte >%04" PRIx16 "  %02x", RegisterAdress, value);

	buffer[0] = RegisterAdress>>8;
	buffer[1] = RegisterAdress & 0xFF;
	buffer[2] = value;

	ret = i2c_master_transmit(dev->handle, buffer, sizeof(buffer), VL53L4CD_I2C_TIMEOUT_MS);
	if (ret<0)
		return -ret;
	return ret;
}

uint8_t VL53L4CD_WrWord(VL53L5CX_Dev_t dev, uint16_t RegisterAdress, uint16_t value)
{
	esp_err_t ret;
	uint8_t buffer[4];

	ESP_LOGV(TAG, "VL53L4CD_WrWord >%04" PRIx16 "  %04" PRIx16, RegisterAdress, value);

	buffer[0] = RegisterAdress>>8;
	buffer[1] = RegisterAdress & 0xFF;
	buffer[2] = value>>8;
	buffer[3] = value & 0xFF;

	ret = i2c_master_transmit(dev->handle, buffer, sizeof(buffer), VL53L4CD_I2C_TIMEOUT_MS);
	if (ret<0)
		return -ret;
	return ret;
}

uint8_t VL53L4CD_WrDWord(VL53L5CX_Dev_t dev, uint16_t RegisterAdress, uint32_t value)
{
	esp_err_t ret;
	uint8_t buffer[6];

	ESP_LOGV(TAG, "VL53L4CD_WrDWord >%04" PRIx16 "  %08" PRIx32, RegisterAdress, value);

	buffer[0] = RegisterAdress>>8;
	buffer[1] = RegisterAdress & 0xFF;
	buffer[2] = (value>>24) & 0xFF;
	buffer[3] = (value>>16) & 0xFF;
	buffer[4] = (value>>8) & 0xFF;
	buffer[5] = value & 0xFF;

	ret = i2c_master_transmit(dev->handle, buffer, sizeof(buffer), VL53L4CD_I2C_TIMEOUT_MS);
	if (ret<0)
		return -ret;
	return ret;
}

uint8_t VL53L4CD_WaitMs(VL53L5CX_Dev_t dev, uint32_t TimeMs)
{
	vTaskDelay(pdMS_TO_TICKS(TimeMs));
	return 0;
}
