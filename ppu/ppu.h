#ifndef PPU_H
#define PPU_H

#include "../system/system_types.h"
#include "../utils/utils.h"

#include "ppu_types.h"


void clock();

ppu* ppu_init();

void ppu_connect_system(ppu* ppu_ctx, nes_system* a);

void ppu_connect_cartridge(ppu* ppu_ctx, cartridge* cart);

uint8_t ppu_read_byte(uint16_t address);

void ppu_write_byte(uint16_t address,uint8_t value);

uint8_t ppu_read_cpu(uint16_t address);

uint8_t ppu_write_cpu(uint16_t address, uint8_t value);

#endif