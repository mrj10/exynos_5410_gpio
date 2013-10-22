#include <stdio.h>

#include "exynos_5410_gpio.h"

int main(int argc, char *argv[]) {
	exynos_5410_gpio_init();
	for(int i = 13; i <= 27; i++) {
		printf("Pin %d = %u\n", i, odroid_xu_gpio_read(i));
	}
	exynos_5410_gpio_destroy();
	return 0;
}
