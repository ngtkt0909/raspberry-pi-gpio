/**
 * raspberry-pi-gpio
 *
 * Copyright (c) 2016 T. Ngtk
 *
 * Released under the MIT License.
 * https://github.com/ngtkt0909/raspberry-pi-gpio/blob/master/LICENSE
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

#include "rpi_common.h"
#include "rpi_gpio.h"

/*------------------------------------------------------------------------------
	Defined Macros
------------------------------------------------------------------------------*/
/* check base address of GPIO */
#define M_CHECK_BASE_GPIO()	(g_gpio_base_gpio != NULL)

/* check base address of clock manager */
#define M_CHECK_BASE_CM()	(g_gpio_base_cm != NULL)

/* check number of GPIO pin */
#define M_CHECK_PIN(pin)	((pin >= 0) && (pin <= 53))

/* check channel of clock manager */
#define M_CHECK_CM_CH(ch)	((ch >= 0) && (ch<= 2))

/* check CM_GPnCTL.MASH */
#define M_CHECK_CM_MASH(mash) \
	((mash >= D_RPI_CMGPCTL_MASH_INT) && (mash <= D_RPI_CMGPCTL_MASH_3STAGE))

/* check CM_GPnCTL.ENAB */
#define M_CHECK_CM_ENAB(enab) \
	((enab >= D_RPI_CMGPCTL_ENAB_OFF) && (enab <= D_RPI_CMGPCTL_ENAB_ON))

/* check CM_GPnCTL.SRC */
#define M_CHECK_CM_SRC(src) \
	((src >= D_RPI_CMGPCTL_SRC_GND) && (src <= D_RPI_CMGPCTL_SRC_HDMI))

/*------------------------------------------------------------------------------
	Global Variables
------------------------------------------------------------------------------*/
static volatile uint8_t *g_gpio_base_gpio = NULL;		/* base address of GPIO */
static volatile uint8_t *g_gpio_base_cm   = NULL;		/* base address of clock manager */

/*------------------------------------------------------------------------------
	Functions
------------------------------------------------------------------------------*/
/**
 * @brief Initialize GPIO
 *
 * @param nothing
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiGpioInit()
{
	int fd;
	void *mmap_gpio, *mmap_cm;

	if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) {
		perror("open");
		return E_OBJ;
	}

	/* map GPIO */
	if ((mmap_gpio = mmap(NULL, D_RPI_BLOCK_SIZE,
						PROT_READ | PROT_WRITE, MAP_SHARED,
						fd, D_RPI_BASE_GPIO)) == MAP_FAILED) {
		perror("mmap");
		return E_OBJ;
	}
	g_gpio_base_gpio = (volatile uint8_t *)mmap_gpio;

	/* map clock manager */
	if ((mmap_cm = mmap(NULL, D_RPI_BLOCK_SIZE,
						PROT_READ | PROT_WRITE, MAP_SHARED,
						fd, D_RPI_BASE_GPIO)) == MAP_FAILED) {
		perror("mmap");
		return E_OBJ;
	}
	g_gpio_base_cm = (volatile uint8_t *)mmap_cm;

	if (close(fd) == -1) {
		perror("close");
		return E_OBJ;
	}

	return E_OK;
}

/**
 * @brief Finalize GPIO
 *
 * @param nothing
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiGpioFinal()
{
	/* unmap GPIO */
	if (munmap((void *)g_gpio_base_gpio, D_RPI_BLOCK_SIZE) == -1) {
		perror("munmap");
		return E_OBJ;
	}
	g_gpio_base_gpio = NULL;

	/* unmap clock manager */
	if (munmap((void *)g_gpio_base_cm, D_RPI_BLOCK_SIZE) == -1) {
		perror("munmap");
		return E_OBJ;
	}
	g_gpio_base_cm = NULL;

	return E_OK;
}

