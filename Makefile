PROJECT = exynos_5410_gpio.a
OBJECTS = exynos_5410_gpio.o
CFLAGS  = -std=c99 -Wall -pedantic -Werror -O3

all: $(PROJECT) odroid_xu_test

.c.o:
	gcc -c $(CFLAGS) $<

odroid_xu_test: odroid_xu_test.c $(PROJECT)
	gcc $(CFLAGS) $^ -o $@

$(PROJECT): $(OBJECTS)
	ar rcs $@ $^
