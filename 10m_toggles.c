#include <stdio.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "exynos_5410_gpio.h"
#include "armpmu_lib.h"

#define EXYNOS_5410_GPIO_REG_BASE_ADDR 0x13400000
// For a given bank of GPIOs, the registers are laid out in memory as: CON DATA UPD DRIVESTR
#define EXYNOS_GPIO_CON_REG_OFFSET 0 //CON (data direction) register
#define EXYNOS_GPIO_DATA_REG_OFFSET 4 // Data register
#define EXYNOS_GPIO_UPD_REG_OFFSET 8 //UPD (pullup/pulldown) register
#define EXYNOS_GPIO_DRIVE_STRENGTH_REG_OFFSET 12 //Drive strength register

#define DISABLE_PULLUP_PULLDOWN 0x0 // No PU or PD
#define ENABLE_PULLUP 0x1 // PU resistor
#define ENABLE_PULLDOWN 0x2 // PD resistor

void print_all_gpio_info() {
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
}

int main(int argc, char *argv[]) {
	//print_all_gpio_info();
	odroid_xu_gpio_setup_all_output();
	//print_all_gpio_info();
	//Method 1 -- Using the library's helper functions, ~20 seconds
	//for(int i = 0; i < 10000000; i++)
	//	odroid_xu_gpio_toggle(27);

	//Method 2 -- Write 0 and 1 directly to avoid register read, ~13 seconds
	//odroid_xu_gpio_write(27, 1);
	//for(int i = 0; i < 5000000; i++) {
	//	odroid_xu_gpio_write(27, 0);
	//	odroid_xu_gpio_write(27, 1);
	//}

	//Method 3 -- Use specialized library function for pin 27 in particular, ~13 seconds
        //odroid_xu_gpio_write_27(1);
        //for(int i = 0; i < 5000000; i++) {
        //      odroid_xu_gpio_write_27(0);
        //      odroid_xu_gpio_write_27(1);
        //}

	//Method 4 -- Memoizing the offset and other bits in the register and just performing register writes, ~5 seconds
	//unsigned int regval = exynos_5410_gpio_read_raw_reg(0x0C60 + EXYNOS_GPIO_DATA_REG_OFFSET);
	for(int i = 0; i < 5000000; i++) {
	 	exynos_5410_gpio_write_raw_reg(0x0C60 + EXYNOS_GPIO_DATA_REG_OFFSET, regval ^ (1U << 1));
	 	exynos_5410_gpio_write_raw_reg(0x0C60 + EXYNOS_GPIO_DATA_REG_OFFSET, regval);
	}

	//Method 5 -- Eliminate library call altogether, ~5 seconds
#if 0
	unsigned int regval1 = exynos_5410_gpio_read_raw_reg(0x0C60 + EXYNOS_GPIO_DATA_REG_OFFSET);
	unsigned int regval2 = regval1 ^ (1U << 1);
	volatile unsigned int *dataregaddr = (volatile unsigned int *)((unsigned int)exynos_5410_gpio_get_map_base() + 0x0C60 + EXYNOS_GPIO_DATA_REG_OFFSET);
	//uint32_t t1, t2 = 0;
	for(int i = 0; i < 500000; i++) {
		//t1 = rdtsc32();
		//if(t2 != 0)
		//	printf("iteration %d, %" PRIu32 " cycles, %f cycles/write\n", i, t2-t1, (float)((t2-t1)/16));
		//t2 = t1;
		*dataregaddr = regval1;
		*dataregaddr = regval2;
		*dataregaddr = regval1;
		*dataregaddr = regval2;
		*dataregaddr = regval1;
		*dataregaddr = regval2;
		*dataregaddr = regval1;
		*dataregaddr = regval2;
		*dataregaddr = regval1;
		*dataregaddr = regval2;
		*dataregaddr = regval1;
		*dataregaddr = regval2;
		*dataregaddr = regval1;
		*dataregaddr = regval2;
		*dataregaddr = regval1;
		*dataregaddr = regval2;
		*dataregaddr = regval1;
		*dataregaddr = regval2;
		*dataregaddr = regval1;
		*dataregaddr = regval2;
	}
#endif
	return 0;
}
