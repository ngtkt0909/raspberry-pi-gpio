# raspberry-pi-gpio

* master:  
[![Build Status](https://travis-ci.org/ngtkt0909/raspberry-pi-gpio.svg?branch=master)](https://travis-ci.org/ngtkt0909/raspberry-pi-gpio)

* develop:  
[![Build Status](https://travis-ci.org/ngtkt0909/raspberry-pi-gpio.svg?branch=develop)](https://travis-ci.org/ngtkt0909/raspberry-pi-gpio)

## Overview
Library for Raspberry Pi GPIO.

* Clock Generator Library (rpi_clkgen.c, rpi_clkgen.h)
* I2C Library (rpi_i2c.c, rpi_i2c.h)
* SPI Library (rpi_spi.c, rpi_spi.h)
* Register Map Library (rpi_regmap.c, rpi_regmap.h)

## Clock Generator Library
### Preparation
Nothing.

### Usage
Sample code to enable clock generator is shown below:
```C
#include "rpi_clkgen.h"

int main(void)
{
	/* output 1HMz clock on GPIO-4 (1GHz / 1000) */
	rpiClkgenEnable(4, D_RPI_CMGPCTL_MASH_INT, D_RPI_CMGPCTL_SRC_PLLC, 1000U, 0U);

	return 0;
}
```

Sample code to disable clock generator is shown below:
```C
#include "rpi_clkgen.h"

int main(void)
{
	/* disable clock on GPIO-4 */
	rpiClkgenDisable(4);

	return 0;
}
```

Clock frequency of each clock sources:

| Clock Source           | Frequency (BCM2837) |
|:-----------------------|--------------------:|
| D_RPI_CMGPCTL_SRC_GND  |  0 Hz               |
| D_RPI_CMGPCTL_SRC_OSC  |  19.2 MHz           |
| D_RPI_CMGPCTL_SRC_DBG0 |  0 Hz               |
| D_RPI_CMGPCTL_SRC_DBG1 |  0 Hz               |
| D_RPI_CMGPCTL_SRC_PLLA |  0 Hz               |
| D_RPI_CMGPCTL_SRC_PLLC |  1 GHz              |
| D_RPI_CMGPCTL_SRC_PLLD |  500 MHz            |
| D_RPI_CMGPCTL_SRC_HDMI |  0 Hz               |

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

## Register Map Library
### Preparation
(T.B.D.)

### Usage
(T.B.D.)

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
