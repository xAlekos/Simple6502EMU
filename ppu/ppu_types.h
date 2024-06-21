#ifndef PPU_TYPES_H
#define PPU_TYPES_H

#include "../cartridge/cartridge_types.h"

typedef struct ppu{

    uint8_t name_table[2][1024]; //2 Name Table da 1 KB
    uint8_t palette_table[32];
    cartridge* cart;

}ppu;

#endif