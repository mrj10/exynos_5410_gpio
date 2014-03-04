#ifndef __EXYNOS_5410_GPIO_H__
#define __EXYNOS_5410_GPIO_H__

//X-macro consisting of (CON10 pin number,
//                       data register offset from EXYNOS_5410_GPIO_REG_BASE_ADDR,
//						 data register bit offset) tuples
#define EXYNOS_5410_GPIO_PINS \
	EXYNOS_5410_GPIO_PIN(13, 0x0C20, 5) \
	EXYNOS_5410_GPIO_PIN(14, 0x0C40, 3) \
	EXYNOS_5410_GPIO_PIN(15, 0x0C20, 2) \
	EXYNOS_5410_GPIO_PIN(16, 0x0C20, 0) \
	EXYNOS_5410_GPIO_PIN(17, 0x0C20, 6) \
	EXYNOS_5410_GPIO_PIN(18, 0x0C20, 3) \
	EXYNOS_5410_GPIO_PIN(19, 0x0C40, 6) \
	EXYNOS_5410_GPIO_PIN(20, 0x0C40, 4) \
	EXYNOS_5410_GPIO_PIN(21, 0x0C40, 5) \
	EXYNOS_5410_GPIO_PIN(22, 0x0C40, 7) \
	EXYNOS_5410_GPIO_PIN(23, 0x0C40, 2) \
	EXYNOS_5410_GPIO_PIN(24, 0x0C40, 1) \
	EXYNOS_5410_GPIO_PIN(25, 0x0C20, 7) \
	EXYNOS_5410_GPIO_PIN(26, 0x0C40, 0) \
	EXYNOS_5410_GPIO_PIN(27, 0x0C60, 1)

#define EXYNOS_5410_GPIO_CATHELPER(x, y) x ## y
#define EXYNOS_5410_GPIO_CAT(x, y) EXYNOS_5410_GPIO_CATHELPER(x, y)

//Caller should avoid calling this if exynos_5410_gpio_initialized is 1.
void exynos_5410_gpio_init();
void exynos_5410_gpio_destroy();

// Useful only for library developers, returns a raw register value (from the mapped page)
unsigned int exynos_5410_gpio_read_raw_reg(unsigned int offset);
// Write a raw register value (from the mapped page)
void exynos_5410_gpio_write_raw_reg(unsigned int offset, unsigned int data);
// Return the base address of the mapped region
void *exynos_5410_gpio_get_map_base();

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
void odroid_xu_gpio_toggle(unsigned int pin_index);

#define EXYNOS_5410_GPIO_PIN(pin_num, offset, bit_num) void EXYNOS_5410_GPIO_CAT(odroid_xu_gpio_toggle_, pin_num) ## ();
EXYNOS_5410_GPIO_PINS
#undef EXYNOS_5410_GPIO_PIN

#define EXYNOS_5410_GPIO_PIN(pin_num, offset, bit_num) unsigned int EXYNOS_5410_GPIO_CAT(odroid_xu_gpio_read_, pin_num) ## pin_num ## ();
EXYNOS_5410_GPIO_PINS
#undef EXYNOS_5410_GPIO_PIN

#define EXYNOS_5410_GPIO_PIN(pin_num, offset, bit_num) void EXYNOS_5410_GPIO_CAT(odroid_xu_gpio_toggle_write_, pin_num) ## (unsigned int data);
EXYNOS_5410_GPIO_PINS
#undef EXYNOS_5410_GPIO_PIN

#endif //#ifndef __EXYNOS_5410_GPIO_H__
