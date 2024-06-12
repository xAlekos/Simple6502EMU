#ifndef CPU_H
#define CPU_H

#include "../utils/utils.h"
#include "cpu_types.h" // Inclusione delle definizioni dei tipi CPU
#include "../system/system_types.h" // Inclusione delle definizioni dei tipi System
#include "../system/system.h"

typedef struct instruction {
    char* name;
    uint8_t (*op)(cpu*);
    uint8_t (*addr_mode)(cpu*);
    uint8_t cycles;
} instruction_t;

/*========================= Addressing Modes ==============================*/
static uint8_t IMP(cpu* ctx); static uint8_t IMM(cpu* ctx);
static uint8_t ZP0(cpu* ctx); static uint8_t ZPX(cpu* ctx);
static uint8_t ZPY(cpu* ctx); static uint8_t ABS(cpu* ctx);
static uint8_t ABX(cpu* ctx); static uint8_t ABY(cpu* ctx);
static uint8_t IND(cpu* ctx); static uint8_t IZX(cpu* ctx);
static uint8_t IZY(cpu* ctx); static uint8_t REL(cpu* ctx);

/*========================= Operations ====================================*/
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
void reset(cpu* ctx);
void irq(cpu* ctx);
void nmi(cpu* ctx);
cpu* cpu_init();
extern instruction_t instructions_table[256];
extern char disassembled_instructions[0XFFFF][0xFF];
void connect_system(cpu* ctx, nes_system* sys);
uint8_t cpu_read_byte(cpu* ctx, uint16_t address);
void cpu_write_byte(cpu* ctx, uint16_t address, uint8_t value);
void set_flag(cpu* ctx, sr_flag_t flag, bool set_to);
bool get_flag(cpu* ctx, sr_flag_t flag);
void disassemble(cpu* ctx, uint16_t start_address, uint16_t end_address);

#endif // CPU_H
