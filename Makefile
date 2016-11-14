CC      = gcc
CFLAGS  = -Wall -O2
RM      = rm -rf

INCLUDE = -I./include
OBJS    = ./src/rpi_clkgen.o ./src/rpi_i2c.o ./src/rpi_spi.o ./src/rpi_regmap.o
DOCS    = ./doc

.SUFFIXES: .c .o

.PHONY: all clean doc

all: $(OBJS)

clean:
	$(RM) $(OBJS) $(DOCS)

doc:
	doxygen ./Doxyfile

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
