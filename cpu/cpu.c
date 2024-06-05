#include "cpu.h"


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


cpu* cpu_init(){

    cpu* new_cpu = (cpu*)calloc(1,sizeof(cpu));
    return new_cpu;
}

void connect_memory(cpu* ctx,mem* memory){
    ctx->memory = memory;
}

uint8_t cpu_read_byte(cpu* ctx,uint16_t address){

    return mem_read_byte(ctx->memory,address);

}

void cpu_write_byte(cpu* ctx,uint16_t address, uint8_t value){

    mem_write_byte(ctx->memory,address,value);

}


void reset(cpu* ctx){

    ctx->a = 0;
    ctx->x = 0;
    ctx->y = 0;
    ctx->sp = 0xFD;
    ctx->status = 0x00 | U;

    ctx->abs_addr = 0XFFFC;
    uint16_t lo = cpu_read_byte(ctx,ctx->abs_addr);
    uint16_t hi = cpu_read_byte(ctx,ctx->abs_addr + 1);
    ctx->pc = (hi << 8) | lo;

    ctx->abs_addr = 0x0000;
    ctx->rel_addr = 0x00;
    ctx->fetched = 0x00;

    ctx->remaining_cycles = 8;
}

void set_flag(cpu* ctx,sr_flag_t flag, bool set_to){

    if(set_to)
        ctx->status |= flag;
    else
        ctx->status &= (~flag);
}

bool get_flag(cpu* ctx,sr_flag_t flag){

  return (ctx->status & flag) != 0; 

}


/*
Quando arriva il segnale di clock svolge in un singolo clock ogni operazione, se questa necessità più di un ciclo
allora non fa niente finchè i cicli necessari per svolgere l'operazione non sono stati esauriti.
*/
void clock(cpu* ctx){
    
    if(ctx->remaining_cycles == 0){

        ctx->cur_opcode = cpu_read_byte(ctx,ctx->pc);
        ctx->pc++;

        ctx->remaining_cycles = instrunctions_table[ctx->cur_opcode].cycles;

        uint8_t another_cycle_1 = (instrunctions_table[ctx->cur_opcode].addr_mode(ctx));

        uint8_t another_cycle_2 = (instrunctions_table[ctx->cur_opcode].op(ctx));

        ctx->remaining_cycles += (another_cycle_1 & another_cycle_2);

    }

    ctx->remaining_cycles--;
    
}


uint8_t fetch(cpu* ctx){

    if(!(instrunctions_table[ctx->cur_opcode].addr_mode == &IMP))
    ctx->fetched = cpu_read_byte(ctx,ctx->abs_addr);
    return ctx->fetched;
}
/*=========================Addressing Modes=======================================*/

//Abbiamo accorpato IMP ed A come addressing mode
uint8_t IMP(cpu* ctx)
{
    ctx->fetched = ctx->a;
    return 0;
}


uint8_t IMM(cpu *ctx)
{
    ctx->abs_addr = ctx->pc;
    ctx->pc++;
    return 0;
}

uint8_t ZP0(cpu *ctx){

    ctx->abs_addr = cpu_read_byte(ctx,ctx->pc);
    ctx->pc++;
    ctx->abs_addr &= 0x00FF;
    return 0;
}

uint8_t ZPX(cpu *ctx){

    ctx->abs_addr = cpu_read_byte(ctx,ctx->pc) + ctx->x;
    ctx->pc++;
    ctx->abs_addr &= 0x00FF;
    return 0;
}

uint8_t ZPY(cpu *ctx){
    
    ctx->abs_addr = cpu_read_byte(ctx,ctx->pc) + ctx->y;
    ctx->pc++;
    ctx->abs_addr &= 0x00FF;
    return 0;
}

// TODO VERIFICA
uint8_t ABS(cpu *ctx){
    
    uint8_t lo = cpu_read_byte(ctx,ctx->pc);
    ctx->pc++;

    uint8_t hi = cpu_read_byte(ctx,ctx->pc);
    ctx->pc++;

    ctx->abs_addr= (hi << 8) | lo;

    return 0;
}

