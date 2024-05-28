#include "memory.h"

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

int load_rom_in_memory(mem* memory,char* path,uint16_t offset){
   
    size_t rom_size;
    size_t read_size;
    FILE* rom = fopen(path,"rb");
    if(rom == NULL)
        return 1;
    fseek(rom,0,SEEK_END);
    rom_size = ftell(rom);
    rewind(rom);

    fread(memory->bytes + offset ,1,rom_size,rom);
    fclose(rom);
    return 0;
}