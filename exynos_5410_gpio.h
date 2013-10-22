#ifndef __EXYNOS_5410_GPIO_H__
#define __EXYNOS_5410_GPIO_H__

//Caller should avoid calling this if exynos_5410_gpio_initialized is 1.
void exynos_5410_gpio_init();
void exynos_5410_gpio_destroy();

// Useful only for library developers, returns a raw register value (from the mapped page)
unsigned int exynos_5410_gpio_read_raw_reg(unsigned int offset);

// bank_base is the address of the first register for the GPIO bank (CON)
// the 8 lowest bits of bitmask determine which bits in the bank are affected by this call
// output should be 1 to make all affected bits outputs, 0 to make them all inputs
// pullup should be 0 to have no pullup or pulldown for these bits, 1 to have a pullup, and 2 to have a pulldown
void exynos_5410_gpio_setup_pin(unsigned int bank_offset, unsigned int bitmask, unsigned int output, unsigned int pullup);
unsigned int exynos_5410_gpio_read(unsigned int bank_offset);
//Only write to the bits indicated by mask (performs RMW)
void exynos_5410_gpio_write_mask(unsigned int bank_offset, unsigned int data, unsigned int mask);
//Writes to all bits (no RMW)
void exynos_5410_gpio_write(unsigned int bank_offset, unsigned int data);
//Read a single bit according to the pin numbers on the ODROID-XU/ODROID-XU+E CON10 connector
unsigned int odroid_xu_gpio_read(unsigned int pin_index);
//Write a single bit according to the pin numbers on the ODROID-XU/ODROID-XU+E CON10 connector (only the lsb of data is used)
void odroid_xu_gpio_write(unsigned int pin_index, unsigned int data);

void odroid_xu_gpio_setup_all_output();
//pullup = 0 for no pullup/pulldown, 1 for pullup, 2 for pulldown
void odroid_xu_gpio_setup_all_input(unsigned int pullup);


#endif //#ifndef __EXYNOS_5410_GPIO_H__
