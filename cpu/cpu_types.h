#ifndef CPU_TYPES_H
#define CPU_TYPES_H

#include "../utils/utils.h"

// Forward declaration per mem e sys
typedef struct cpu_ram mem;
typedef struct sys nes_system;

typedef enum flags {
    C = (1 << 0), // Carry
    Z = (1 << 1), // Zero
    I = (1 << 2), // Mask interrupt 
    D = (1 << 3), // Decimal
    B = (1 << 4), // Break
    U = (1 << 5), // Unused
    V = (1 << 6), // Overflow 
    N = (1 << 7)  // Negative
} sr_flag_t;

typedef struct cpu {
    uint8_t a, x, y;        // Registers Accumulator, X and Y
    uint16_t pc;            // Program Counter register
    uint8_t sp;             // Stack Pointer register
    uint8_t status;         // Status register (flags)
    mem* memory; 
    uint8_t cur_opcode;
    uint8_t remaining_cycles;
    uint8_t fetched;
    uint16_t abs_addr;      // Indirizzo (ottenuto dalla modalità indirizzamento) dell'operando di cui eseguire il fetch
    int8_t rel_addr;
} cpu;

#endif 