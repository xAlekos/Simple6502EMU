#ifndef SYSTEM_TYPES_H
#define SYSTEM_TYPES_H

#include "../utils/utils.h"

// Forward declaration per cpu
struct cpu;

typedef struct cpu_ram {
    uint8_t bytes[2048];
} mem;

typedef struct sys {
    mem* cpu_ram;
    struct cpu* ctx; // Usare la forward declaration
}nes_system;

#endif // SYSTEM_TYPES_H
