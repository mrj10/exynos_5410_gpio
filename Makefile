PROJECT = exynos_5410_gpio.a
OBJECTS = exynos_5410_gpio.o
CFLAGS  = -Wall -pedantic -Werror -O3 -mcpu=cortex-a15

all: $(PROJECT)

.c.o:
	gcc -c $(CFLAGS) $<

$(PROJECT): $(OBJECTS)
	libtool -o $(PROJECT) -static $(OBJECTS)