/**
 * @brief Setter of GPFSEL.FSEL
 *
 * @param [in]	pin		number of GPIO pin
 *		@arg 0-53	GPIO pin 0 - GPIO pin 53
 * @param [in]	fsel	function select
 *		@arg D_RPI_GPFSEL_FSEL_INPUT	input
 *		@arg D_RPI_GPFSEL_FSEL_OUTPUT	output
 *		@arg D_RPI_GPFSEL_FSEL_ALT0		alternate function 0
 *		@arg D_RPI_GPFSEL_FSEL_ALT1		alternate function 1
 *		@arg D_RPI_GPFSEL_FSEL_ALT2		alternate function 2
 *		@arg D_RPI_GPFSEL_FSEL_ALT3		alternate function 3
 *		@arg D_RPI_GPFSEL_FSEL_ALT4		alternate function 4
 *		@arg D_RPI_GPFSEL_FSEL_ALT5		alternate function 5
 *
 * @return nothing
 */
void rpiGpioSetGpfselFsel(uint8_t pin, uint32_t fsel)
{
	uint32_t *addr = M_RPI_ADDR_GPFSEL(pin);
	uint8_t shamt = M_RPI_SHAMT_GPFSEL_FSEL(pin);
	uint32_t mask = (D_RPI_MASK_GPFSEL_FSEL << shamt);

	/* check parameter */
	assert(M_CHECK_BASE_GPIO());
	assert(M_CHECK_PIN(pin));

	/* set FSEL */
	*addr = ((fsel << shamt) & mask) | (*addr & ~mask);
}

/**
 * @brief Setter of CM_GPnCTL.MASH
 *
 * @param [in]	ch		channel of clock manager
 *		@arg 0-2	CM_GP0CTL - CM_GP2CTL
 * @param [in]	mash	MASH control
 *		@arg D_RPI_CMGPCTL_MASH_INT		integer division
 *		@arg D_RPI_CMGPCTL_MASH_1STAGE	1-stage MASH
 *		@arg D_RPI_CMGPCTL_MASH_2STAGE	2-stage MASH
 *		@arg D_RPI_CMGPCTL_MASH_3STAGE	3-stage MASH
 *
 * @return nothing
 */
void rpiGpioSetCmGpctlMash(uint8_t ch, uint32_t mash)
{
	uint32_t *addr = M_RPI_ADDR_CMGPCTL(ch);
	uint32_t mask = (D_RPI_MASK_CMGPCTL_PASSWD << D_RPI_SHAMT_CMGPCTL_PASSWD) |
					(D_RPI_MASK_CMGPCTL_MASH   << D_RPI_SHAMT_CMGPCTL_MASH);

	/* check parameter */
	assert(M_CHECK_BASE_CM());
	assert(M_CHECK_CM_CH(ch));
	assert(M_CHECK_CM_MASH(mash));

	/* set MASH */
	*addr = ((((uint32_t)D_RPI_CMGPCTL_PASSWD << D_RPI_SHAMT_CMGPCTL_PASSWD) |
			  (mash                           << D_RPI_SHAMT_CMGPCTL_MASH)) &  mask) |
			(*addr                                                          & ~mask);
}

/**
 * @brief Setter of CM_GPnCTL.ENAB
 *
 * @param [in]	ch		channel of clock manager
 *		@arg 0-2	CM_GP0CTL - CM_GP2CTL
 * @param [in]	enab	enable/disable the clock generator
 *		@arg D_RPI_CMGPCTL_ENAB_OFF		disable the clock generator
 *		@arg D_RPI_CMGPCTL_ENAB_ON		enable the clock generator
 *
 * @return nothing
 */
