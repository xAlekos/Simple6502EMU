#include "./cpu/cpu.h"
#include <raylib.h>




void draw_registers(cpu* ctx){
    BeginDrawing();
    DrawText("A:\n\n\n\nX:\n\n\n\nY:\n\n\n\nP:\n\n\n\nS:\n\n\n\n",850,10,40,WHITE);
    DrawText(TextFormat("%d\n\n\n\n%d\n\n\n\n%d\n\n\n\n$%X\n\n\n\n$%X",ctx->a,ctx->x,ctx->y,ctx->pc,ctx->sp),900,10,40,WHITE);
    EndDrawing();
    


}

int main(int argc, char** argv){
    cpu* ctx = cpu_init();
    InitWindow(1024,1024,"6502 emu");
    SetTargetFPS(60);
    ClearBackground(BLACK);

    while(!WindowShouldClose()){
        draw_registers(ctx);
        
    }
}













