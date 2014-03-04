#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include "exynos_5410_gpio.h"

#define EXYNOS_5410_GPIO_REG_BASE_ADDR 0x13400000
// For a given bank of GPIOs, the registers are laid out in memory as: CON DATA UPD DRIVESTR
#define EXYNOS_GPIO_CON_REG_OFFSET 0 //CON (data direction) register
#define EXYNOS_GPIO_DATA_REG_OFFSET 4 // Data register
#define EXYNOS_GPIO_UPD_REG_OFFSET 8 //UPD (pullup/pulldown) register
#define EXYNOS_GPIO_DRIVE_STRENGTH_REG_OFFSET 12 //Drive strength register

#define DISABLE_PULLUP_PULLDOWN 0x0 // No PU or PD
#define ENABLE_PULLDOWN 0x1 // PD resistor
#define ENABLE_PULLUP 0x2 // PU resistor

#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
__LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

static int exynos_5410_gpio_initialized = 0;
static void *exynos_5410_gpio_mapbase;

//Caller should avoid calling this if exynos_5410_gpio_initialized is 1.
void exynos_5410_gpio_init() {
  int fd;

  if(exynos_5410_gpio_initialized != 0) FATAL;

  if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) FATAL;

  exynos_5410_gpio_mapbase = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, EXYNOS_5410_GPIO_REG_BASE_ADDR & ~MAP_MASK);
  if(exynos_5410_gpio_mapbase == (void *) -1) FATAL;

  close(fd);
  exynos_5410_gpio_initialized = 1;
}

void exynos_5410_gpio_destroy() {
  if(exynos_5410_gpio_initialized != 1) FATAL;
  if(munmap(exynos_5410_gpio_mapbase, MAP_SIZE) == -1) FATAL;
  exynos_5410_gpio_initialized = 0;
}

// Read a raw register value (from the mapped page)
unsigned int exynos_5410_gpio_read_raw_reg(unsigned int offset) {
	volatile uint32_t *addr = (volatile uint32_t *)((char *)exynos_5410_gpio_mapbase + offset);
	return *addr;
}

// Write a raw register value (from the mapped page)
void exynos_5410_gpio_write_raw_reg(unsigned int offset, unsigned int data) {
	volatile uint32_t *addr = (volatile uint32_t *)((char *)exynos_5410_gpio_mapbase + offset);
	*addr = data;
}

// Return the base address of the mapped region
void *exynos_5410_gpio_get_map_base() {
	return exynos_5410_gpio_mapbase;
}

// bank_base is the address of the first register for the GPIO bank (CON)
// the 8 lowest bits of bitmask determine which bits in the bank are affected by this call
// output should be 1 to make all affected bits outputs, 0 to make them all inputs
// pullup should be 0 to have no pullup or pulldown for these bits, 1 to have a pulldown, and 2 to have a pullup
void exynos_5410_gpio_setup_pin(unsigned int bank_offset, unsigned int bitmask, unsigned int output, unsigned int pullup) {
	if(exynos_5410_gpio_initialized != 1) FATAL;
	if(!(pullup == 0U || pullup == 1U || pullup == 2U)) FATAL;
	if(!(output == 0 || output == 1)) FATAL;

	volatile uint32_t *addr = (volatile uint32_t *)((char *)exynos_5410_gpio_mapbase + bank_offset); //CON register
	uint32_t con_val = *addr;
	int i;
	for(i = 0; i < 8; i++) {
		if(bitmask & (1U << i)) {
			con_val &= ~(0xFU << (4*i));
			con_val |= (output << (4*i));
		}
	}
	*addr = con_val;
	addr += (EXYNOS_GPIO_UPD_REG_OFFSET / sizeof(*addr)); //Point to UPD register
	uint32_t upd_val = *addr;
	for(i = 0; i < 8; i++) {
		if(bitmask & (1U << i)) {
			upd_val &= ~(0x3U << (2*i));
			upd_val |= (pullup << (2*i));
		}
	}
	*addr = upd_val;
}

unsigned int exynos_5410_gpio_read(unsigned int bank_offset) {
	volatile uint32_t *addr = (volatile uint32_t *)((char *)exynos_5410_gpio_mapbase + bank_offset + EXYNOS_GPIO_DATA_REG_OFFSET); //DATA register
	return *addr;
}

//Only write to the bits indicated by mask (performs RMW)
void exynos_5410_gpio_write_mask(unsigned int bank_offset, unsigned int data, unsigned int mask) {
	volatile uint32_t *addr = (volatile uint32_t *)((char *)exynos_5410_gpio_mapbase + bank_offset + EXYNOS_GPIO_DATA_REG_OFFSET); //DATA register
	uint32_t val = *addr;
	val &= ~mask;
	data &= mask; //TODO don't need to do this if we assume data has unused bits zeroed out.
	val |= data;
	*addr = val;
}

