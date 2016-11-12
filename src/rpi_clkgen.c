/**
 * raspberry-pi-gpio
 *
 * Copyright (c) 2016 T. Ngtk
 *
 * Released under the MIT License.
 * https://github.com/ngtkt0909/raspberry-pi-gpio/blob/master/LICENSE
 */

#include <assert.h>

#include "rpi_common.h"
#include "rpi_clkgen.h"

/*------------------------------------------------------------------------------
	Defined Macros
------------------------------------------------------------------------------*/
#define D_CH_INVALID	(0xFF)		/* invalid channel number */
#define D_CH_GPCLK0		(0)			/* channel number of GPCLK0 */
#define D_CH_GPCLK1		(1)			/* channel number of GPCLK1 */
#define D_CH_GPCLK2		(2)			/* channel number of GPCLK2 */

/* check number of GPIO pin */
#define M_CHECK_PIN(pin)	((pin >= 0) && (pin <= 53))

/*------------------------------------------------------------------------------
	Type Definition
------------------------------------------------------------------------------*/
typedef struct t_clkgen_gpio_map {
	uint8_t	ch;			/* channel of clock manager */
	uint8_t	alt;		/* alternate function for clock manager */
} T_CLKGEN_GPIO_MAP;

/*------------------------------------------------------------------------------
	Global Variables
------------------------------------------------------------------------------*/
/* map table: number of GPIO pin -> channel of clock manager */
const T_CLKGEN_GPIO_MAP g_clkgen_gpio_ch_map[] = {
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 0 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 1 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 2 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 3 */
	{D_CH_GPCLK0,	D_RPI_GPFSEL_FSEL_ALT0},		/* GPIO pin 4 */
	{D_CH_GPCLK1,	D_RPI_GPFSEL_FSEL_ALT0},		/* GPIO pin 5 */
	{D_CH_GPCLK2,	D_RPI_GPFSEL_FSEL_ALT0},		/* GPIO pin 6 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 7 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 8 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 9 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 10 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 11 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 12 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 13 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 14 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 15 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 16 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 17 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 18 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 19 */
	{D_CH_GPCLK0,	D_RPI_GPFSEL_FSEL_ALT5},		/* GPIO pin 20 */
	{D_CH_GPCLK1,	D_RPI_GPFSEL_FSEL_ALT5},		/* GPIO pin 21 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 22 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 23 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 24 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 25 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 26 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 27 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 28 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 29 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 30 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 31 */
	{D_CH_GPCLK0,	D_RPI_GPFSEL_FSEL_ALT0},		/* GPIO pin 32 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 33 */
	{D_CH_GPCLK0,	D_RPI_GPFSEL_FSEL_ALT0},		/* GPIO pin 34 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 35 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 36 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 37 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 38 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 39 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 40 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 41 */
	{D_CH_GPCLK1,	D_RPI_GPFSEL_FSEL_ALT0},		/* GPIO pin 42 */
	{D_CH_GPCLK2,	D_RPI_GPFSEL_FSEL_ALT0},		/* GPIO pin 43 */
	{D_CH_GPCLK1,	D_RPI_GPFSEL_FSEL_ALT0},		/* GPIO pin 44 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 45 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 46 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 47 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 48 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 49 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 50 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 51 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 52 */
	{D_CH_INVALID,	D_RPI_GPFSEL_FSEL_INPUT},		/* GPIO pin 53 */
};

