/**
 * @file		rpi_gpio.h
 * @brief		GPIO Library Header
 *
 * @author		T. Ngtk
 * @copyright	Copyright (c) 2016 T. Ngtk
 *
 * @par License
 *	Released under the MIT License.<BR>
 *	https://github.com/ngtkt0909/raspberry-pi-gpio/blob/master/LICENSE
 */

#ifndef __RPI_GPIO_H__
#define __RPI_GPIO_H__		/**< include guard */

#include <stdint.h>

/*------------------------------------------------------------------------------
	Prototype Declaration
------------------------------------------------------------------------------*/
int8_t rpiGpioOpenIn(uint8_t pin);
int8_t rpiGpioOpenOut(uint8_t pin);
int8_t rpiGpioClose(uint8_t pin);
int8_t rpiGpioSet(uint8_t pin);
int8_t rpiGpioClr(uint8_t pin);
int8_t rpiGpioGet(uint8_t pin, int32_t *val);

#endif /* __RPI_GPIO_H__ */
