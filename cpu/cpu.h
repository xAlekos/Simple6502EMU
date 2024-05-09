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
    uint16_t operand_addr; //Indirizzo (ottenuto dalla modalità indirizzamento) dell'operando di cui eseguire il fetch


}cpu;

typedef struct instruction{

char* name;
uint8_t (*op)(void);
uint8_t (*addr_mode)(void);
uint8_t cycles;

}instruction_t;

/*
    Addressing Modes
*/
static uint8_t IMP(cpu* ctx); static uint8_t IMM(cpu* ctx);
static uint8_t ZP0(cpu* ctx); static uint8_t ZPX(cpu* ctx);
static uint8_t ZPY(cpu* ctx); static uint8_t ABS(cpu* ctx);
static uint8_t ABX(cpu* ctx); static uint8_t ABY(cpu* ctx);
static uint8_t IND(cpu* ctx); static uint8_t IZX(cpu* ctx);
static uint8_t IZY(cpu* ctx); static uint8_t REL(cpu* ctx);

/*
    Opcodes
*/

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


//Lookup Table per istruzione, Addressingmode, e cicli necessari

instruction_t instrunctions_table[256] = {
        {"BRK", &BRK, &IMM, 7}, {"ORA", &ORA, &IZX, 6}, {"???", &XXX, &IMP, 2},
        {"???", &XXX, &IMP, 8}, {"???", &NOP, &IMP, 3}, {"ORA", &ORA, &ZP0, 3},
        {"ASL", &ASL, &ZP0, 5}, {"???", &XXX, &IMP, 5}, {"PHP", &PHP, &IMP, 3},
        {"ORA", &ORA, &IMM, 2}, {"ASL", &ASL, &IMP, 2}, {"???", &XXX, &IMP, 2},
        {"???", &NOP, &IMP, 4}, {"ORA", &ORA, &ABS, 4}, {"ASL", &ASL, &ABS, 6},
        {"???", &XXX, &IMP, 6}, {"BPL", &BPL, &REL, 2}, {"ORA", &ORA, &IZY, 5},
        
        {"???", &XXX, &IMP, 2}, {"???", &XXX, &IMP, 8}, {"???", &NOP, &IMP, 4},
        {"ORA", &ORA, &ZPX, 4}, {"ASL", &ASL, &ZPX, 6}, {"???", &XXX, &IMP, 6},
        {"CLC", &CLC, &IMP, 2}, {"ORA", &ORA, &ABY, 4}, {"???", &NOP, &IMP, 2},
        {"???", &XXX, &IMP, 7}, {"???", &NOP, &IMP, 4}, {"ORA", &ORA, &ABX, 4},
        {"ASL", &ASL, &ABX, 7}, {"???", &XXX, &IMP, 7}, {"JSR", &JSR, &ABS, 6},
        {"AND", &AND, &IZX, 6}, {"???", &XXX, &IMP, 2}, {"???", &XXX, &IMP, 8},

        {"BIT", &BIT, &ZP0, 3}, {"AND", &AND, &ZP0, 3}, {"ROL", &ROL, &ZP0, 5},
        {"???", &XXX, &IMP, 5}, {"PLP", &PLP, &IMP, 4}, {"AND", &AND, &IMM, 2},
        {"ROL", &ROL, &IMP, 2}, {"???", &XXX, &IMP, 2}, {"BIT", &BIT, &ABS, 4},
        {"AND", &AND, &ABS, 4}, {"ROL", &ROL, &ABS, 6}, {"???", &XXX, &IMP, 6},
        {"BMI", &BMI, &REL, 2}, {"AND", &AND, &IZY, 5}, {"???", &XXX, &IMP, 2},
        {"???", &XXX, &IMP, 8}, {"???", &NOP, &IMP, 4}, {"AND", &AND, &ZPX, 4},

        {"ROL", &ROL, &ZPX, 6}, {"???", &XXX, &IMP, 6}, {"SEC", &SEC, &IMP, 2},
        {"AND", &AND, &ABY, 4}, {"???", &NOP, &IMP, 2}, {"???", &XXX, &IMP, 7},
        {"???", &NOP, &IMP, 4}, {"AND", &AND, &ABX, 4}, {"ROL", &ROL, &ABX, 7},
        {"???", &XXX, &IMP, 7}, {"RTI", &RTI, &IMP, 6}, {"EOR", &EOR, &IZX, 6},
        {"???", &XXX, &IMP, 2}, {"???", &XXX, &IMP, 8}, {"???", &NOP, &IMP, 3},
        {"EOR", &EOR, &ZP0, 3}, {"LSR", &LSR, &ZP0, 5}, {"???", &XXX, &IMP, 5},

        {"PHA", &PHA, &IMP, 3}, {"EOR", &EOR, &IMM, 2}, {"LSR", &LSR, &IMP, 2},
        {"???", &XXX, &IMP, 2}, {"JMP", &JMP, &ABS, 3}, {"EOR", &EOR, &ABS, 4},
        {"LSR", &LSR, &ABS, 6}, {"???", &XXX, &IMP, 6}, {"BVC", &BVC, &REL, 2},
        {"EOR", &EOR, &IZY, 5}, {"???", &XXX, &IMP, 2}, {"???", &XXX, &IMP, 8},
        {"???", &NOP, &IMP, 4}, {"EOR", &EOR, &ZPX, 4}, {"LSR", &LSR, &ZPX, 6},
        {"???", &XXX, &IMP, 6}, {"CLI", &CLI, &IMP, 2}, {"EOR", &EOR, &ABY, 4},

        {"???", &NOP, &IMP, 2}, {"???", &XXX, &IMP, 7}, {"???", &NOP, &IMP, 4},
        {"EOR", &EOR, &ABX, 4}, {"LSR", &LSR, &ABX, 7}, {"???", &XXX, &IMP, 7},
        {"RTS", &RTS, &IMP, 6}, {"ADC", &ADC, &IZX, 6}, {"???", &XXX, &IMP, 2},
        {"???", &XXX, &IMP, 8}, {"???", &NOP, &IMP, 3}, {"ADC", &ADC, &ZP0, 3},
        {"ROR", &ROR, &ZP0, 5}, {"???", &XXX, &IMP, 5}, {"PLA", &PLA, &IMP, 4},
        {"ADC", &ADC, &IMM, 2}, {"ROR", &ROR, &IMP, 2}, {"???", &XXX, &IMP, 2},

        {"JMP", &JMP, &IND, 5}, {"ADC", &ADC, &ABS, 4}, {"ROR", &ROR, &ABS, 6},
        {"???", &XXX, &IMP, 6}, {"BVS", &BVS, &REL, 2}, {"ADC", &ADC, &IZY, 5},
        {"???", &XXX, &IMP, 2}, {"???", &XXX, &IMP, 8}, {"???", &NOP, &IMP, 4},
        {"ADC", &ADC, &ZPX, 4}, {"ROR", &ROR, &ZPX, 6}, {"???", &XXX, &IMP, 6},
        {"SEI", &SEI, &IMP, 2}, {"ADC", &ADC, &ABY, 4}, {"???", &NOP, &IMP, 2},
        {"???", &XXX, &IMP, 7}, {"???", &NOP, &IMP, 4}, {"ADC", &ADC, &ABX, 4},

        {"ROR", &ROR, &ABX, 7}, {"???", &XXX, &IMP, 7}, {"???", &NOP, &IMP, 2},
        {"STA", &STA, &IZX, 6}, {"???", &NOP, &IMP, 2}, {"???", &XXX, &IMP, 6},
        {"STY", &STY, &ZP0, 3}, {"STA", &STA, &ZP0, 3}, {"STX", &STX, &ZP0, 3},
        {"???", &XXX, &IMP, 3}, {"DEY", &DEY, &IMP, 2}, {"???", &NOP, &IMP, 2},
        {"TXA", &TXA, &IMP, 2}, {"???", &XXX, &IMP, 2}, {"STY", &STY, &ABS, 4},
        {"STA", &STA, &ABS, 4}, {"STX", &STX, &ABS, 4}, {"???", &XXX, &IMP, 4},

        {"BCC", &BCC, &REL, 2}, {"STA", &STA, &IZY, 6}, {"???", &XXX, &IMP, 2},
        {"???", &XXX, &IMP, 6}, {"STY", &STY, &ZPX, 4}, {"STA", &STA, &ZPX, 4},
        {"STX", &STX, &ZPY, 4}, {"???", &XXX, &IMP, 4}, {"TYA", &TYA, &IMP, 2},
        {"STA", &STA, &ABY, 5}, {"TXS", &TXS, &IMP, 2}, {"???", &XXX, &IMP, 5},
        {"???", &NOP, &IMP, 5}, {"STA", &STA, &ABX, 5}, {"???", &XXX, &IMP, 5},
        {"???", &XXX, &IMP, 5}, {"LDY", &LDY, &IMM, 2}, {"LDA", &LDA, &IZX, 6},

        {"LDX", &LDX, &IMM, 2}, {"???", &XXX, &IMP, 6}, {"LDY", &LDY, &ZP0, 3},
        {"LDA", &LDA, &ZP0, 3}, {"LDX", &LDX, &ZP0, 3}, {"???", &XXX, &IMP, 3},
        {"TAY", &TAY, &IMP, 2}, {"LDA", &LDA, &IMM, 2}, {"TAX", &TAX, &IMP, 2},
        {"???", &XXX, &IMP, 2}, {"LDY", &LDY, &ABS, 4}, {"LDA", &LDA, &ABS, 4},
        {"LDX", &LDX, &ABS, 4}, {"???", &XXX, &IMP, 4}, {"BCS", &BCS, &REL, 2},
        {"LDA", &LDA, &IZY, 5}, {"???", &XXX, &IMP, 2}, {"???", &XXX, &IMP, 5},

        {"LDY", &LDY, &ZPX, 4}, {"LDA", &LDA, &ZPX, 4}, {"LDX", &LDX, &ZPY, 4},
        {"???", &XXX, &IMP, 4}, {"CLV", &CLV, &IMP, 2}, {"LDA", &LDA, &ABY, 4},
        {"TSX", &TSX, &IMP, 2}, {"???", &XXX, &IMP, 4}, {"LDY", &LDY, &ABX, 4},
        {"LDA", &LDA, &ABX, 4}, {"LDX", &LDX, &ABY, 4}, {"???", &XXX, &IMP, 4},
        {"CPY", &CPY, &IMM, 2}, {"CMP", &CMP, &IZX, 6}, {"???", &NOP, &IMP, 2},
        {"???", &XXX, &IMP, 8}, {"CPY", &CPY, &ZP0, 3}, {"CMP", &CMP, &ZP0, 3},

        {"DEC", &DEC, &ZP0, 5}, {"???", &XXX, &IMP, 5}, {"INY", &INY, &IMP, 2},
        {"CMP", &CMP, &IMM, 2}, {"DEX", &DEX, &IMP, 2}, {"???", &XXX, &IMP, 2},
        {"CPY", &CPY, &ABS, 4}, {"CMP", &CMP, &ABS, 4}, {"DEC", &DEC, &ABS, 6},
        {"???", &XXX, &IMP, 6}, {"BNE", &BNE, &REL, 2}, {"CMP", &CMP, &IZY, 5},
        {"???", &XXX, &IMP, 2}, {"???", &XXX, &IMP, 8}, {"???", &NOP, &IMP, 4},
        {"CMP", &CMP, &ZPX, 4}, {"DEC", &DEC, &ZPX, 6}, {"???", &XXX, &IMP, 6},

        {"CLD", &CLD, &IMP, 2}, {"CMP", &CMP, &ABY, 4}, {"NOP", &NOP, &IMP, 2},
        {"???", &XXX, &IMP, 7}, {"???", &NOP, &IMP, 4}, {"CMP", &CMP, &ABX, 4},
        {"DEC", &DEC, &ABX, 7}, {"???", &XXX, &IMP, 7}, {"CPX", &CPX, &IMM, 2},
        {"SBC", &SBC, &IZX, 6}, {"???", &NOP, &IMP, 2}, {"???", &XXX, &IMP, 8},
        {"CPX", &CPX, &ZP0, 3}, {"SBC", &SBC, &ZP0, 3}, {"INC", &INC, &ZP0, 5},
        {"???", &XXX, &IMP, 5}, {"INX", &INX, &IMP, 2}, {"SBC", &SBC, &IMM, 2},

        {"NOP", &NOP, &IMP, 2}, {"???", &SBC, &IMP, 2}, {"CPX", &CPX, &ABS, 4},
        {"SBC", &SBC, &ABS, 4}, {"INC", &INC, &ABS, 6}, {"???", &XXX, &IMP, 6},
        {"BEQ", &BEQ, &REL, 2}, {"SBC", &SBC, &IZY, 5}, {"???", &XXX, &IMP, 2},
        {"???", &XXX, &IMP, 8}, {"???", &NOP, &IMP, 4}, {"SBC", &SBC, &ZPX, 4},
        {"INC", &INC, &ZPX, 6}, {"???", &XXX, &IMP, 6}, {"SED", &SED, &IMP, 2},
        {"SBC", &SBC, &ABY, 4}, {"NOP", &NOP, &IMP, 2}, {"???", &XXX, &IMP, 7},
        {"???", &NOP, &IMP, 4}, {"SBC", &SBC, &ABX, 4}, {"INC", &INC, &ABX, 7},
        {"???", &XXX, &IMP, 7},
    };

void clock(cpu* ctx);

void reset();
void irq();
void nmi();

cpu* cpu_init();

void connect_memory(cpu* ctx,mem* memory);

uint8_t cpu_read_byte(cpu* ctx,uint16_t address);

void cpu_write_byte(cpu* ctx,uint16_t address, uint8_t value);

void set_flag(cpu* ctx,sr_flag_t flag, bool set_to);

bool get_flag(cpu* ctx, sr_flag_t flag);

void init_instruction_lookup_table(instruction_t* table);