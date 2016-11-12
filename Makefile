CC      = gcc
CFLAGS  = -Wall -O2
RM      = rm -rf

INCLUDE = -I./include
OBJS    = ./src/rpi_clkgen.o ./src/rpi_gpio.o ./src/rpi_i2c.o ./src/rpi_spi.o

.SUFFIXES: .c .o

.PHONY: all clean doc

all: $(OBJS)

clean:
	$(RM) $(OBJS)

doc:
	doxygen ./Doxyfile

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
