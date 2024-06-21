#include "system.h"

mem* cpu_mem_init(){
    
    mem* new_mem = (mem*)calloc(1,sizeof(mem));
    return new_mem;

}

nes_system* system_init()
{
    nes_system* new_system = (nes_system*)calloc(1,sizeof(nes_system));
    new_system->cpu_ram = cpu_mem_init();
    return new_system;
}

uint8_t mem_read_byte(mem* memory, uint16_t address){
    
    if (address >= 0x0000 && address <= 0x1FFF)
        return memory->bytes[address & 0x07FF];

    else if(address >= 0x2000 && address <= 0x3FFF)
        return ppu_read_cpu(address & 0x07FF);
}

void mem_write_byte(mem* memory, uint16_t address, uint8_t value){

    if (address >= 0x0000 && address <= 0x1FFF)
        memory->bytes[address & 0x07FF] = value;

    else if(address >= 0x2000 && address <= 0x3FFF)
        ppu_write_cpu(address & 0x07FF,value);

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
    return rom_size;
}

void load_cartridge(nes_system* sys,cartridge* cart){
    
    sys->rom = cart;
    ppu_connect_cartridge(sys->ppu_ctx,cart);
}