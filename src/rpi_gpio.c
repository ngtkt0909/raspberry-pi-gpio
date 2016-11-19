/**
 * @file		rpi_gpio.c
 * @brief		GPIO Library Implementation
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
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

#include "rpi_common.h"
#include "rpi_gpio.h"

/*------------------------------------------------------------------------------
	Defined Macros
------------------------------------------------------------------------------*/
#define D_DIR_GPIO			"/sys/class/gpio/"		/**< GPIO file */
#define D_LENGTH_PIN		(3)						/**< maximum string length for pin */
#define D_LENGTH_PATH		(256)					/**< maximum string length for path */
#define D_LENGTH_VAL		(1)						/**< maximum string length for value */
#define D_DELAY_FILEGEN		(100000)				/**< delay time between declaration of pin and dir */

/** calc digit size */
#define M_DIGIT_SIZE(val)	((uint32_t)log10((double)(val)) + 1)

/*------------------------------------------------------------------------------
	Prototype Declaration
------------------------------------------------------------------------------*/
static int8_t sRpiGpioOpen(uint8_t pin, char *dir, uint8_t dir_size);
static int8_t sRpiGpioWrite(char *path, char *str, uint8_t str_size);
static int8_t sRpiGpioRead(char *path, char *str, uint8_t str_size);

/*------------------------------------------------------------------------------
	Functions (External)
------------------------------------------------------------------------------*/
/**
 * @brief GPIO Port Open (Input Direction)
 *
 * @param [in]	pin	number of GPIO pin
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiGpioOpenIn(uint8_t pin)
{
	/* declare GPIO pin as input direction */
	return sRpiGpioOpen(pin, "in", 2U);
}

/**
 * @brief GPIO Port Open (Output Direction)
 *
 * @param [in]	pin	number of GPIO pin
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiGpioOpenOut(uint8_t pin)
{
	/* declare GPIO pin as output direction */
	return sRpiGpioOpen(pin, "out", 3U);
}

/**
 * @brief GPIO Port Close
 *
 * @param [in]	pin	number of GPIO pin
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiGpioClose(uint8_t pin)
{
	int ret;
	char pin_str[D_LENGTH_PIN];

	/* check parameter */
	assert(sizeof("") + M_DIGIT_SIZE(pin) <= D_LENGTH_PIN);

	/* undeclare GPIO pin */
	sprintf(pin_str, "%d", pin);

	return sRpiGpioWrite(D_DIR_GPIO "unexport", pin_str, M_DIGIT_SIZE(pin));
}

/**
 * @brief GPIO Port Set
 *
 * @param [in]	pin	number of GPIO pin
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiGpioSet(uint8_t pin)
{
	int ret;
	char pin_str[D_LENGTH_PATH];

	/* check parameter */
	assert(sizeof(D_DIR_GPIO "gpio/value") + M_DIGIT_SIZE(pin) <= D_LENGTH_PATH);

	/* set GPIO pin */
	sprintf(pin_str, D_DIR_GPIO "gpio%d/value", pin);

	return sRpiGpioWrite(pin_str, "1", 1U);
}

/**
 * @brief GPIO Port Clear
 *
 * @param [in]	pin	number of GPIO pin
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiGpioClr(uint8_t pin)
{
	int ret;
	char path_str[D_LENGTH_PATH];

	/* check parameter */
	assert(sizeof(D_DIR_GPIO "gpio/value") + M_DIGIT_SIZE(pin) <= D_LENGTH_PATH);

	/* clear GPIO pin */
	sprintf(path_str, D_DIR_GPIO "gpio%d/value", pin);

	return sRpiGpioWrite(path_str, "0", 1U);
}

/**
 * @brief GPIO Port Get
 *
 * @param [in]	pin	number of GPIO pin
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
int8_t rpiGpioGet(uint8_t pin, int32_t *val)
{
	int ret;
	char path_str[D_LENGTH_PATH];
	char val_str[D_LENGTH_VAL];

	/* read GPIO pin */
	sprintf(path_str, D_DIR_GPIO "gpio%d/value", pin);

	ret = sRpiGpioRead(path_str, val_str, D_LENGTH_VAL);
	assert(ret == E_OK);

	/* convert string to integer */
	*val = strtol(val_str, NULL, 0);
	if ((*val == LONG_MIN) || (*val == LONG_MAX)) {
		perror("strtol");
		return E_OBJ;
	}

	return E_OK;
}

/*------------------------------------------------------------------------------
	Functions (Internal)
------------------------------------------------------------------------------*/
/**
 * @brief GPIO Port Open
 *
 * @param [in]	pin			number of GPIO pin
 * @param [in]	dir			string of direction
 *		@args "in"	input direction
 *		@args "out"	output direction
 * @param [in]	dir_size	byte-size of string direction
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
static int8_t sRpiGpioOpen(uint8_t pin, char *dir, uint8_t dir_size)
{
	int ret;
	char pin_str[D_LENGTH_PIN];
	char path_str[D_LENGTH_PATH];

	/* check parameter */
	assert(sizeof("") + M_DIGIT_SIZE(pin) <= D_LENGTH_PIN);
	assert(sizeof(D_DIR_GPIO "gpio/direction") + M_DIGIT_SIZE(pin) <= D_LENGTH_PATH);

	/* declare GPIO pin */
	sprintf(pin_str, "%d", pin);
	if ((ret = sRpiGpioWrite(D_DIR_GPIO "export", pin_str, M_DIGIT_SIZE(pin))) != E_OK) {
		return ret;
	}

	/* wait for gpio file generation */
	usleep(D_DELAY_FILEGEN);

	/* declare GPIO direction */
	sprintf(path_str, D_DIR_GPIO "gpio%d/direction", pin);
	if ((ret = sRpiGpioWrite(path_str, dir, dir_size)) != E_OK) {
		return ret;
	}

	return E_OK;
}

/**
 * @brief Write to File
 *
 * @param [in]	path		file path
 * @param [in]	str			string to be wrote
 * @param [in]	str_size	byte-size of string to be wrote
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
static int8_t sRpiGpioWrite(char *path, char *str, uint8_t str_size)
{
	int fd;
	int8_t ret = E_OK;

	/* open file path */
	if ((fd = open(path, O_WRONLY)) == -1) {
		perror("open");
		ret = E_OBJ;
	}

	/* write to the file */
	if ((ret == E_OK) && (write(fd, str, str_size) != str_size)) {
		perror("write");
		ret = E_OBJ;
	}

	/* close file path */
	if ((ret == E_OK) && (close(fd) == -1)) {
		perror("close");
		ret = E_OBJ;
	}

	return ret;
}

/**
 * @brief Read from File
 *
 * @param [in]	path		file path
 * @param [in]	str			string to be wrote
 * @param [in]	str_size	byte-size of string to be read
 *
 * @retval E_OK		success
 * @retval E_OBJ	failure (object error)
 */
static int8_t sRpiGpioRead(char *path, char *str, uint8_t str_size)
{
	int fd;
	int8_t ret = E_OK;

	/* open file path */
	if ((fd = open(path, O_RDONLY)) == -1) {
		perror("open");
		ret = E_OBJ;
	}

	/* read from the file */
	if ((ret == E_OK) && (read(fd, str, str_size) != str_size)) {
		perror("read");
		ret = E_OBJ;
	}

	/* close file path */
	if ((ret == E_OK) && (close(fd) == -1)) {
		perror("close");
		ret = E_OBJ;
	}

	return ret;
}