void rpiGpioSetCmGpctlEnab(uint8_t ch, uint32_t enab)
{
	uint32_t *addr = M_RPI_ADDR_CMGPCTL(ch);
	uint32_t mask = (D_RPI_MASK_CMGPCTL_PASSWD << D_RPI_SHAMT_CMGPCTL_PASSWD) |
					(D_RPI_MASK_CMGPCTL_ENAB   << D_RPI_SHAMT_CMGPCTL_ENAB);

	/* check parameter */
	assert(M_CHECK_BASE_CM());
	assert(M_CHECK_CM_CH(ch));
	assert(M_CHECK_CM_ENAB(enab));

	/* set ENAB */
	*addr = ((((uint32_t)D_RPI_CMGPCTL_PASSWD << D_RPI_SHAMT_CMGPCTL_PASSWD) |
			  (enab                           << D_RPI_SHAMT_CMGPCTL_ENAB)) &  mask) |
			(*addr                                                          & ~mask);
}

/**
 * @brief Setter of CM_GPnCTL.SRC
 *
 * @param [in]	ch		channel of clock manager
 *		@arg 0-2	CM_GP0CTL - CM_GP2CTL
 * @param [in]	src		clock source
 *		@arg D_RPI_CMGPCTL_SRC_GND		GND
 *		@arg D_RPI_CMGPCTL_SRC_OSC		oscillator
 *		@arg D_RPI_CMGPCTL_SRC_DBG0		testdebug0
 *		@arg D_RPI_CMGPCTL_SRC_DBG1		testdebug1
 *		@arg D_RPI_CMGPCTL_SRC_PLLA		PLLA per
 *		@arg D_RPI_CMGPCTL_SRC_PLLC		PLLC per
 *		@arg D_RPI_CMGPCTL_SRC_PLLD		PLLD per
 *		@arg D_RPI_CMGPCTL_SRC_HDMI		HDMI auxiliary
 *
 * @return nothing
 */
void rpiGpioSetCmGpctlSrc(uint8_t ch, uint32_t src)
{
	uint32_t *addr = M_RPI_ADDR_CMGPCTL(ch);
	uint32_t mask = (D_RPI_MASK_CMGPCTL_PASSWD << D_RPI_SHAMT_CMGPCTL_PASSWD) |
					(D_RPI_MASK_CMGPCTL_SRC    << D_RPI_SHAMT_CMGPCTL_SRC);

	/* check parameter */
	assert(M_CHECK_BASE_CM());
	assert(M_CHECK_CM_CH(ch));
	assert(M_CHECK_CM_SRC(src));

	/* set SRC */
	*addr = ((((uint32_t)D_RPI_CMGPCTL_PASSWD << D_RPI_SHAMT_CMGPCTL_PASSWD) |
			  (src                            << D_RPI_SHAMT_CMGPCTL_SRC)) &  mask) |
			(*addr                                                         & ~mask);
}

/**
 * @brief Setter of CM_GPnDIV.DIVI
 *
 * @param [in]	ch		channel of clock manager
 *		@arg 0-2	CM_GP0DIV - CM_GP2DIV
 * @param [in]	divi	integer part of divisor
 *
 * @return nothing
 */
void rpiGpioSetCmGpdivDivi(uint8_t ch, uint32_t divi)
{
	uint32_t *addr = M_RPI_ADDR_CMGPDIV(ch);
	uint32_t mask = (D_RPI_MASK_CMGPDIV_PASSWD << D_RPI_SHAMT_CMGPDIV_PASSWD) |
					(D_RPI_MASK_CMGPDIV_DIVI   << D_RPI_SHAMT_CMGPDIV_DIVI);

	/* check parameter */
	assert(M_CHECK_BASE_CM());
	assert(M_CHECK_CM_CH(ch));

	/* set DIVI */
	*addr = ((((uint32_t)D_RPI_CMGPDIV_PASSWD << D_RPI_SHAMT_CMGPDIV_PASSWD) |
			  (divi                           << D_RPI_SHAMT_CMGPDIV_DIVI)) &  mask) |
			(*addr                                                          & ~mask);
}

/**
 * @brief Setter of CM_GPnDIV.DIVF
 *
 * @param [in]	ch		channel of clock manager
 *		@arg 0-2	CM_GP0DIV - CM_GP2DIV
 * @param [in]	divf	fractional part of divisor
 *
 * @return nothing
 */
