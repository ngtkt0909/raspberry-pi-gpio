/**
 * @file		rpi_i2c.h
 * @brief		I2C Library Header
 *
 * @author		T. Ngtk
 * @copyright	Copyright (c) 2016 T. Ngtk
 *
 * @par License
 *	Released under the MIT License.<BR>
 *	https://github.com/ngtkt0909/raspberry-pi-gpio/blob/master/LICENSE
 */

#ifndef __RPI_I2C_H__
#define __RPI_I2C_H__		/**< include guard */

#include <stdint.h>
#include <linux/i2c-dev.h>
#include "rpi_common.h"

/*------------------------------------------------------------------------------
	Prototype Declaration
------------------------------------------------------------------------------*/
int8_t rpiI2cOpen(uint8_t *dev_path);
int8_t rpiI2cClose();
int8_t rpiI2cSetSlave(uint8_t slave_addr);
int8_t rpiI2cWrite(uint8_t cmd, uint8_t data);
int8_t rpiI2cRead(uint8_t cmd, uint8_t *data);
int8_t rpiI2cWriteBlock(uint8_t cmd, uint8_t *buf, uint32_t size);
int8_t rpiI2cReadBlock(uint8_t cmd, uint8_t *buf, uint32_t size);

#endif /* __RPI_I2C_H__ */
