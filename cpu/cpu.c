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

//Abbiamo accorpato IMP ed A come addressing mode
uint8_t IMP(cpu* ctx)
{
    ctx->fetched = ctx->a;
    return 0;
}


uint8_t IMM(cpu *ctx)
{
    ctx->operand_addr = ctx->pc;
    ctx->pc++;
    return 0;
}

uint8_t ZP0(cpu *ctx){

    ctx->operand_addr = cpu_read_byte(ctx,ctx->pc);
    ctx->pc++;
    ctx->operand_addr &= 0x00FF;
    return 0;
}

uint8_t ZPX(cpu *ctx){

    ctx->operand_addr = cpu_read_byte(ctx,ctx->pc) + ctx->x;
    ctx->pc++;
    ctx->operand_addr &= 0x00FF;
    return 0;
}

uint8_t ZPY(cpu *ctx){
    
    ctx->operand_addr = cpu_read_byte(ctx,ctx->pc) + ctx->y;
    ctx->pc++;
    ctx->operand_addr &= 0x00FF;
    return 0;
}

// TODO VERIFICA
uint8_t ABS(cpu *ctx){
    
    uint8_t lo = cpu_read_byte(ctx,ctx->pc);
    ctx->pc++;

    uint8_t hi = cpu_read_byte(ctx,ctx->pc);
    ctx->pc++;

    ctx->operand_addr= (hi << 8) | lo;

    return 0;
}

uint8_t ABX(cpu *ctx){

    uint8_t lo = cpu_read_byte(ctx,ctx->pc);
    ctx->pc++;

    uint8_t hi = cpu_read_byte(ctx,ctx->pc);
    ctx->pc++;
    
    ctx->operand_addr= (hi << 8) | lo;
    ctx->operand_addr += ctx->x;

    if((ctx->operand_addr & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

uint8_t ABY(cpu *ctx){

    uint8_t lo = cpu_read_byte(ctx,ctx->pc);
    ctx->pc++;

    uint8_t hi = cpu_read_byte(ctx,ctx->pc);
    ctx->pc++;
    
    ctx->operand_addr= (hi << 8) | lo;
    ctx->operand_addr += ctx->y;

    if((ctx->operand_addr & 0xFF00) != (hi << 8))
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

    ctx->operand_addr = (hi << 8) | lo;

    //TODO AGGIUNGERE BUG
    return 0;
}
