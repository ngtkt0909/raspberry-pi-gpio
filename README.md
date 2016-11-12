# raspberry-pi-gpio

* master:  
[![Build Status](https://travis-ci.org/ngtkt0909/raspberry-pi-gpio.svg?branch=master)](https://travis-ci.org/ngtkt0909/raspberry-pi-gpio)

* develop:  
[![Build Status](https://travis-ci.org/ngtkt0909/raspberry-pi-gpio.svg?branch=develop)](https://travis-ci.org/ngtkt0909/raspberry-pi-gpio)

## Overview
Library for Raspberry Pi GPIO.

* Clock Generator Library (rpi_clkgen.c, rpi_clkgen.h)
* GPIO Library (rpi_gpio.c, rpi_gpio.h)
* I2C Library (rpi_i2c.c, rpi_i2c.h)
* SPI Library (rpi_spi.c, rpi_spi.h)

## Clock Generator Library
### Preparation
(T.B.D.)

### Usage
(T.B.D.)

## GPIO Library
### Preparation
(T.B.D.)

### Usage
(T.B.D.)

## I2C Library
### Preparation
Enable I2C device driver:
```shell
$ sudo raspi-config
```
Select *[7 Advanced Options] > [A7 I2C] > [Yes]*.

You can discover device drivers at `/dev`:
```shell
$ ls /dev | grep i2c
```

### Usage
Sample code to use I2C library is shown below (error handling omitted):
```C
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
	rpiI2cSetSlave(0x12U);

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
```

## SPI Library
### Preparation
Enable SPI device driver:
```shell
$ sudo raspi-config
```
Select *[7 Advanced Options] > [A6 SPI] > [Yes]*.

You can discover device drivers at `/dev`:
```shell
$ ls /dev | grep spi
```

### Usage
Sample code to use SPI library is shown below (error handling omitted):
```C
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
```

## Documentation
Install tools to generate documentation:
```shell
$ sudo apt-get install doxygen graphviz texlive-full
```

Make html based documentation (./doc/html/index.html):
```shell
$ make doc
```

Make pdf based documentation (./doc/latex/refman.pdf):
```shell
$ cd doc/latex
$ make
```

## License
[MIT](https://github.com/ngtkt0909/dot-emacs/blob/develop/LICENSE)

## Author
[T. Ngtk](https://github.com/ngtkt0909)
