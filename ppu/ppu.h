#include "../utils/utils.h"
#include "ppu_types.h"

uint8_t ppu_read_byte(uint16_t address);

void ppu_write_byte(uint16_t address,uint8_t value);

uint8_t ppu_read_cpu(uint16_t address);

uint8_t ppu_write_cpu(uint16_t address, uint8_t value);