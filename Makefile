PROJECT = exynos_5410_gpio.a
OBJECTS = exynos_5410_gpio.o
CFLAGS  = -std=c99 -Wall -pedantic -Werror -O3 -mcpu=cortex-a15 -mfpu=neon-vfpv4

all: $(PROJECT) odroid_xu_test

.c.o:
	gcc -c $(CFLAGS) $<

odroid_xu_test: odroid_xu_test.c $(PROJECT)
	gcc $(CFLAGS) $^ -o $@

$(PROJECT): $(OBJECTS)
	ar rcs $@ $^

clean:
	rm $(PROJECT) odroid_xu_test $(OBJECTS)
