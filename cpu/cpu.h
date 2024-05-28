
#ifndef CPU_H
#define CPU_H
#include "../utils/utils.h"
#include "../memory/memory.h"



typedef enum flags{

    C = (1 << 0), //Carry
    Z = (1 << 1), // Zero
    I = (1 << 2), //Mask interrupt 
    D = (1 << 3), // Decimal
    B = (1 << 4), // Break
    U = (1 << 5), // Unused
    V = (1 << 6), // Overflow 
    N = (1 << 7)

}sr_flag_t;


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


    //Valori in esecuzione
    uint8_t cur_opcode;
    uint8_t remaining_cycles;
    uint8_t fetched;
    uint16_t abs_addr; //Indirizzo (ottenuto dalla modalità indirizzamento) dell'operando di cui eseguire il fetch
    int8_t rel_addr;

}cpu;

typedef struct instruction{

char* name;
uint8_t (*op)(cpu*);
uint8_t (*addr_mode)(cpu*);
uint8_t cycles;

}instruction_t;

/*=========================Addressing Modes=======================================*/
static uint8_t IMP(cpu* ctx); static uint8_t IMM(cpu* ctx);
static uint8_t ZP0(cpu* ctx); static uint8_t ZPX(cpu* ctx);
static uint8_t ZPY(cpu* ctx); static uint8_t ABS(cpu* ctx);
static uint8_t ABX(cpu* ctx); static uint8_t ABY(cpu* ctx);
static uint8_t IND(cpu* ctx); static uint8_t IZX(cpu* ctx);
static uint8_t IZY(cpu* ctx); static uint8_t REL(cpu* ctx);

/*=========================Operations=======================================*/

static uint8_t XXX(cpu* ctx);    static uint8_t LDX(cpu* ctx);    static uint8_t LDY(cpu* ctx);
static uint8_t LDA(cpu* ctx);    static uint8_t BPL(cpu* ctx);    static uint8_t JSR(cpu* ctx);
static uint8_t BRK(cpu* ctx);    static uint8_t BMI(cpu* ctx);    static uint8_t RTI(cpu* ctx);
static uint8_t BVC(cpu* ctx);    static uint8_t RTS(cpu* ctx);    static uint8_t BVS(cpu* ctx);
static uint8_t NOP(cpu* ctx);    static uint8_t BCC(cpu* ctx);    static uint8_t BCS(cpu* ctx);
static uint8_t BNE(cpu* ctx);    static uint8_t CPX(cpu* ctx);    static uint8_t CPY(cpu* ctx);
static uint8_t BEQ(cpu* ctx);    static uint8_t ORA(cpu* ctx);    static uint8_t AND(cpu* ctx);
static uint8_t EOR(cpu* ctx);    static uint8_t BIT(cpu* ctx);    static uint8_t ADC(cpu* ctx);
static uint8_t STA(cpu* ctx);    static uint8_t STX(cpu* ctx);    static uint8_t STY(cpu* ctx);
static uint8_t CMP(cpu* ctx);    static uint8_t SBC(cpu* ctx);    static uint8_t ASL(cpu* ctx);
static uint8_t ROL(cpu* ctx);    static uint8_t LSR(cpu* ctx);    static uint8_t ROR(cpu* ctx);
static uint8_t DEC(cpu* ctx);    static uint8_t DEX(cpu* ctx);    static uint8_t DEY(cpu* ctx);
static uint8_t INC(cpu* ctx);    static uint8_t INX(cpu* ctx);    static uint8_t INY(cpu* ctx);
static uint8_t PHP(cpu* ctx);    static uint8_t SEC(cpu* ctx);    static uint8_t CLC(cpu* ctx);
static uint8_t CLI(cpu* ctx);    static uint8_t PLP(cpu* ctx);    static uint8_t PLA(cpu* ctx);
static uint8_t PHA(cpu* ctx);    static uint8_t SEI(cpu* ctx);    static uint8_t TYA(cpu* ctx);
static uint8_t CLV(cpu* ctx);    static uint8_t CLD(cpu* ctx);    static uint8_t SED(cpu* ctx);
static uint8_t TXA(cpu* ctx);    static uint8_t TXS(cpu* ctx);    static uint8_t TAX(cpu* ctx);
static uint8_t TAY(cpu* ctx);    static uint8_t TSX(cpu* ctx);    static uint8_t JMP(cpu* ctx);


void clock(cpu* ctx);

void reset();
void irq();
void nmi();

cpu* cpu_init();

extern instruction_t instrunctions_table[256];

void connect_memory(cpu* ctx,mem* memory);

uint8_t cpu_read_byte(cpu* ctx,uint16_t address);

void cpu_write_byte(cpu* ctx,uint16_t address, uint8_t value);

void set_flag(cpu* ctx,sr_flag_t flag, bool set_to);

bool get_flag(cpu* ctx, sr_flag_t flag);

#endif