//Writes to all bits (no RMW)
void exynos_5410_gpio_write(unsigned int bank_offset, unsigned int data) {
	volatile uint32_t *addr = (volatile uint32_t *)((char *)exynos_5410_gpio_mapbase + bank_offset + EXYNOS_GPIO_DATA_REG_OFFSET); //DATA register
	*addr = data;
}

static inline void odroid_xu_pin_lookup(unsigned int pin_index, unsigned int *bank_offset, unsigned int *bit_index) {
	switch(pin_index) {
		#define EXYNOS_5410_GPIO_PIN(pin_num, offset, bit_num) case pin_num: *bank_offset = offset; *bit_index = bit_num; break;
		EXYNOS_5410_GPIO_PINS
		#undef EXYNOS_5410_GPIO_PIN
		default: FATAL; break;
	}
}

//Read a single bit according to the pin numbers on the ODROID-XU/ODROID-XU+E CON10 connector
unsigned int odroid_xu_gpio_read(unsigned int pin_index) {
	unsigned int bank_offset, bit_index;
	odroid_xu_pin_lookup(pin_index, &bank_offset, &bit_index);
	return (exynos_5410_gpio_read(bank_offset) >> bit_index) & 0x1;
}

//Write a single bit according to the pin numbers on the ODROID-XU/ODROID-XU+E CON10 connector (only the lsb of data is used)
void odroid_xu_gpio_write(unsigned int pin_index, unsigned int data) {
	unsigned int bank_offset, bit_index;
	odroid_xu_pin_lookup(pin_index, &bank_offset, &bit_index);
	exynos_5410_gpio_write_mask(bank_offset, ((data & 1U) << bit_index), (1U << bit_index));
}

void odroid_xu_gpio_setup_all_output() {
	unsigned int bank_offset, bit_index;
	for(int i = 13; i <= 27; i++) {
		odroid_xu_pin_lookup(i, &bank_offset, &bit_index);
		exynos_5410_gpio_setup_pin(bank_offset, 1U << bit_index, 1 /*output*/, 0 /*no pullup/pulldown*/);
	}
}

//pullup = 0 for no pullup/pulldown, 1 for pulldown, 2 for pullup
void odroid_xu_gpio_setup_all_input(unsigned int pullup) {
	unsigned int bank_offset, bit_index;
	for(int i = 13; i <= 27; i++) {
		odroid_xu_pin_lookup(i, &bank_offset, &bit_index);
		exynos_5410_gpio_setup_pin(bank_offset, 1U << bit_index, 0 /*input*/, pullup);
	}
}

void odroid_xu_gpio_toggle(unsigned int pin_index) {
	unsigned int bank_offset, bit_index;
	odroid_xu_pin_lookup(pin_index, &bank_offset, &bit_index);
	unsigned int d = exynos_5410_gpio_read(bank_offset);
	d ^= (1U << bit_index);
	exynos_5410_gpio_write_mask(bank_offset, d, (1U << bit_index));
}

//Create specialized functions for each pin to avoid the pin_lookup() call
#define EXYNOS_5410_GPIO_PIN(pin_num, offset, bit_num) void EXYNOS_5410_GPIO_CAT(odroid_xu_gpio_toggle_, pin_num) () \
	unsigned int bank_offset = offset; \
	unsigned int bit_index = bit_num; \
	unsigned int d = exynos_5410_gpio_read(bank_offset); \
	d ^= (1U << bit_index); \
	exynos_5410_gpio_write_mask(bank_offset, d, (1U << bit_index)); \
}
EXYNOS_5410_GPIO_PINS
#undef EXYNOS_5410_GPIO_PIN

#define EXYNOS_5410_GPIO_PIN(pin_num, offset, bit_num) unsigned int EXYNOS_5410_GPIO_CAT(odroid_xu_gpio_read_, pin_num) () \
	unsigned int bank_offset = offset; \
	unsigned int bit_index = bit_num; \
	return (exynos_5410_gpio_read(bank_offset) >> bit_index) & 0x1; \
}
EXYNOS_5410_GPIO_PINS
#undef EXYNOS_5410_GPIO_PIN

#define EXYNOS_5410_GPIO_PIN(pin_num, offset, bit_num) void EXYNOS_5410_GPIO_CAT(odroid_xu_gpio_write_, pin_num) (unsigned int data) \
	unsigned int bank_offset = offset; \
	unsigned int bit_index = bit_num; \
	exynos_5410_gpio_write_mask(bank_offset, ((data & 1U) << bit_index), (1U << bit_index)); \
}
EXYNOS_5410_GPIO_PINS
#undef EXYNOS_5410_GPIO_PIN
