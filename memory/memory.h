#include "../utils/utils.h"

typedef struct memory{

    uint8_t bytes[0x10000];

}mem;


mem* mem_init(){
    
    mem* new_mem = (mem*)calloc(1,sizeof(mem));
    return new_mem;

}


uint8_t mem_read_byte(mem* memory, uint16_t address){

    return memory->bytes[address];
}

void mem_write_byte(mem* memory, uint16_t address, uint8_t value){

    memory->bytes[address] = value;

}