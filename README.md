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
Not necessary.

### Usage
Sample code to enable clock generator:
```C
#include "rpi_clkgen.h"

int main(void)
{
	/* output 1HMz clock on GPIO-4 (1GHz / 1000) */
	rpiClkgenEnable(4U, D_RPI_CMGPCTL_MASH_INT, D_RPI_CMGPCTL_SRC_PLLC, 1000U, 0U);

	return 0;
}
```

Sample code to disable clock generator:
```C
#include "rpi_clkgen.h"

int main(void)
{
	/* disable clock on GPIO-4 */
	rpiClkgenDisable(4U);

	return 0;
}
```

Clock frequency of each clock sources:

| Clock Source   | Parameter              | Frequency (BCM2837) |
|:---------------|:-----------------------|--------------------:|
| GND            | D_RPI_CMGPCTL_SRC_GND  |  0 Hz               |
| oscillator     | D_RPI_CMGPCTL_SRC_OSC  |  19.2 MHz           |
| testdebug0     | D_RPI_CMGPCTL_SRC_DBG0 |  0 Hz               |
| testdebug1     | D_RPI_CMGPCTL_SRC_DBG1 |  0 Hz               |
| PLLA per       | D_RPI_CMGPCTL_SRC_PLLA |  0 Hz               |
| PLLC per       | D_RPI_CMGPCTL_SRC_PLLC |  1 GHz              |
| PLLD per       | D_RPI_CMGPCTL_SRC_PLLD |  500 MHz            |
| HDMI auxiliary | D_RPI_CMGPCTL_SRC_HDMI |  0 Hz               |

GPIO pins that can be used as a clock source:
```
GPIO-4, GPIO-5, GPIO-6, GPIO-20, GPIO-21, GPIO-32, GPIO-34, GPIO-42, GPIO-43, GPIO-44
```

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
Sample code to use I2C library:
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
Sample code to use SPI library:
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
Not necessary.

### Usage
Sample code to use register map library:
```C
#include "rpi_regmap.h"

int main(void)
{
	/* initialize register map library */
	rpiRegmapInit();

	/*
	 * other register map libraries, rpiRegmapSet***() and rpiRegmapGet***(), are available here.
	 * [Setter functions]
	 *   void rpiRegmapSetGpfselFsel(uint8_t pin, uint32_t fsel);
	 *   void rpiRegmapSetCmGpctlMash(uint8_t ch, uint32_t mash);
	 *   void rpiRegmapSetCmGpctlEnab(uint8_t ch, uint32_t enab);
	 *   void rpiRegmapSetCmGpctlSrc(uint8_t ch, uint32_t src);
	 *   void rpiRegmapSetCmGpdivDivi(uint8_t ch, uint32_t divi);
	 *   void rpiRegmapSetCmGpdivDivf(uint8_t ch, uint32_t divf);
	 * [Getter functions]
	 *   uint32_t rpiRegmapGetGpfselFsel(uint8_t pin);
	 *   uint32_t rpiRegmapGetCmGpctlMash(uint8_t ch);
	 *   uint32_t rpiRegmapGetCmGpctlBusy(uint8_t ch);
	 *   uint32_t rpiRegmapGetCmGpctlEnab(uint8_t ch);
	 *   uint32_t rpiRegmapGetCmGpctlSrc(uint8_t ch);
	 *   uint32_t rpiRegmapGetCmGpdivDivi(uint8_t ch);
	 *   uint32_t rpiRegmapGetCmGpdivDivf(uint8_t ch);
	 */
	...

	/* finalize register map library */
	rpiRegmapFinal();

	return 0;
}
```

For more details of libraries and SoC registers,
refer to the documentation introduced in the Documentation section and
[hardware manual](https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf).

***[Attention] The Above hardware manual is written for Raspberry Pi 1 (BCM2835).
Note that the base address of the peripheral register is different
on Raspberry Pi 2 (BCM2836) and Raspberry Pi 3 (BCM2837).
If you want to use my register map library on Raspberry Pi 1,
you must change the value of `D_RPI_BASE_GPIO` and `D_RPI_BASE_CM` as below.***

| Raspberry Pi   | SoC     | Base Address | D_RPI_BASE_GPIO | D_RPI_BASE_CM |
|:---------------|:--------|:-------------|:----------------|:--------------|
| Raspberry Pi 1 | BCM2835 | 0x20000000   | 0x20200000      | 0x20101000    |
| Raspberry Pi 2 | BCM2836 | 0x3F000000   | 0x3F200000      | 0x3F101000    |
| Raspberry Pi 3 | BCM2837 | 0x3F000000   | 0x3F200000      | 0x3F101000    |

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