/*------------------------------------------------------------------------------
	Functions
------------------------------------------------------------------------------*/
/**
 * @brief Enable Clock Generator
 *
 * @param [in]	pin		number of GPIO pin
 *		@arg 0-53	GPIO pin 0 - GPIO pin 53
 * @param [in]	mash	MASH control
 *		@arg D_RPI_CMGPCTL_MASH_INT		integer division
 *		@arg D_RPI_CMGPCTL_MASH_1STAGE	1-stage MASH
 *		@arg D_RPI_CMGPCTL_MASH_2STAGE	2-stage MASH
 *		@arg D_RPI_CMGPCTL_MASH_3STAGE	3-stage MASH
 * @param [in]	src		clock source
 *		@arg D_RPI_CMGPCTL_SRC_GND		GND
 *		@arg D_RPI_CMGPCTL_SRC_OSC		oscillator
 *		@arg D_RPI_CMGPCTL_SRC_DBG0		testdebug0
 *		@arg D_RPI_CMGPCTL_SRC_DBG1		testdebug1
 *		@arg D_RPI_CMGPCTL_SRC_PLLA		PLLA per
 *		@arg D_RPI_CMGPCTL_SRC_PLLC		PLLC per
 *		@arg D_RPI_CMGPCTL_SRC_PLLD		PLLD per
 *		@arg D_RPI_CMGPCTL_SRC_HDMI		HDMI auxiliary
 * @param [in]	divi	integer part of divisor
 * @param [in]	divf	fractional part of divisor
 *
 * @return nothing
 */
void rpiClkgenEnable(uint8_t pin, uint32_t mash, uint32_t src, uint32_t divi, uint32_t divf)
{
	uint8_t ch, alt;
	int8_t ret;

	/* check parameter */
	assert(M_CHECK_PIN(pin));

	ch  = g_clkgen_gpio_ch_map[pin].ch;
	alt = g_clkgen_gpio_ch_map[pin].alt;

	/* initialize GPIO */
	ret = rpiGpioInit();
	assert(ret == E_OK);

	/* disable clock generator */
	rpiGpioSetCmGpctlEnab(ch, D_RPI_CMGPCTL_ENAB_OFF);
	while (rpiGpioGetCmGpctlBusy(ch) == D_RPI_CMGPCTL_BUSY_ON);

	/* set parameters */
	rpiGpioSetGpfselFsel(pin, alt);
	rpiGpioSetCmGpctlMash(ch, mash);
	rpiGpioSetCmGpctlSrc(ch, src);
	rpiGpioSetCmGpdivDivi(ch, divi);
	rpiGpioSetCmGpdivDivf(ch, divf);

	/* enable clock generator */
	rpiGpioSetCmGpctlEnab(ch, D_RPI_CMGPCTL_ENAB_ON);
	while (rpiGpioGetCmGpctlBusy(ch) == D_RPI_CMGPCTL_BUSY_OFF);

	/* finalize GPIO */
	ret = rpiGpioFinal();
	assert(ret == E_OK);
}

/**
 * @brief Disable Clock Generator
 *
 * @param [in]	pin		number of GPIO pin
 *		@arg 0-53	GPIO pin 0 - GPIO pin 53
 *
 * @return nothing
 */
void rpiClkgenDisable(uint8_t pin)
{
	uint8_t ch;
	int8_t ret;

	/* check parameter */
	assert(M_CHECK_PIN(pin));

	ch = g_clkgen_gpio_ch_map[pin].ch;

	/* initialize GPIO */
	ret = rpiGpioInit();
	assert(ret == E_OK);

	/* disable clock generator */
	rpiGpioSetCmGpctlEnab(ch, D_RPI_CMGPCTL_ENAB_OFF);
	while (rpiGpioGetCmGpctlBusy(ch) == D_RPI_CMGPCTL_BUSY_ON);

	/* reset parameters */
	rpiGpioSetGpfselFsel(pin, D_RPI_GPFSEL_FSEL_INPUT);
	rpiGpioSetCmGpctlMash(ch, D_RPI_CMGPCTL_MASH_INT);
	rpiGpioSetCmGpctlSrc(ch, D_RPI_CMGPCTL_SRC_GND);
	rpiGpioSetCmGpdivDivi(ch, 0U);
	rpiGpioSetCmGpdivDivf(ch, 0U);

	/* finalize GPIO */
	ret = rpiGpioFinal();
	assert(ret == E_OK);
}
