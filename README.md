# raspberry-pi-gpio

## Overview
Library for Raspberry Pi GPIO.

* SPI Library (rpi_spi.c, rpi_spi.h)

## SPI Library
### Usage
Code example to use SPI library is shown below (error handling omitted):

````C
#include "rpi_spi.h"

#define BUF_SIZE	(8)

int main(void)
{
	uint8_t tx_data[BUF_SIZE];
	uint8_t rx_data[BUF_SIZE];

	/* open SPI port */
	rpiSpiOpen("/dev/spidev0.0");

	/* set SPI mode (CPOL: positive logic, CPHA: positive edge) */
	rpiSpiSetSpiMode(SPI_MODE_0);

	/* set transfer speed (2MHz) */
	rpiSpiSetSpeed(2000000UL);

	/* set transfer delay time (10usec)*/
	rpiSpiSetDelay(10U);

	/* set bits per word (8bit) */
	rpiSpiSetBitsPerWord(8U);

	/* set CS polarity (negative logic) */
	rpiSpiSetCsPolarity(D_SPI_CS_NEG_LOGIC);

	/* transfer data */
	tx_data[0] = ...;
	tx_data[1] = ...;
	...
	tx_data[7] = ...;
	rpiSpiTransfer(tx_data, rx_data, BUF_SIZE);

	/* close SPI port */
	rpiSpiClose();

	return 0;
}
````

## License
[MIT](https://github.com/ngtkt0909/dot-emacs/blob/develop/LICENSE)

## Author
[T. Ngtk](https://github.com/ngtkt0909)
