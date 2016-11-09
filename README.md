# raspberry-pi-gpio

## Overview
Library for Raspberry Pi GPIO.

* I2C Library (rpi_i2c.c, rpi_i2c.h)
* SPI Library (rpi_spi.c, rpi_spi.h)

## I2C Library
### Usage
Sample code to use I2C library is shown below (error handling omitted):
````C
#include "rpi_i2c.h"

#define BUF_SIZE	(8)

int main(void)
{
	uint8_t cmd, data;
	uint8_t tx_data[BUF_SIZE];
	uint8_t rx_data[BUF_SIZE];

	/* open SPI port */
	rpiI2cOpen("/dev/i2c-1");

	/* set slave address (0x12) */
	rpiI2cSetSlave(0x12);

	/* write data */
	cmd = ...;
	data = ...;
	rpiI2cWrite(cmd, &data);

	/* read data */
	cmd = ...;
	rpiI2cRead(cmd, &data);

	/* write block data */
	cmd = ...;
	tx_data[0] = ...;
	tx_data[1] = ...;
	...
	tx_data[7] = ...;
	rpiI2cWriteBlock(cmd, tx_data, BUF_SIZE);

	/* read block data */
	cmd = ...;
	rpiI2cReadBlock(cmd, rx_data, BUF_SIZE);

	/* close I2C port */
	rpiI2cClose();

	return 0;
}
````

## SPI Library
### Usage
Sample code to use SPI library is shown below (error handling omitted):
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
	rpiSpiSetMode(SPI_MODE_0);

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
