PROJECT = exynos_5410_gpio.a
OBJECTS = exynos_5410_gpio.o
CFLAGS  = -std=c99 -Wall -pedantic -Werror -O3 

all: $(PROJECT)

.c.o:
	gcc -c $(CFLAGS) $<

$(PROJECT): $(OBJECTS)
	ar rcs $@ $^