void rpiGpioSetCmGpdivDivf(uint8_t ch, uint32_t divf)
{
	uint32_t *addr = M_RPI_ADDR_CMGPDIV(ch);
	uint32_t mask = (D_RPI_MASK_CMGPDIV_PASSWD << D_RPI_SHAMT_CMGPDIV_PASSWD) |
					(D_RPI_MASK_CMGPDIV_DIVF   << D_RPI_SHAMT_CMGPDIV_DIVF);

	/* check parameter */
	assert(M_CHECK_BASE_CM());
	assert(M_CHECK_CM_CH(ch));

	/* set DIVF */
	*addr = ((((uint32_t)D_RPI_CMGPDIV_PASSWD << D_RPI_SHAMT_CMGPDIV_PASSWD) |
			  (divf                           << D_RPI_SHAMT_CMGPDIV_DIVF)) &  mask) |
			(*addr                                                          & ~mask);
}

/**
 * @brief Getter of GPFSEL.FSEL
 *
 * @param [in]	pin		number of GPIO pin
 *		@arg 0-53	GPIO pin 0 - GPIO pin 53
 *
 * @retval D_RPI_GPFSEL_FSEL_INPUT	input
 * @retval D_RPI_GPFSEL_FSEL_OUTPUT	output
 * @retval D_RPI_GPFSEL_FSEL_ALT0	alternate function 0
 * @retval D_RPI_GPFSEL_FSEL_ALT1	alternate function 1
 * @retval D_RPI_GPFSEL_FSEL_ALT2	alternate function 2
 * @retval D_RPI_GPFSEL_FSEL_ALT3	alternate function 3
 * @retval D_RPI_GPFSEL_FSEL_ALT4	alternate function 4
 * @retval D_RPI_GPFSEL_FSEL_ALT5	alternate function 5
 */
uint32_t rpiGpioGetGpfselFsel(uint8_t pin)
{
	uint32_t *addr = M_RPI_ADDR_GPFSEL(pin);

	/* check parameter */
	assert(M_CHECK_BASE_GPIO());
	assert(M_CHECK_PIN(pin));

	/* get FSEL */
	return (*addr >> M_RPI_SHAMT_GPFSEL_FSEL(pin)) & D_RPI_MASK_GPFSEL_FSEL;
}

/**
 * @brief Getter of CM_GPnCTL.MASH
 *
 * @param [in]	ch		channel of clock manager
 *		@arg 0-2	CM_GP0CTL - CM_GP2CTL
 *
 * @retval D_RPI_CMGPCTL_MASH_INT		integer division
 * @retval D_RPI_CMGPCTL_MASH_1STAGE	1-stage MASH
 * @retval D_RPI_CMGPCTL_MASH_2STAGE	2-stage MASH
 * @retval D_RPI_CMGPCTL_MASH_3STAGE	3-stage MASH
 */
uint32_t rpiGpioGetCmGpctlMash(uint8_t ch)
{
	uint32_t *addr = M_RPI_ADDR_CMGPCTL(ch);

	/* check parameter */
	assert(M_CHECK_BASE_CM());
	assert(M_CHECK_CM_CH(ch));

	/* get MASH */
	return (*addr >> D_RPI_SHAMT_CMGPCTL_MASH) & D_RPI_MASK_CMGPCTL_MASH;
}

/**
 * @brief Getter of CM_GPnCTL.BUSY
 *
 * @param [in]	ch		channel of clock manager
 *		@arg 0-2	CM_GP0CTL - CM_GP2CTL
 *
 * @retval D_RPI_CMGPCTL_BUSY_OFF	clock generator is 'not' running
 * @retval D_RPI_CMGPCTL_BUSY_ON	clock generator is running
 */
