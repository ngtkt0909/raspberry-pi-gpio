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
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "rpi_common.h"
#include "rpi_spi.h"

/*------------------------------------------------------------------------------
	Defined Macros
------------------------------------------------------------------------------*/
#define D_FD_NOT_OPEND			(-1)		/* file descriptor (not opened) */

/* check port opened */
#define M_CHECK_OPENED(fd)	(fd != D_FD_NOT_OPEND)

/* check SPI mode */
#define M_CHECK_MODE(mode) \
	((mode == SPI_MODE_0) || (mode == SPI_MODE_1) || (mode == SPI_MODE_2) || (mode == SPI_MODE_3))

/*------------------------------------------------------------------------------
	Global Variables
------------------------------------------------------------------------------*/
static int		g_spi_fd			= D_FD_NOT_OPEND;	/* file descriptor */
static uint8_t	g_spi_mode			= SPI_MODE_0;		/* SPI mode */
static uint32_t	g_spi_speed			= 1000000UL;		/* transfer speed */
static uint16_t	g_spi_delay			= 0U;				/* transfer delay time */
static uint8_t	g_spi_bits_per_word	= 8U;				/* bits per word */
static uint8_t	g_spi_cs_polarity	= 0U;				/* CS polarity */

/*------------------------------------------------------------------------------
	Functions
------------------------------------------------------------------------------*/
/**
 * @brief SPI Port Open
 *
 * @param [in]	dev_path	string of path for device node (e.g. "/dev/spidev0.0")
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiSpiOpen(uint8_t *dev_path)
{
	/* check opened port */
	if (M_CHECK_OPENED(g_spi_fd)) {
		fprintf(stderr, "rpiSpiOpen failed: SPI port is already opened\n");
		return E_OBJ;
	}

	/* open SPI port */
	if ((g_spi_fd = open(dev_path, O_RDWR)) == -1) {
		perror("open");
		return E_OBJ;
	}

	/* set default SPI mode */
	if (rpiSpiSetSpiMode(g_spi_mode) != E_OK) {
		fprintf(stderr, "rpiSpiSetSpiMode failed\n");
		return E_OBJ;
	}

	/* set default transfer speed */
	if (rpiSpiSetSpeed(g_spi_speed) != E_OK) {
		fprintf(stderr, "rpiSpiSetSpeed failed\n");
		return E_OBJ;
	}

	/* set default transfer delay time */
	if (rpiSpiSetDelay(g_spi_delay) != E_OK) {
		fprintf(stderr, "rpiSpiSetDelay failed\n");
		return E_OBJ;
	}

	/* set default bits per word */
	if (rpiSpiSetBitsPerWord(g_spi_bits_per_word) != E_OK) {
		fprintf(stderr, "rpiSpiSetBitsPerWord failed\n");
		return E_OBJ;
	}

	/* set default CS polarity */
	if (rpiSpiSetCsPolarity(g_spi_cs_polarity) != E_OK) {
		fprintf(stderr, "rpiSpiSetCsPolarity failed\n");
		return E_OBJ;
	}

	return E_OK;
}

/**
 * @brief SPI Port Close
 *
 * @param Nothing
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiSpiClose()
{
	/* check opened port */
	if (!M_CHECK_OPENED(g_spi_fd)) {
		fprintf(stderr, "rpiSpiClose failed: SPI port isn't opened\n");
		return E_OBJ;
	}

	/* close SPI port */
	if (close(g_spi_fd) == -1) {
		perror("close");
		return E_OBJ;
	}

	/* clear file descriptor */
	g_spi_fd = D_FD_NOT_OPEND;

	return E_OK;
}

/**
 * @brief SPI Data Transfer
 *
 * @param [in]	tx_data		address of write data buffer
 * @param [out]	rx_data		address of read data buffer
 * @param [in]	size		buffer size
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiSpiTransfer(uint8_t *tx_data, uint8_t *rx_data, uint32_t size)
{
	struct spi_ioc_transfer msg = {
		.tx_buf        = (unsigned long)tx_data,
		.rx_buf        = (unsigned long)rx_data,
		.len           = size,
		.speed_hz      = g_spi_speed,
		.delay_usecs   = g_spi_delay,
		.bits_per_word = g_spi_bits_per_word,
		.cs_change     = g_spi_cs_polarity,
	};

	/* transfer data */
	if (ioctl(g_spi_fd, SPI_IOC_MESSAGE(1), &msg) == -1) {
		perror("ioctl");
		return E_OBJ;
	}

	return E_OK;
}

/**
 * @brief SPI Mode Setting
 *
 * @param [in]	mode	SPI mode
 *		@arg SPI_MODE_0		CPOL: positive logic, CPHA: positive edge
 *		@arg SPI_MODE_1		CPOL: positive logic, CPHA: negative edge
 *		@arg SPI_MODE_2		CPOL: negative logic, CPHA: positive edge
 *		@arg SPI_MODE_3		CPOL: negative logic, CPHA: negative edge
 *
 * @retval E_OK		success
 * @retval E_PAR	failure (parameter error)
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiSpiSetSpiMode(uint8_t mode)
{
	/* parameter check */
	if (!M_CHECK_MODE(mode)) {
		return E_PAR;
	}

	/* set SPI mode for read-direction */
	if (ioctl(g_spi_fd, SPI_IOC_RD_MODE, &g_spi_mode) == -1) {
		perror("ioctl");
		return E_OBJ;
	}

	/* set SPI mode for write-direction */
	if (ioctl(g_spi_fd, SPI_IOC_WR_MODE, &g_spi_mode) == -1) {
		perror("ioctl");
		return E_OBJ;
	}

	return E_OK;
}

/**
 * @brief SPI Transfer Speed Setting
 *
 * @param [in]	speed	transfer speed
 *
 * @retval E_OK		success
 */
int8_t rpiSpiSetSpeed(uint32_t speed)
{
	g_spi_speed = speed;
	return E_OK;
}

/**
 * @brief SPI Transfer Delay Time Setting
 *
 * @param [in]	delay	transfer delay time (usec)
 *
 * @retval E_OK		success
 */
int8_t rpiSpiSetDelay(uint16_t delay)
{
	g_spi_delay = delay;
	return E_OK;
}

/**
 * @brief SPI Bits per Word Setting
 *
 * @param [in]	len		bit length per a word
 *
 * @retval E_OK		success
 */
int8_t rpiSpiSetBitsPerWord(uint8_t len)
{
	g_spi_bits_per_word = len;
	return E_OK;
}

/**
 * @brief SPI CS Polarity Setting
 *
 * @param [in]	pol		CS polarity
 *		@arg D_SPI_CS_POS_LOGIC		positive logic
 *		@arg D_SPI_CS_NEG_LOGIC		negative logic
 *
 * @retval E_OK		success
 */
int8_t rpiSpiSetCsPolarity(uint8_t pol)
{
	g_spi_cs_polarity = pol;
	return E_OK;
}
