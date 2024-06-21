#ifndef SYSTEM_H
#define SYSTEM_H

#include "../utils/utils.h"
#include "system_types.h"
#include "../ppu/ppu.h"
#include "../cpu/cpu.h"

mem* cpu_mem_init();
nes_system* system_init();
uint8_t mem_read_byte(mem* memory, uint16_t address);
void mem_write_byte(mem* memory, uint16_t address, uint8_t value);
int load_rom_in_memory(mem* memory, char* path, uint16_t offset);


void load_cartridge();

#endif // SYSTEM_H
