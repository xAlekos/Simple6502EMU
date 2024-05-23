#include "cpu.h"


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

        uint8_t another_cycle_1 = (instrunctions_table[ctx->cur_opcode].addr_mode());
        uint8_t another_cycle_2 = (instrunctions_table[ctx->cur_opcode].op());

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
    set_flag(ctx,V,tmp & 0x00);

    return 0;
}

uint8_t DEC(cpu* ctx){

    fetch(ctx);
    uint8_t tmp = ctx->fetched - 1;
    cpu_write_byte(ctx,ctx->abs_addr,tmp);
    set_flag(ctx,N,tmp & 0x80);
    set_flag(ctx,V,tmp & 0x00);

    return 0;
}

uint8_t DEX(cpu* ctx){

    ctx->x--;
    set_flag(ctx,N,ctx->x & 0x80);
    set_flag(ctx,V,ctx->x & 0x00);

    return 0;
}

uint8_t DEY(cpu* ctx){

    ctx->y--;
    set_flag(ctx,N,ctx->y & 0x80);
    set_flag(ctx,V,ctx->y & 0x00);

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

uint8_t LDY(cpu *ctx){

    fetch(ctx);
    ctx->y = ctx->fetched;

    set_flag(ctx,N, ctx->y & 0x80);
    set_flag(ctx,Z, ctx->y == 0x00);

    return 0;
}

uint8_t LSR(cpu *ctx)
{
    fetch(ctx);

    set_flag(ctx,C,ctx->fetched & 0x01);
    
    uint16_t tmp = ctx->fetched >> 1;

    set_flag(ctx,Z,(tmp & 0X00FF) == 0X0000);
    set_flag(ctx,N,(tmp & 0X00FF) & 0x0080);


    if(instrunctions_table[ctx->cur_opcode].addr_mode == &IMP)
        ctx->a = tmp & 0x00FF;
    else
        cpu_write_byte(ctx,ctx->abs_addr,tmp & 0X00FF);

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
