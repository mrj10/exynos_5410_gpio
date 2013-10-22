#include <stdio.h>

#include "exynos_5410_gpio.h"

#define EXYNOS_5410_GPIO_REG_BASE_ADDR 0x13400000
// For a given bank of GPIOs, the registers are laid out in memory as: CON DATA UPD DRIVESTR
#define EXYNOS_GPIO_CON_REG_OFFSET 0 //CON (data direction) register
#define EXYNOS_GPIO_DATA_REG_OFFSET 4 // Data register
#define EXYNOS_GPIO_UPD_REG_OFFSET 8 //UPD (pullup/pulldown) register
#define EXYNOS_GPIO_DRIVE_STRENGTH_REG_OFFSET 12 //Drive strength register

#define DISABLE_PULLUP_PULLDOWN 0x0 // No PU or PD
#define ENABLE_PULLUP 0x1 // PU resistor
#define ENABLE_PULLDOWN 0x2 // PD resistor

int main(int argc, char *argv[]) {
	exynos_5410_gpio_init();
	printf("0x0C20 bank CON register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C20 + EXYNOS_GPIO_CON_REG_OFFSET));
	printf("0x0C20 bank DATA register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C20 + EXYNOS_GPIO_DATA_REG_OFFSET));
	printf("0x0C20 bank UPD register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C20 + EXYNOS_GPIO_UPD_REG_OFFSET));
	printf("0x0C20 bank DRIVE_STRENGTH register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C20 + EXYNOS_GPIO_DRIVE_STRENGTH_REG_OFFSET));
	printf("0x0C40 bank CON register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C40 + EXYNOS_GPIO_CON_REG_OFFSET));
	printf("0x0C40 bank DATA register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C40 + EXYNOS_GPIO_DATA_REG_OFFSET));
	printf("0x0C40 bank UPD register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C40 + EXYNOS_GPIO_UPD_REG_OFFSET));
	printf("0x0C40 bank DRIVE_STRENGTH register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C40 + EXYNOS_GPIO_DRIVE_STRENGTH_REG_OFFSET));
	printf("0x0C60 bank CON register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C60 + EXYNOS_GPIO_CON_REG_OFFSET));
	printf("0x0C60 bank DATA register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C60 + EXYNOS_GPIO_DATA_REG_OFFSET));
	printf("0x0C60 bank UPD register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C60 + EXYNOS_GPIO_UPD_REG_OFFSET));
	printf("0x0C60 bank DRIVE_STRENGTH register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C60 + EXYNOS_GPIO_DRIVE_STRENGTH_REG_OFFSET));
	for(int i = 13; i <= 27; i++) {
		printf("Pin %d = %u\n", i, odroid_xu_gpio_read(i));
	}

	odroid_xu_gpio_setup_all_output();

	printf("0x0C20 bank CON register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C20 + EXYNOS_GPIO_CON_REG_OFFSET));
	printf("0x0C20 bank DATA register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C20 + EXYNOS_GPIO_DATA_REG_OFFSET));
	printf("0x0C20 bank UPD register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C20 + EXYNOS_GPIO_UPD_REG_OFFSET));
	printf("0x0C20 bank DRIVE_STRENGTH register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C20 + EXYNOS_GPIO_DRIVE_STRENGTH_REG_OFFSET));
	printf("0x0C40 bank CON register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C40 + EXYNOS_GPIO_CON_REG_OFFSET));
	printf("0x0C40 bank DATA register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C40 + EXYNOS_GPIO_DATA_REG_OFFSET));
	printf("0x0C40 bank UPD register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C40 + EXYNOS_GPIO_UPD_REG_OFFSET));
	printf("0x0C40 bank DRIVE_STRENGTH register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C40 + EXYNOS_GPIO_DRIVE_STRENGTH_REG_OFFSET));
	printf("0x0C60 bank CON register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C60 + EXYNOS_GPIO_CON_REG_OFFSET));
	printf("0x0C60 bank DATA register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C60 + EXYNOS_GPIO_DATA_REG_OFFSET));
	printf("0x0C60 bank UPD register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C60 + EXYNOS_GPIO_UPD_REG_OFFSET));
	printf("0x0C60 bank DRIVE_STRENGTH register = 0x%08X\n", exynos_5410_gpio_read_raw_reg(0x0C60 + EXYNOS_GPIO_DRIVE_STRENGTH_REG_OFFSET));

	for(int i = 13; i <= 27; i++) {
		printf("Pin %d = %u\n", i, odroid_xu_gpio_read(i));
	}

	odroid_xu_gpio_toggle(27);

	printf("Toggled pin 27\n");
	for(int i = 13; i <= 27; i++) {
		printf("Pin %d = %u\n", i, odroid_xu_gpio_read(i));
	}

	exynos_5410_gpio_destroy();
	return 0;
}
