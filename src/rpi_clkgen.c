/**
 * @file		rpi_clkgen.c
 * @brief		Clock Generator Library Implementation
 *
 * @author		T. Ngtk
 * @copyright	Copyright (c) 2016 T. Ngtk
 *
 * @par License
 *	Released under the MIT License.<BR>
 *	https://github.com/ngtkt0909/raspberry-pi-gpio/blob/master/LICENSE
 */

#include <assert.h>
#include "rpi_clkgen.h"

/*------------------------------------------------------------------------------
	Defined Macros
------------------------------------------------------------------------------*/
#define D_CH_INVALID	(0xFF)		/**< invalid channel number */
#define D_CH_GPCLK0		(0)			/**< channel number of GPCLK0 */
#define D_CH_GPCLK1		(1)			/**< channel number of GPCLK1 */
#define D_CH_GPCLK2		(2)			/**< channel number of GPCLK2 */

/** check number of GPIO pin */
#define M_CHECK_PIN(pin)	((pin >= 0) && (pin <= 53))

/** check channel of clock manager */
#define M_CHECK_CH(ch)		((ch >= D_CH_GPCLK0) && (ch <= D_CH_GPCLK2))

/** check alternate function */
#define M_CHECK_FSEL(fsel) \
	((fsel == D_RPI_GPFSEL_FSEL_ALT0) || \
	 (fsel == D_RPI_GPFSEL_FSEL_ALT1) || \
	 (fsel == D_RPI_GPFSEL_FSEL_ALT2) || \
	 (fsel == D_RPI_GPFSEL_FSEL_ALT3) || \
	 (fsel == D_RPI_GPFSEL_FSEL_ALT4) || \
	 (fsel == D_RPI_GPFSEL_FSEL_ALT5))

/*------------------------------------------------------------------------------
	Type Definition
------------------------------------------------------------------------------*/
/** @brief GPIO - clock manager map table */
typedef struct t_clkgen_gpio_map {
	uint8_t	ch;			/**< channel of clock manager */
	uint8_t	fsel;		/**< alternate function for clock manager */
} T_CLKGEN_GPIO_MAP;

/*------------------------------------------------------------------------------
	Global Variables
------------------------------------------------------------------------------*/
/** map table: number of GPIO pin -> channel of clock manager */
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
	uint8_t ch, fsel;
	int8_t ret;

	/* check parameter */
	assert(M_CHECK_PIN(pin));
	ch   = g_clkgen_gpio_ch_map[pin].ch;
	fsel = g_clkgen_gpio_ch_map[pin].fsel;
	assert(M_CHECK_CH(ch));
	assert(M_CHECK_FSEL(fsel));

	/* initialize register map */
	ret = rpiRegmapInit();
	assert(ret == E_OK);

	/* disable clock generator */
	rpiRegmapSetCmGpctlEnab(ch, D_RPI_CMGPCTL_ENAB_OFF);
	while (rpiRegmapGetCmGpctlBusy(ch) == D_RPI_CMGPCTL_BUSY_ON);

	/* set parameters */
	rpiRegmapSetGpfselFsel(pin, fsel);
	rpiRegmapSetCmGpctlMash(ch, mash);
	rpiRegmapSetCmGpctlSrc(ch, src);
	rpiRegmapSetCmGpdivDivi(ch, divi);
	rpiRegmapSetCmGpdivDivf(ch, divf);

	/* enable clock generator */
	rpiRegmapSetCmGpctlEnab(ch, D_RPI_CMGPCTL_ENAB_ON);
	while (rpiRegmapGetCmGpctlBusy(ch) == D_RPI_CMGPCTL_BUSY_OFF);

	/* finalize register map */
	ret = rpiRegmapFinal();
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
	assert(M_CHECK_CH(ch));

	/* initialize register map */
	ret = rpiRegmapInit();
	assert(ret == E_OK);

	/* disable clock generator */
	rpiRegmapSetCmGpctlEnab(ch, D_RPI_CMGPCTL_ENAB_OFF);
	while (rpiRegmapGetCmGpctlBusy(ch) == D_RPI_CMGPCTL_BUSY_ON);

	/* reset parameters */
	rpiRegmapSetGpfselFsel(pin, D_RPI_GPFSEL_FSEL_INPUT);
	rpiRegmapSetCmGpctlMash(ch, D_RPI_CMGPCTL_MASH_INT);
	rpiRegmapSetCmGpctlSrc(ch, D_RPI_CMGPCTL_SRC_GND);
	rpiRegmapSetCmGpdivDivi(ch, 0U);
	rpiRegmapSetCmGpdivDivf(ch, 0U);

	/* finalize register map */
	ret = rpiRegmapFinal();
	assert(ret == E_OK);
}
