/**
 * @file		rpi_regmap.h
 * @brief		Register Map Library Header
 *
 * @author		T. Ngtk
 * @copyright	Copyright (c) 2016 T. Ngtk
 *
 * @par License
 *	Released under the MIT License.<BR>
 *	https://github.com/ngtkt0909/raspberry-pi-gpio/blob/master/LICENSE
 */

#ifndef __RPI_REGMAP_H__
#define __RPI_REGMAP_H__		/**< include guard */

#include <stdint.h>

/*------------------------------------------------------------------------------
	Defined Macros
------------------------------------------------------------------------------*/
#define D_RPI_BASE_GPIO					(0x3F200000)		/**< base address of GPIO */
#define D_RPI_BASE_CM					(0x3F101000)		/**< base address of clock manager */
#define D_RPI_BLOCK_SIZE				(4096)				/**< block size for mmap */

#define D_RPI_BASE_GPFSEL				(g_regmap_base_gpio + 0x00)		/**< GPIO Function Select */
#define D_RPI_BASE_GPSET				(g_regmap_base_gpio + 0x1C)		/**< GPIO Pin Output Set */
#define D_RPI_BASE_GPCLR				(g_regmap_base_gpio + 0x28)		/**< GPIO Pin Output Clear */
#define D_RPI_BASE_GPLEV				(g_regmap_base_gpio + 0x34)		/**< GPIO Pin Level */
#define D_RPI_BASE_GPEDS				(g_regmap_base_gpio + 0x40)		/**< GPIO Pin Event Detect Status */
#define D_RPI_BASE_GPREN				(g_regmap_base_gpio + 0x4C)		/**< GPIO Pin Rising Edge Detect Enable */
#define D_RPI_BASE_GPFEN				(g_regmap_base_gpio + 0x58)		/**< GPIO Pin Falling Edge Detect Enable */
#define D_RPI_BASE_GPHEN				(g_regmap_base_gpio + 0x64)		/**< GPIO Pin High Detect Enable */
#define D_RPI_BASE_GPLEN				(g_regmap_base_gpio + 0x70)		/**< GPIO Pin Low Detect Enable */
#define D_RPI_BASE_GPAREN				(g_regmap_base_gpio + 0x7C)		/**< GPIO Pin Async. Rising Edge Detect */
#define D_RPI_BASE_GPAFEN				(g_regmap_base_gpio + 0x88)		/**< GPIO Pin Async. Falling Edge Detect */
#define D_RPI_BASE_GPPUD				(g_regmap_base_gpio + 0x94)		/**< GPIO Pin Pull-up/down Enable */
#define D_RPI_BASE_GPPUDCLK				(g_regmap_base_gpio + 0x98)		/**< GPIO Pin Pull-up/down Enable Clock */
#define D_RPI_BASE_CMGPCTL				(g_regmap_base_cm   + 0x70)		/**< Clock Manager General Purpose Clocks Control */
#define D_RPI_BASE_CMGPDIV				(g_regmap_base_cm   + 0x74)		/**< Clock Manager General Purpose Clock Divisors */

#define M_RPI_ADDR_GPFSEL(pin)			(((uint32_t *)D_RPI_BASE_GPFSEL) + ((pin) / 10))	/**< address of GPFSEL */
#define M_RPI_ADDR_CMGPCTL(ch)			(((uint32_t *)D_RPI_BASE_CMGPCTL) + ((ch) << 1))	/**< address of CM_GPnCTL */
#define M_RPI_ADDR_CMGPDIV(ch)			(((uint32_t *)D_RPI_BASE_CMGPDIV) + ((ch) << 1))	/**< address of CM_GPnDIV */

#define M_RPI_SHAMT_GPFSEL_FSEL(pin)	(((pin) % 10) * 3)	/**< shift amount of GPFSEL.FSEL */
#define D_RPI_SHAMT_CMGPCTL_PASSWD		(24)				/**< shift amount of CM_GPnCTL.PASSWD */
#define D_RPI_SHAMT_CMGPCTL_MASH		(9)					/**< shift amount of CM_GPnCTL.MASH */
#define D_RPI_SHAMT_CMGPCTL_BUSY		(7)					/**< shift amount of CM_GPnCTL.BUSY */
#define D_RPI_SHAMT_CMGPCTL_ENAB		(4)					/**< shift amount of CM_GPnCTL.ENAB */
#define D_RPI_SHAMT_CMGPCTL_SRC			(0)					/**< shift amount of CM_GPnCTL.SRC */
#define D_RPI_SHAMT_CMGPDIV_PASSWD		(24)				/**< shift amount of CM_GPnDIV.PASSWD */
#define D_RPI_SHAMT_CMGPDIV_DIVI		(12)				/**< shift amount of CM_GPnDIV.DIVI */
#define D_RPI_SHAMT_CMGPDIV_DIVF		(0)					/**< shift amount of CM_GPnDIV.DIVF */

#define M_RPI_MASK_GPFSEL_FSEL(pin)		(0x00000007 << M_RPI_SHAMT_GPFSEL_FSEL(pin))
															/**< mask of GPFSEL.FSEL */
