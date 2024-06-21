#ifndef SYSTEM_TYPES_H
#define SYSTEM_TYPES_H

#include "../utils/utils.h"
#include "../cpu/cpu_types.h"
#include "../ppu/ppu_types.h"
#include "../cartridge/cartridge_types.h"

typedef struct cpu_ram {

    uint8_t bytes[2048];
    
} mem;

typedef struct sys {
    
    mem* cpu_ram;
    ppu* ppu_ctx;
    cartridge* rom;
    cpu* ctx; 

}nes_system;

#endif 