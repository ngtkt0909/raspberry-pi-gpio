/**
 * @file		rpi_i2c.c
 * @brief		I2C Library Implementation
 *
 * @author		T. Ngtk
 * @copyright	Copyright (c) 2016 T. Ngtk
 *
 * @par License
 *	Released under the MIT License.<BR>
 *	https://github.com/ngtkt0909/raspberry-pi-gpio/blob/master/LICENSE
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "rpi_common.h"
#include "rpi_i2c.h"

/*------------------------------------------------------------------------------
	Defined Macros
------------------------------------------------------------------------------*/
#define D_FD_NOT_OPENED			(-1)		/**< file descriptor (not opened) */
#define D_DEFAULT_SLAVE_ADDR	(0x00U)		/**< default slave address */

/*------------------------------------------------------------------------------
	Global Variables
------------------------------------------------------------------------------*/
static int g_i2c_fd = D_FD_NOT_OPENED;		/**< file descriptor */

/*------------------------------------------------------------------------------
	Functions
------------------------------------------------------------------------------*/
/**
 * @brief I2C Port Open
 *
 * @param [in]	dev_path	string of path for device node (e.g. "/dev/i2c-1")
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiI2cOpen(uint8_t *dev_path)
{
	/* check parameter */
	assert(dev_path != NULL);

	/* check port */
	assert(g_i2c_fd == D_FD_NOT_OPENED);

	/* open SPI port */
	if ((g_i2c_fd = open((const char *)dev_path, O_RDWR)) == -1) {
		perror("open");
		return E_OBJ;
	}

	/* set default slave address */
	if (rpiI2cSetSlave(D_DEFAULT_SLAVE_ADDR) != E_OK) {
		fprintf(stderr, "rpiI2cSetSlave failed\n");
		return E_OBJ;
	}

	return E_OK;
}

/**
 * @brief I2C Port Close
 *
 * @param Nothing
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiI2cClose()
{
	/* check port */
	assert(g_i2c_fd != D_FD_NOT_OPENED);

	/* close SPI port */
	if (close(g_i2c_fd) == -1) {
		perror("close");
		return E_OBJ;
	}

	/* clear file descriptor */
	g_i2c_fd = D_FD_NOT_OPENED;

	return E_OK;
}

/**
 * @brief I2C Slave Address Setting
 *
 * @param [in]	slave_addr	slave address of connected device
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiI2cSetSlave(uint8_t slave_addr)
{
	/* set slave address */
	if (ioctl(g_i2c_fd, I2C_SLAVE, &slave_addr) == -1) {
		perror("ioctl");
		return E_OBJ;
	}

	return E_OK;
}

/**
 * @brief I2C Data Write
 *
 * @param [in]	cmd		command for slave device
 * @param [in]	data	write data
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiI2cWrite(uint8_t cmd, uint8_t data)
{
	uint8_t buf_tx[2];

	/* write command + master data */
	buf_tx[0] = cmd;
	buf_tx[1] = data;
	if (write(g_i2c_fd, buf_tx, 2) != 2) {
		perror("write");
		return E_OBJ;
	}

	return E_OK;
}

/**
 * @brief I2C Data Read
 *
 * @param [in]	cmd		command for slave device
 * @param [out]	data	address of read data
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiI2cRead(uint8_t cmd, uint8_t *data)
{
	/* check parameter */
	assert(data != NULL);

	/* write command */
	if (write(g_i2c_fd, &cmd, 1) != 1) {
		perror("write");
		return E_OBJ;
	}

	/* read slave data */
	if (read(g_i2c_fd, data, 1) != 1) {
		perror("read");
		return E_OBJ;
	}

	return E_OK;
}

/**
 * @brief I2C Block Data Write
 *
 * @param [in]	cmd		command for slave device
 * @param [in]	buf		address of write data buffer
 * @param [in]	size	buffer size (byte)
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiI2cWriteBlock(uint8_t cmd, uint8_t *buf, uint32_t size)
{
	uint8_t *buf_tx;

	/* check parameter */
	assert(buf != NULL);

	/* allocate buffer */
	if ((buf_tx = (uint8_t *)malloc(sizeof(uint8_t) * (size + 1))) == NULL) {
		perror("malloc");
		return E_OBJ;
	}

	/* write command + master data */
	buf_tx[0] = cmd;
	memcpy(&buf_tx[1], buf, size);
	if (write(g_i2c_fd, buf_tx, size + 1) != size + 1) {
		perror("write");
		return E_OBJ;
	}

	/* free buffer */
	free(buf_tx);

	return E_OK;
}

/**
 * @brief I2C Block Data Read
 *
 * @param [in]	cmd		command for slave device
 * @param [out]	buf		address of read data buffer
 * @param [in]	size	buffer size (byte)
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiI2cReadBlock(uint8_t cmd, uint8_t *buf, uint32_t size)
{
	/* check parameter */
	assert(buf != NULL);

	/* write command */
	if (write(g_i2c_fd, &cmd, 1) != 1) {
		perror("write");
		return E_OBJ;
	}

	/* read slave data */
	if (read(g_i2c_fd, buf, size) != size) {
		perror("read");
		return E_OBJ;
	}

	return E_OK;
}
