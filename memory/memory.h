#ifndef MEMORY_H
#define MEMORY_H
#include "../utils/utils.h"

typedef struct memory{

    uint8_t bytes[0x10000];

}mem;

mem* mem_init();

uint8_t mem_read_byte(mem* memory, uint16_t address);

void mem_write_byte(mem* memory, uint16_t address, uint8_t value);

int load_rom_in_memory(mem* memory,char* path,uint16_t offset);

#endif