#define D_RPI_MASK_CMGPCTL_PASSWD		(0xFF000000)		/**< mask of CM_GPnCTL.PASSWD */
#define D_RPI_MASK_CMGPCTL_MASH			(0x00000600)		/**< mask of CM_GPnCTL.MASH */
#define D_RPI_MASK_CMGPCTL_BUSY			(0x00000080)		/**< mask of CM_GPnCTL.BUSY */
#define D_RPI_MASK_CMGPCTL_ENAB			(0x00000010)		/**< mask of CM_GPnCTL.ENAB */
#define D_RPI_MASK_CMGPCTL_SRC			(0x0000000F)		/**< mask of CM_GPnCTL.SRC */
#define D_RPI_MASK_CMGPDIV_PASSWD		(0xFF000000)		/**< mask of CM_GPnDIV.PASSWD */
#define D_RPI_MASK_CMGPDIV_DIVI			(0x00FFF000)		/**< mask of CM_GPnDIV.DIVI */
#define D_RPI_MASK_CMGPDIV_DIVF			(0x00000FFF)		/**< mask of CM_GPnDIV.DIVF */

/* GPFSEL.FSEL */
#define D_RPI_GPFSEL_FSEL_INPUT			(0x0)				/**< GPIO Pin is an input */
#define D_RPI_GPFSEL_FSEL_OUTPUT		(0x1)				/**< GPIO Pin is an output */
#define D_RPI_GPFSEL_FSEL_ALT0			(0x4)				/**< GPIO Pin takes alternate function 0 */
#define D_RPI_GPFSEL_FSEL_ALT1			(0x5)				/**< GPIO Pin takes alternate function 1 */
#define D_RPI_GPFSEL_FSEL_ALT2			(0x6)				/**< GPIO Pin takes alternate function 2 */
#define D_RPI_GPFSEL_FSEL_ALT3			(0x7)				/**< GPIO Pin takes alternate function 3 */
#define D_RPI_GPFSEL_FSEL_ALT4			(0x3)				/**< GPIO Pin takes alternate function 4 */
#define D_RPI_GPFSEL_FSEL_ALT5			(0x2)				/**< GPIO Pin takes alternate function 5 */

/* CM_GPnCTL.PASSWD */
#define D_RPI_CMGPCTL_PASSWD			(0x5A)				/**< clock manager password */

/* CM_GPnCTL.MASH */
#define D_RPI_CMGPCTL_MASH_INT			(0x0)				/**< integer division */
#define D_RPI_CMGPCTL_MASH_1STAGE		(0x1)				/**< 1-stage MASH (equivalent to non-MASH dividers) */
#define D_RPI_CMGPCTL_MASH_2STAGE		(0x2)				/**< 2-stage MASH */
#define D_RPI_CMGPCTL_MASH_3STAGE		(0x3)				/**< 3-stage MASH */

/* CM_GPnCTL.BUSY */
#define D_RPI_CMGPCTL_BUSY_OFF			(0x0)				/**< clock generator is 'not' running */
#define D_RPI_CMGPCTL_BUSY_ON			(0x1)				/**< clock generator is running */

/* CM_GPnCTL.ENAB */
#define D_RPI_CMGPCTL_ENAB_OFF			(0x0)				/**< disable the clock generator */
#define D_RPI_CMGPCTL_ENAB_ON			(0x1)				/**< enable the clock generator */

/* CM_GPnCTL.SRC */
#define D_RPI_CMGPCTL_SRC_GND			(0x0)				/**< GND */
#define D_RPI_CMGPCTL_SRC_OSC			(0x1)				/**< oscillator */
#define D_RPI_CMGPCTL_SRC_DBG0			(0x2)				/**< testdebug0 */
#define D_RPI_CMGPCTL_SRC_DBG1			(0x3)				/**< testdebug1 */
#define D_RPI_CMGPCTL_SRC_PLLA			(0x4)				/**< PLLA per */
#define D_RPI_CMGPCTL_SRC_PLLC			(0x5)				/**< PLLC per */
#define D_RPI_CMGPCTL_SRC_PLLD			(0x6)				/**< PLLD per */
#define D_RPI_CMGPCTL_SRC_HDMI			(0x7)				/**< HDMI auxiliary */

/* CM_GPnDIV.PASSWD */
#define D_RPI_CMGPDIV_PASSWD			(0x5A)				/**< clock manager password */

/*------------------------------------------------------------------------------
	Prototype Declaration
------------------------------------------------------------------------------*/
int8_t rpiRegmapInit();
int8_t rpiRegmapFinal();

void rpiRegmapSetGpfselFsel(uint8_t pin, uint32_t fsel);
void rpiRegmapSetCmGpctlMash(uint8_t ch, uint32_t mash);
void rpiRegmapSetCmGpctlEnab(uint8_t ch, uint32_t enab);
void rpiRegmapSetCmGpctlSrc(uint8_t ch, uint32_t src);
void rpiRegmapSetCmGpdivDivi(uint8_t ch, uint32_t divi);
void rpiRegmapSetCmGpdivDivf(uint8_t ch, uint32_t divf);

uint32_t rpiRegmapGetGpfselFsel(uint8_t pin);
uint32_t rpiRegmapGetCmGpctlMash(uint8_t ch);
uint32_t rpiRegmapGetCmGpctlBusy(uint8_t ch);
uint32_t rpiRegmapGetCmGpctlEnab(uint8_t ch);
uint32_t rpiRegmapGetCmGpctlSrc(uint8_t ch);
uint32_t rpiRegmapGetCmGpdivDivi(uint8_t ch);
uint32_t rpiRegmapGetCmGpdivDivf(uint8_t ch);

#endif /* __RPI_REGMAP_H__ */
