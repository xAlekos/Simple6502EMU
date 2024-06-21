#include "./cpu/cpu.h"
#include "./system/system.h"
#include <raylib.h>


void draw_registers(cpu* ctx){
    BeginDrawing();
    DrawText("A:\n\nX:\n\nY:\n\nP:\n\nS:\n\n",850,10,22,WHITE);
    DrawText(TextFormat("$%X [%d]\n\n$%X [%d]\n\n$%X [%d]\n\n$%04X\n\n$%X",ctx->a,ctx->a,ctx->x,ctx->x,ctx->y,ctx->y,ctx->pc,ctx->sp),900,10,22,WHITE);
    DrawText(TextFormat("Rel Addr: %d",ctx->rel_addr),600,100,22,WHITE);
    EndDrawing();

}

void draw_status_register(cpu* ctx){
    BeginDrawing();
    int ypos = 10;
    DrawText("C",800,ypos,22,get_flag(ctx,C) == true ? GREEN : WHITE);
    ypos+=24;
    DrawText("Z",800,ypos,22,get_flag(ctx,Z) == true ? GREEN : WHITE);
    ypos+=24;
    DrawText("I",800,ypos,22,get_flag(ctx,I) == true ? GREEN : WHITE);
    ypos+=24;
    DrawText("D",800,ypos,22,get_flag(ctx,D) == true ? GREEN : WHITE);
    ypos+=24;
    DrawText("B",800,ypos,22,get_flag(ctx,B) == true ? GREEN : WHITE);
    ypos+=24;
    DrawText("-",800,ypos,22,get_flag(ctx,U) == true ? GREEN : WHITE);
    ypos+=24;
    DrawText("V",800,ypos,22,get_flag(ctx,V) == true ? GREEN : WHITE);
    ypos+=24;
    DrawText("N",800,ypos,22,get_flag(ctx,N) == true ? GREEN : WHITE);
}

void draw_memory_page(cpu* ctx,uint8_t page){
    int ypos = 42;
    int posx = 64;
    DrawText(TextFormat("$%02X",page),22,10,22,WHITE);
    
    for(int i = 0 ; i<16;i++){
        DrawText(TextFormat("%02X",i),posx,10,22,WHITE);
        posx+=32;
    }
    
    for(int i = 0 ; i<256;i+=16){
        DrawText(TextFormat("%02X",i),24,ypos,22,WHITE);
        ypos+=32;
    }

    posx = 64;
    ypos = 42;


    for(int i = 0 ; i<256;i++){
        DrawText(TextFormat("%02X",ctx->memory->bytes[(page*256+i) & 0x07FF ]),posx,ypos,22,WHITE);
        posx+=32;
        if((i + 1) % 16 == 0){
            ypos +=32;
            posx=64;
        } 
    }


}

void draw_cycles(cpu* ctx){

    DrawText(TextFormat("Cicli Istruzione: %d",ctx->remaining_cycles),580,10,22,WHITE);

}

void draw_disassembled_code(cpu* ctx){
    int ypos = 250;
    disassemble(ctx,ctx->pc,ctx->pc + 128);
    for(int i = 0; i < 25; i++){
        DrawText(disassembled_instructions[i],800,ypos,22,WHITE);
        ypos+=22;
    }
}

int main(int argc, char** argv){
    cpu* ctx = cpu_init();
    nes_system* sys = system_init();
    cpu_connect_system(ctx,sys);
    uint8_t page = 0x0000;

    InitWindow(1024,1024,"6502 emu");
    SetTargetFPS(300);
    ClearBackground(BLACK);
    reset(ctx);
    load_rom_in_memory(ctx->memory,"./roms/r",0x0000);
    ctx->pc = 0x0000;
    while(!WindowShouldClose()){
        ClearBackground(BLACK);
        draw_registers(ctx);
        draw_status_register(ctx);
        draw_memory_page(ctx,page);
        draw_disassembled_code(ctx);
        draw_cycles(ctx);
        if(IsKeyPressed(KEY_LEFT))
            page--;
        if(IsKeyPressed(KEY_RIGHT))
            page++;
        if(IsKeyPressed(KEY_SPACE) || ctx->remaining_cycles > 0)
            clock(ctx);
    }
}













