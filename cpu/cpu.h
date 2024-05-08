#include "./utils/utils.h"
#include "./memory/memory.h"


typedef enum flags{

    C = (1 << 0), //Carry
    Z = (1 << 1), // Zero
    I = (1 << 2), //Mask interrupt 
    D = (1 << 3), // Decimal
    B = (1 << 4), // Break
    U = (1 << 5), // Unused
    V = (1 << 6), // Overflow 
    N = (1 << 7)
}sr_flag;

typedef struct cpu{

    //Registers Accumulator, X and Y;
    uint8_t a, x ,y;

    //Program Counter register
    uint16_t pc;

    //Stack Pointer register
    uint8_t  sp;
    
    /*
    8 possible flags:
        
        N : Negative
        V : Overflow
        U : Ignored
        B : Break
        D : Decimal 
        I : Interrupt inhibit
        Z : Zero
        C : Carry
    */
    uint8_t status;

    //64 KiB memory, shared.
    mem* memory;

}cpu;

cpu* cpu_init();

void connect_memory(cpu* ctx,mem* memory);

uint8_t cpu_read_byte(cpu* ctx,uint16_t address);

void cpu_write_byte(cpu* ctx,uint16_t address, uint8_t value);

uint16_t read_address(uint16_t offset, cpu* ctx);