uint8_t ABX(cpu *ctx){

    uint8_t lo = cpu_read_byte(ctx,ctx->pc);
    ctx->pc++;

    uint8_t hi = cpu_read_byte(ctx,ctx->pc);
    ctx->pc++;
    
    ctx->abs_addr= (hi << 8) | lo;
    ctx->abs_addr += ctx->x;

    if((ctx->abs_addr & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

uint8_t ABY(cpu *ctx){

    uint8_t lo = cpu_read_byte(ctx,ctx->pc);
    ctx->pc++;

    uint8_t hi = cpu_read_byte(ctx,ctx->pc);
    ctx->pc++;
    
    ctx->abs_addr= (hi << 8) | lo;
    ctx->abs_addr += ctx->y;

    if((ctx->abs_addr & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

uint8_t IND(cpu *ctx){

    uint8_t wrap = 0;
    uint16_t point_to;

    uint8_t lo = cpu_read_byte(ctx,ctx->pc);
    ctx->pc++;

    uint8_t hi = cpu_read_byte(ctx,ctx->pc);
    ctx->pc++;
    
    point_to = (hi << 8) | lo;

    if(lo == 0x00FF) //BUG HARDWARE! 
        wrap = 1;

    lo = cpu_read_byte(ctx, point_to);

    if(wrap != 1)
        hi = cpu_read_byte(ctx, point_to + 1);
    else
        hi = cpu_read_byte(ctx, point_to & 0xFF00);

    ctx->abs_addr = (hi << 8) | lo;

    return 0;
}

uint8_t IZX(cpu *ctx){

    uint16_t point_to = cpu_read_byte(ctx,ctx->pc) + ctx->x;
    ctx->pc++;

    point_to &= 0X00FF;
    uint8_t lo = cpu_read_byte(ctx,point_to);
    uint8_t hi = cpu_read_byte(ctx, point_to + 1);
    ctx->abs_addr = (hi << 8) | lo;
     
    return 0;
}

uint8_t IZY(cpu *ctx){

    uint16_t point_to = cpu_read_byte(ctx,ctx->pc);
    ctx->pc++;
    point_to &= 0X00FF;

    uint8_t lo = cpu_read_byte(ctx,point_to);
    uint8_t hi = cpu_read_byte(ctx, point_to + 1);
    ctx->abs_addr = ((hi << 8) | lo) + ctx->y;

    if((ctx->abs_addr & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

uint8_t REL(cpu *ctx){

    ctx->rel_addr = cpu_read_byte(ctx,ctx->pc);
    ctx->pc++;
    
    if(ctx->rel_addr & 0x80) // se è negativo estendi il segno!
        ctx->rel_addr |= 0XFF00;

    return 0;
}


/*=========================Operations=======================================*/


uint8_t AND(cpu *ctx)
{

    fetch(ctx);
    ctx->a = ctx->a & ctx->fetched;
    set_flag(ctx, Z, ctx->a == 0x00);
    set_flag(ctx, N, ctx->a & 0x80);
    return 1;
}

uint8_t EOR(cpu *ctx)
{

    fetch(ctx);
    ctx->a = ctx->a ^ ctx->fetched;
    set_flag(ctx, Z, ctx->a == 0x00);
    set_flag(ctx, N, ctx->a & 0x80);
    return 1;
}

uint8_t ORA(cpu *ctx)
{

    fetch(ctx);
    ctx->a = ctx->a | ctx->fetched;
    set_flag(ctx, Z, ctx->a == 0x00);
    set_flag(ctx, N, ctx->a & 0x80);
    return 1;
}

uint8_t BIT(cpu *ctx){
    fetch(ctx);
    set_flag(ctx,N,ctx->fetched & (1 << 7));
    set_flag(ctx,V,ctx->fetched & (1 << 6));
    set_flag(ctx,Z, ctx->a & ctx->fetched);

    return 0;
}

uint8_t ADC(cpu *ctx)
{
    uint16_t sum;
    fetch(ctx);
    //Facciamo il casting a 16 bit per poter rilevare un eventuale carry 
    sum = (uint16_t)ctx->a + (uint16_t)ctx->fetched + (uint16_t)get_flag(ctx,C);

    set_flag(ctx, C, sum > 255);
    set_flag(ctx, Z, (sum & 0x00FF) == 0x00);
    set_flag(ctx, N, sum & 0x80);
    set_flag(ctx, V, ((ctx->a ^ sum) & (~(ctx->a ^ ctx->fetched))) & 0x80);
    
    ctx->a = sum & 0x00FF;

    return 1;
}

uint8_t STA(cpu *ctx)
{

    cpu_write_byte(ctx,ctx->abs_addr,ctx->a);

    return 0;
}

uint8_t STX(cpu *ctx)
{

    cpu_write_byte(ctx,ctx->abs_addr,ctx->x);

    return 0;
}

uint8_t STY(cpu *ctx)
{

    cpu_write_byte(ctx,ctx->abs_addr,ctx->y);

    return 0;
}

uint8_t TAX(cpu* ctx){

    ctx->x = ctx->a;
    set_flag(ctx,N,ctx->x & 0x80);
    set_flag(ctx,Z,ctx->x == 0x00);
    
    return 0;
}

uint8_t TAY(cpu* ctx){

    ctx->y = ctx->a;
    set_flag(ctx,N,ctx->y & 0x80);
    set_flag(ctx,Z,ctx->y == 0x00);
    
    return 0;
}

uint8_t TSX(cpu* ctx){

    ctx->x = ctx->sp;
    set_flag(ctx,N,ctx->x & 0x80);
    set_flag(ctx,Z,ctx->x == 0x00);
    
    return 0;
}



uint8_t TXA(cpu* ctx){

    ctx->a = ctx->x;
    set_flag(ctx,N,ctx->a & 0x80);
    set_flag(ctx,Z,ctx->a == 0x00);
    
    return 0;
}

uint8_t TXS(cpu* ctx){

    ctx->sp = ctx->x;
  
    return 0;
}

uint8_t TYA(cpu* ctx){

    ctx->a = ctx->y;
    set_flag(ctx,N,ctx->a & 0x80);
    set_flag(ctx,Z,ctx->a == 0x00);
    
    return 0;
}


uint8_t INC(cpu* ctx){

    fetch(ctx);
    uint8_t tmp = ctx->fetched + 1;
    cpu_write_byte(ctx,ctx->abs_addr,tmp);
    set_flag(ctx,N,tmp & 0x80);
    set_flag(ctx,Z,tmp & 0x00);

    return 0;
}

uint8_t INX(cpu *ctx){

    ctx->x++;
    set_flag(ctx,N,ctx->x & 0x80);
    set_flag(ctx,Z,ctx->x & 0x00);
}

uint8_t INY(cpu *ctx){

    ctx->y++;
    set_flag(ctx,N,ctx->y & 0x80);
    set_flag(ctx,Z,ctx->y & 0x00);
}

uint8_t DEC(cpu* ctx){

    fetch(ctx);
    uint8_t tmp = ctx->fetched - 1;
    cpu_write_byte(ctx,ctx->abs_addr,tmp);
    set_flag(ctx,N,tmp & 0x80);
    set_flag(ctx,Z,tmp & 0x00);

    return 0;
}

uint8_t DEX(cpu* ctx){

    ctx->x--;
    set_flag(ctx,N,ctx->x & 0x80);
    set_flag(ctx,Z,ctx->x & 0x00);

    return 0;
}

uint8_t DEY(cpu* ctx){

    ctx->y--;
    set_flag(ctx,N,ctx->y & 0x80);
    set_flag(ctx,Z,ctx->y & 0x00);

    return 0;
}

uint8_t SBC(cpu *ctx){
    
    uint16_t sub;
    fetch(ctx);
    uint16_t fetched_inverted = (uint16_t)ctx->fetched ^ 0x00FF;
    //Facciamo il casting a 16 bit per poter rilevare un eventuale carry 
    sub = (uint16_t)ctx->a + fetched_inverted + (uint16_t)get_flag(ctx,C);

    set_flag(ctx, C, sub > 255);
    set_flag(ctx, Z, (sub & 0x00FF) == 0x00);
    set_flag(ctx, N, sub & 0x80);
    set_flag(ctx, V, ((ctx->a ^ sub) & (~(ctx->a ^ ctx->fetched))) & 0x80);
    
    ctx->a = sub & 0x00FF;

    return 1;
}

uint8_t NOP(cpu *ctx)
{
    return 0;
}

uint8_t BCS(cpu *ctx)
{

    if(get_flag(ctx,C) == 1){

        ctx->remaining_cycles++;
        ctx->abs_addr = ctx->pc + ctx->rel_addr;

        if((ctx->abs_addr & 0xFF00) != (ctx->pc & 0XFF00))
            ctx->remaining_cycles++;

       ctx->pc = ctx->abs_addr;
    }

    return 0;
}

uint8_t BCC(cpu *ctx){

    if(get_flag(ctx,C) == 0){

        ctx->remaining_cycles++;
        ctx->abs_addr = ctx->pc + ctx->rel_addr;

        if((ctx->abs_addr & 0xFF00) != (ctx->pc & 0XFF00))
            ctx->remaining_cycles++;

       ctx->pc = ctx->abs_addr;
    }

    return 0;
}

uint8_t BEQ(cpu *ctx){

    if(get_flag(ctx,Z) == 1){

        ctx->remaining_cycles++;
        ctx->abs_addr = ctx->pc + ctx->rel_addr;

        if((ctx->abs_addr & 0xFF00) != (ctx->pc & 0XFF00))
            ctx->remaining_cycles++;

       ctx->pc = ctx->abs_addr;
    }

    return 0;
}

uint8_t BMI(cpu *ctx){

    if(get_flag(ctx,N) == 1){

        ctx->remaining_cycles++;
        ctx->abs_addr = ctx->pc + ctx->rel_addr;

        if((ctx->abs_addr & 0xFF00) != (ctx->pc & 0XFF00))
            ctx->remaining_cycles++;

       ctx->pc = ctx->abs_addr;
    }

    return 0;
}

uint8_t BNE(cpu *ctx){

    if(get_flag(ctx,Z) == 0){

        ctx->remaining_cycles++;
        ctx->abs_addr = ctx->pc + ctx->rel_addr;

        if((ctx->abs_addr & 0xFF00) != (ctx->pc & 0XFF00))
            ctx->remaining_cycles++;

       ctx->pc = ctx->abs_addr;
    }

    return 0;
}

uint8_t LDA(cpu *ctx){

    fetch(ctx);
    ctx->a = ctx->fetched;

    set_flag(ctx,N, ctx->a & 0x80);
    set_flag(ctx,Z, ctx->a == 0x00);

    return 0;
}

uint8_t LDX(cpu *ctx){

    fetch(ctx);
    ctx->x = ctx->fetched;

    set_flag(ctx,N, ctx->x & 0x80);
    set_flag(ctx,Z, ctx->x == 0x00);

    return 0;
}

uint8_t XXX(cpu *ctx)
{
    return 0;
}

uint8_t LDY(cpu *ctx)
{

    fetch(ctx);
    ctx->y = ctx->fetched;

    set_flag(ctx,N, ctx->y & 0x80);
    set_flag(ctx,Z, ctx->y == 0x00);

    return 0;
}

uint8_t ASL(cpu *ctx)
{
    fetch(ctx);

    set_flag(ctx,C,ctx->fetched & 0x80);
    
    ctx->fetched = ctx->fetched << 1;

    set_flag(ctx,Z,ctx->fetched == 0X00);
    set_flag(ctx,N,ctx->fetched & 0x80);


    if(instrunctions_table[ctx->cur_opcode].addr_mode == &IMP)
        ctx->a = ctx->fetched & 0x00FF;
    else
        cpu_write_byte(ctx,ctx->abs_addr,ctx->fetched);

    return 0;
}



uint8_t LSR(cpu *ctx)
{
    fetch(ctx);

    set_flag(ctx,C,ctx->fetched & 0x01);
    
    ctx->fetched = ctx->fetched >> 1;

    set_flag(ctx,Z,ctx->fetched == 0X00);
    set_flag(ctx,N,ctx->fetched & 0x80);


    if(instrunctions_table[ctx->cur_opcode].addr_mode == &IMP)
        ctx->a = ctx->fetched;
    else
        cpu_write_byte(ctx,ctx->abs_addr,ctx->fetched);

    return 0;
}

uint8_t ROL(cpu *ctx)
{
    fetch(ctx);

    set_flag(ctx,C,ctx->fetched & 0x80);
    
    ctx->fetched = (ctx->fetched << 1)  | get_flag(ctx,C);
    

    set_flag(ctx,Z,ctx->fetched == 0X00);
    set_flag(ctx,N,ctx->fetched & 0x80);


    if(instrunctions_table[ctx->cur_opcode].addr_mode == &IMP)
        ctx->a = ctx->fetched;
    else
        cpu_write_byte(ctx,ctx->abs_addr,ctx->fetched);

    return 0;
}

uint8_t ROR(cpu *ctx)
{
    fetch(ctx);

    set_flag(ctx,C,ctx->fetched & 0x01);
    
    ctx->fetched = (ctx->fetched >> 1)  | get_flag(ctx,C) << 7;
    

    set_flag(ctx,Z,ctx->fetched == 0X00);
    set_flag(ctx,N,ctx->fetched & 0x80);


    if(instrunctions_table[ctx->cur_opcode].addr_mode == &IMP)
        ctx->a = ctx->fetched;
    else
        cpu_write_byte(ctx,ctx->abs_addr,ctx->fetched);

    return 0;
}



uint8_t BPL(cpu *ctx)
{

    if(get_flag(ctx,N) == 0){

        ctx->remaining_cycles++;
        ctx->abs_addr = ctx->pc + ctx->rel_addr;

        if((ctx->abs_addr & 0xFF00) != (ctx->pc & 0XFF00))
            ctx->remaining_cycles++;

       ctx->pc = ctx->abs_addr;
    }

    return 0;
}

uint8_t BVC(cpu *ctx){

    if(get_flag(ctx,V) == 0){

        ctx->remaining_cycles++;
        ctx->abs_addr = ctx->pc + ctx->rel_addr;

        if((ctx->abs_addr & 0xFF00) != (ctx->pc & 0XFF00))
            ctx->remaining_cycles++;

       ctx->pc = ctx->abs_addr;
    }

    return 0;
}

uint8_t BVS(cpu *ctx){

    if(get_flag(ctx,V) == 1){

        ctx->remaining_cycles++;
        ctx->abs_addr = ctx->pc + ctx->rel_addr;

        if((ctx->abs_addr & 0xFF00) != (ctx->pc & 0XFF00))
            ctx->remaining_cycles++;

       ctx->pc = ctx->abs_addr;
    }

    return 0;
}

uint8_t SEC(cpu *ctx)
{
    set_flag(ctx,C,true);
    return 0;
}

uint8_t SED(cpu *ctx)
{
    set_flag(ctx,D,true);
    return 0;
}

uint8_t SEI(cpu *ctx)
{
    set_flag(ctx,I,true);
    return 0;
}

uint8_t CLC(cpu *ctx)
{
    set_flag(ctx,C,false);
    return 0;
}

uint8_t CLI(cpu *ctx)
{
    set_flag(ctx,I,false);
    return 0;
}

uint8_t CLV(cpu *ctx)
{
    set_flag(ctx,V,false);
}

uint8_t CLD(cpu *ctx)
{
    set_flag(ctx,D,false);
    return 0;
}

uint8_t CMP(cpu *ctx)
{
    fetch(ctx);
    int8_t tmp = ctx->a - ctx->fetched;
    set_flag(ctx,N,ctx->a >= ctx->fetched);
    set_flag(ctx,N,tmp & 0x80);
    set_flag(ctx,Z,tmp == 0x00);

    return 0;
}

uint8_t CPX(cpu *ctx)
{
    fetch(ctx);
    int8_t tmp = ctx->x - ctx->fetched;
    set_flag(ctx,N,ctx->x >= ctx->fetched);
    set_flag(ctx,N,tmp & 0x80);
    set_flag(ctx,Z,tmp == 0x00);

    return 0;
}

uint8_t CPY(cpu *ctx)
{
    fetch(ctx);
    int8_t tmp = ctx->y - ctx->fetched;
    set_flag(ctx,N,ctx->y >= ctx->fetched);
    set_flag(ctx,N,tmp & 0x80);
    set_flag(ctx,Z,tmp == 0x00);

    return 0;
}

uint8_t PHA(cpu *ctx)
{
    cpu_write_byte(ctx,0x0100 + ctx->sp, ctx->a);
    ctx->sp--;
    return 0;
}

uint8_t PHP(cpu *ctx)
{
    set_flag(ctx,B,1);
    set_flag(ctx,U,1);

    cpu_write_byte(ctx,0x0100 + ctx->sp, ctx->status);

    set_flag(ctx,B, 0);
	set_flag(ctx,U, 0);

    ctx->sp--;

    return 0;
}

uint8_t PLA(cpu *ctx)
{
    ctx->sp++;
    ctx->a = cpu_read_byte(ctx,0x0100 + ctx->sp);

    set_flag(ctx, N, ctx->a & 0x80);
	set_flag(ctx, Z, ctx->a == 0x00);


    return 0;
}

uint8_t PLP(cpu *ctx)
{
    ctx->sp++;
	ctx->sp = cpu_read_byte(ctx,0x0100 + ctx->sp);
	set_flag(ctx,U, 1);

    return 0;
}

uint8_t JMP(cpu *ctx)
{
    ctx->pc = ctx->abs_addr;
    return 0;
}

uint8_t JSR(cpu *ctx)
{
    ctx->pc--; // indica l'istruzione attuale

    cpu_write_byte(ctx,0x0100 + ctx->sp,(ctx->pc >> 8) & 0x00FF);
    ctx->sp--;

    cpu_write_byte(ctx,0x0100 + ctx->sp,ctx->pc & 0x00FF);
    ctx->sp--;

    ctx->pc = ctx->abs_addr;

    return 0;
}

uint8_t RTS(cpu *ctx)
{
    ctx->sp++;
    uint8_t pc_lo = cpu_read_byte(ctx,0x0100 + ctx->sp);
    ctx->sp++;
    uint8_t pc_hi = cpu_read_byte(ctx,0x0100 + ctx->sp);

    ctx->pc = (pc_hi << 8) | pc_lo;
    ctx->pc++;
    return 0;
}

uint8_t BRK(cpu *ctx){

    ctx->pc++;

    set_flag(ctx,I,1);

    cpu_write_byte(ctx,0x0100 + ctx->sp,(ctx->pc >> 8) & 0x00FF);
    ctx->sp--;

    cpu_write_byte(ctx,0x0100 + ctx->sp,ctx->pc & 0x00FF);
    ctx->sp--;

    set_flag(ctx,B,1);

    cpu_write_byte(ctx,0x0100 + ctx->sp, ctx->status);

    set_flag(ctx,B, 0);

    ctx->sp--;

    return 0;
}

uint8_t RTI(cpu *ctx){

    ctx->sp++;

	ctx->sp = cpu_read_byte(ctx,0x0100 + ctx->sp);
	set_flag(ctx, U, 0);
    set_flag(ctx, B, 0);

    ctx->sp++;
    uint8_t pc_lo = cpu_read_byte(ctx,0x0100 + ctx->sp);
    ctx->sp++;
    uint8_t pc_hi = cpu_read_byte(ctx,0x0100 + ctx->sp);

    ctx->pc = (pc_hi << 8) | pc_lo;

    return 0;
}


/*==========================================Disassembler=======================================================*/

char disassembled_instructions[0XFFFF][0xFF];

void disassemble(cpu* ctx,uint16_t start_address, uint16_t end_address){

    uint16_t address = start_address;
    uint16_t instruction_num = 0;
    uint8_t lo,hi;
    instruction_t current_instruction;

    while(address < end_address){
        current_instruction = instrunctions_table[cpu_read_byte(ctx,address)];
        strcpy(disassembled_instructions[instruction_num],current_instruction.name);
        uint16_t operand;
        char formatted_temp[50];
        if(current_instruction.addr_mode == &IMP){

        }
        else if(current_instruction.addr_mode == &IMM){
            address++;
            operand = cpu_read_byte(ctx,address) & 0x00FF;
            snprintf(formatted_temp,50," #$%02X",0x00FF & operand);
            strcat(disassembled_instructions[instruction_num], formatted_temp);
        }
        else if(current_instruction.addr_mode == &ZP0){
            address++;
            operand = cpu_read_byte(ctx,address) & 0x00FF;
            snprintf(formatted_temp,50," $%02X",0x00FF & operand);
            strcat(disassembled_instructions[instruction_num], formatted_temp);
        }
        else if(current_instruction.addr_mode == &ZPX){
            address++;
            operand = cpu_read_byte(ctx,address) & 0x00FF;
            snprintf(formatted_temp,50," $%02X, X",0x00FF & operand);
            strcat(disassembled_instructions[instruction_num], formatted_temp);
        }
        else if(current_instruction.addr_mode == &ZPY){
            address++;
            operand = cpu_read_byte(ctx,address) & 0x00FF;
            snprintf(formatted_temp,50," $%02X, Y",0x00FF & operand);
            strcat(disassembled_instructions[instruction_num], formatted_temp);
        }
        else if(current_instruction.addr_mode == &ABS){
            address++;
            lo = cpu_read_byte(ctx,address);
            address++;
            hi = cpu_read_byte(ctx,address);
            operand = (hi << 8) | lo;
            snprintf(formatted_temp,50," $%04X",operand);
            strcat(disassembled_instructions[instruction_num], formatted_temp);
            

        }
        else if(current_instruction.addr_mode == &ABX){
            address++;
            lo = cpu_read_byte(ctx,address);
            address++;
            hi = cpu_read_byte(ctx,address);
            operand = (hi << 8) | lo;
            snprintf(formatted_temp,50," $%04X, X",operand);
            strcat(disassembled_instructions[instruction_num], formatted_temp);
        }
        else if(current_instruction.addr_mode == &ABY){
            address++;
            lo = cpu_read_byte(ctx,address);
            address++;
            hi = cpu_read_byte(ctx,address);
            operand = (hi << 8) | lo;
            snprintf(formatted_temp,50," $%04X, Y",operand);
            strcat(disassembled_instructions[instruction_num], formatted_temp);
        }
        else if(current_instruction.addr_mode == &IND){
            address++;
            lo = cpu_read_byte(ctx,address);
            address++;
            hi = cpu_read_byte(ctx,address);
            operand = (hi << 8) | lo;
            snprintf(formatted_temp,50," ($%04X)",operand);
            strcat(disassembled_instructions[instruction_num], formatted_temp);
        }
        else if(current_instruction.addr_mode == &IZX){
            address++;
            operand = cpu_read_byte(ctx,address) & 0x00FF;
            snprintf(formatted_temp,50," ($%02X, X)",0x00FF & operand);
            strcat(disassembled_instructions[instruction_num], formatted_temp);
        }
        else if(current_instruction.addr_mode == &IZY){
            address++;
            operand = cpu_read_byte(ctx,address) & 0x00FF;
            snprintf(formatted_temp,50," ($%02X), Y",0x00FF & operand);
            strcat(disassembled_instructions[instruction_num], formatted_temp);
        }
        else if(current_instruction.addr_mode == &REL){
            address++;
            operand = cpu_read_byte(ctx,address) & 0x00FF;
            snprintf(formatted_temp,50," $%02X  [REL]",0x00FF & operand);
            strcat(disassembled_instructions[instruction_num], formatted_temp);
        }

        address++;
        instruction_num++;
    }

}