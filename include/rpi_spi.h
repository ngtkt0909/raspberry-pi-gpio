/**
 * @file		rpi_spi.h
 * @brief		SPI Library Header
 *
 * @author		T. Ngtk
 * @copyright	Copyright (c) 2016 T. Ngtk
 *
 * @par License
 *	Released under the MIT License.<BR>
 *	https://github.com/ngtkt0909/raspberry-pi-gpio/blob/master/LICENSE
 */

#ifndef __RPI_SPI_H__
#define __RPI_SPI_H__		/**< include guard */

#include <stdint.h>
#include <linux/spi/spidev.h>
#include "rpi_common.h"

/*------------------------------------------------------------------------------
	Defined Macros
------------------------------------------------------------------------------*/
#define D_SPI_CS_POS_LOGIC		(1U)				/**< CS Positive Logic */
#define D_SPI_CS_NEG_LOGIC		(0U)				/**< CS Negative Logic */

/*------------------------------------------------------------------------------
	Prototype Declaration
------------------------------------------------------------------------------*/
int8_t rpiSpiOpen(uint8_t *dev_name);
int8_t rpiSpiClose();
int8_t rpiSpiTransfer(uint8_t *tx_data, uint8_t *rx_data, uint32_t size);
int8_t rpiSpiSetMode(uint8_t mode);
int8_t rpiSpiSetSpeed(uint32_t speed);
int8_t rpiSpiSetDelay(uint16_t delay);
int8_t rpiSpiSetBitsPerWord(uint8_t len);
int8_t rpiSpiSetCsPolarity(uint8_t pol);

#endif /* __RPI_SPI_H__ */
