/**
 * raspberry-pi-gpio
 *
 * Copyright (c) 2016 T. Ngtk
 *
 * Released under the MIT License.
 * https://github.com/ngtkt0909/raspberry-pi-gpio/blob/master/LICENSE
 */

#if !defined(__RPI_I2C_H__)
#define __RPI_I2C_H__		/* include guard */

#include <linux/i2c-dev.h>
#include <stdint.h>

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
