#ifndef SYSTEM_H
#define SYSTEM_H

#include "../utils/utils.h"
#include "system_types.h" // Inclusione delle definizioni dei tipi System
#include "../cpu/cpu_types.h" // Inclusione delle definizioni dei tipi CPU

mem* mem_init();
nes_system* system_init();
uint8_t mem_read_byte(mem* memory, uint16_t address);
void mem_write_byte(mem* memory, uint16_t address, uint8_t value);
int load_rom_in_memory(mem* memory, char* path, uint16_t offset);

#endif // SYSTEM_H
