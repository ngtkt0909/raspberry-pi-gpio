/**
 * raspberry-pi-gpio
 *
 * Copyright (c) 2016 T. Ngtk
 *
 * Released under the MIT License.
 * https://github.com/ngtkt0909/raspberry-pi-gpio/blob/master/LICENSE
 */

#ifndef __RPI_CLKGEN_H__
#define __RPI_CLKGEN_H__		/* include guard */

#include <stdint.h>
#include "rpi_gpio.h"

/*------------------------------------------------------------------------------
	Prototype Declaration
------------------------------------------------------------------------------*/
void rpiClkgenEnable(uint8_t pin, uint32_t mash, uint32_t src, uint32_t divi, uint32_t divf);
void rpiClkgenDisable(uint8_t pin);

#endif /* __RPI_CLKGEN_H__ */
