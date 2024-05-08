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
static uint8_t IMP(void); static uint8_t IMM(void);
static uint8_t ZP0(void); static uint8_t ZPX(void);
static uint8_t ZPY(void); static uint8_t ABS(void);
static uint8_t ABX(void); static uint8_t ABY(void);
static uint8_t IND(void); static uint8_t IZX(void);
static uint8_t IZY(void); static uint8_t REL(void);

/*
    Opcodes
*/
static uint8_t XXX(void);    static uint8_t LDX(void);    static uint8_t LDY(void);
static uint8_t LDA(void);    static uint8_t BPL(void);    static uint8_t JSR(void);
static uint8_t BRK(void);    static uint8_t BMI(void);    static uint8_t RTI(void);
static uint8_t BVC(void);    static uint8_t RTS(void);    static uint8_t BVS(void);
static uint8_t NOP(void);    static uint8_t BCC(void);    static uint8_t BCS(void);
static uint8_t BNE(void);    static uint8_t CPX(void);    static uint8_t CPY(void);
static uint8_t BEQ(void);    static uint8_t ORA(void);    static uint8_t AND(void);
static uint8_t EOR(void);    static uint8_t BIT(void);    static uint8_t ADC(void);
static uint8_t STA(void);    static uint8_t STX(void);    static uint8_t STY(void);
static uint8_t CMP(void);    static uint8_t SBC(void);    static uint8_t ASL(void);
static uint8_t ROL(void);    static uint8_t LSR(void);    static uint8_t ROR(void);
static uint8_t DEC(void);    static uint8_t DEX(void);    static uint8_t DEY(void);
static uint8_t INC(void);    static uint8_t INX(void);    static uint8_t INY(void);
static uint8_t PHP(void);    static uint8_t SEC(void);    static uint8_t CLC(void);
static uint8_t CLI(void);    static uint8_t PLP(void);    static uint8_t PLA(void);
static uint8_t PHA(void);    static uint8_t SEI(void);    static uint8_t TYA(void);
static uint8_t CLV(void);    static uint8_t CLD(void);    static uint8_t SED(void);
static uint8_t TXA(void);    static uint8_t TXS(void);    static uint8_t TAX(void);
static uint8_t TAY(void);    static uint8_t TSX(void);    static uint8_t JMP(void);




cpu* cpu_init();

void connect_memory(cpu* ctx,mem* memory);

uint8_t cpu_read_byte(cpu* ctx,uint16_t address);

void cpu_write_byte(cpu* ctx,uint16_t address, uint8_t value);

uint16_t read_address(uint16_t offset, cpu* ctx);

void set_flag(cpu* ctx,sr_flag_t flag, bool set_to);

bool get_flag(cpu* ctx, sr_flag_t flag);

void init_instruction_lookup_table(instruction_t* table);