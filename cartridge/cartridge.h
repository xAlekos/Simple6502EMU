#include "cartridge_types.h"

cartridge* cart_init();

uint8_t cpu_read(uint16_t addr);

void cpu_write(uint16_t addr, uint8_t value);

uint8_t ppu_read(uint16_t addr);

void ppu_write(uint16_t addr, uint8_t value);