uint32_t rpiGpioGetCmGpctlBusy(uint8_t ch)
{
	uint32_t *addr = M_RPI_ADDR_CMGPCTL(ch);

	/* check parameter */
	assert(M_CHECK_BASE_CM());
	assert(M_CHECK_CM_CH(ch));

	/* get BUSY */
	return (*addr >> D_RPI_SHAMT_CMGPCTL_BUSY) & D_RPI_MASK_CMGPCTL_BUSY;
}

/**
 * @brief Getter of CM_GPnCTL.ENAB
 *
 * @param [in]	ch		channel of clock manager
 *		@arg 0-2	CM_GP0CTL - CM_GP2CTL
 *
 * @retval D_RPI_CMGPCTL_ENAB_OFF	disable the clock generator
 * @retval D_RPI_CMGPCTL_ENAB_ON	enable the clock generator
 */
uint32_t rpiGpioGetCmGpctlEnab(uint8_t ch)
{
	uint32_t *addr = M_RPI_ADDR_CMGPCTL(ch);

	/* check parameter */
	assert(M_CHECK_BASE_CM());
	assert(M_CHECK_CM_CH(ch));

	/* get ENAB */
	return (*addr >> D_RPI_SHAMT_CMGPCTL_ENAB) & D_RPI_MASK_CMGPCTL_ENAB;
}

/**
 * @brief Getter of CM_GPnCTL.SRC
 *
 * @param [in]	ch		channel of clock manager
 *		@arg 0-2	CM_GP0CTL - CM_GP2CTL
 *
 * @retval D_RPI_CMGPCTL_SRC_GND	GND
 * @retval D_RPI_CMGPCTL_SRC_OSC	oscillator
 * @retval D_RPI_CMGPCTL_SRC_DBG0	testdebug0
 * @retval D_RPI_CMGPCTL_SRC_DBG1	testdebug1
 * @retval D_RPI_CMGPCTL_SRC_PLLA	PLLA per
 * @retval D_RPI_CMGPCTL_SRC_PLLC	PLLC per
 * @retval D_RPI_CMGPCTL_SRC_PLLD	PLLD per
 * @retval D_RPI_CMGPCTL_SRC_HDMI	HDMI auxiliary
 */
uint32_t rpiGpioGetCmGpctlSrc(uint8_t ch)
{
	uint32_t *addr = M_RPI_ADDR_CMGPCTL(ch);

	/* check parameter */
	assert(M_CHECK_BASE_CM());
	assert(M_CHECK_CM_CH(ch));

	/* get SRC */
	return (*addr >> D_RPI_SHAMT_CMGPCTL_SRC) & D_RPI_MASK_CMGPCTL_SRC;
}

/**
 * @brief Getter of CM_GPnDIV.DIVI
 *
 * @param [in]	ch		channel of clock manager
 *		@arg 0-2	CM_GP0DIV - CM_GP2DIV
 *
 * @return integer part of divisor
 */
uint32_t rpiGpioGetCmGpdivDivi(uint8_t ch)
{
	uint32_t *addr = M_RPI_ADDR_CMGPDIV(ch);

	/* check parameter */
	assert(M_CHECK_BASE_CM());
	assert(M_CHECK_CM_CH(ch));

	/* get DIVI */
	return (*addr >> D_RPI_SHAMT_CMGPDIV_DIVI) & D_RPI_MASK_CMGPDIV_DIVI;
}

/**
 * @brief Getter of CM_GPnDIV.DIVF
 *
 * @param [in]	ch		channel of clock manager
 *		@arg 0-2	CM_GP0DIV - CM_GP2DIV
 *
 * @return fractional part of divisor
 */
uint32_t rpiGpioGetCmGpdivDivf(uint8_t ch)
{
	uint32_t *addr = M_RPI_ADDR_CMGPDIV(ch);

	/* check parameter */
	assert(M_CHECK_BASE_CM());
	assert(M_CHECK_CM_CH(ch));

	/* get DIVF */
	return (*addr >> D_RPI_SHAMT_CMGPDIV_DIVF) & D_RPI_MASK_CMGPDIV_DIVF;
}
