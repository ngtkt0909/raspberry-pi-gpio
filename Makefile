CC      = gcc
CFLAGS  = -Wall -O2
RM      = rm -rf

INCLUDE = -I./include
OBJS    = ./src/rpi_gpio.o ./src/rpi_i2c.o ./src/rpi_spi.o

.SUFFIXES: .c .o

.PHONY: all
all: $(OBJS)

.PHONY: clean
clean:
	$(RM) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
