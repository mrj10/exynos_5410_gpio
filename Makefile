PROJECT = exynos_5410_gpio.a
OBJECTS = exynos_5410_gpio.o
CFLAGS  = -std=gnu99 -Wall -pedantic -Werror -O3 -mcpu=cortex-a15 -mfpu=neon-vfpv4 -I../enable_arm_pmu

all: $(PROJECT) odroid_xu_test 10m_toggles

.c.o:
	gcc -c $(CFLAGS) $<

odroid_xu_test: odroid_xu_test.c $(PROJECT)
	gcc $(CFLAGS) $^ -o $@

10m_toggles: 10m_toggles.c $(PROJECT)
	gcc $(CFLAGS) $^ -o $@

$(PROJECT): $(OBJECTS)
	ar rcs $@ $^

clean:
	rm $(PROJECT) odroid_xu_test $(OBJECTS)
