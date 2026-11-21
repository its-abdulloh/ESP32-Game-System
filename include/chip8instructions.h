#pragma once
#include <stdint.h>
#include <stdlib.h>

typedef struct 
{
    //registers
    uint8_t V[16];
    //4096 bytes of memory
    uint8_t memory[MEMORY_SIZE];
    //index register
    uint16_t I;
    //program counter
    uint16_t pc;
    //stack
    uint16_t stack[16];
    //stack pointer
    uint8_t sp;
    //timers
    uint8_t delayTimer;
    uint8_t soundTimer;
    //keypad
    uint8_t keypad[16];
    //graphics
    uint8_t video[WIDTH*HEIGHT];
    //operation code
    uint16_t opcode;
    int draw_flag;
    int high_res;
    uint8_t rpl_flags[16];
} Chip8;

uint8_t randomByte();
void chip8_00E0(Chip8 *chip8);
void chip8_00EE(Chip8 *chip8);
void chip8_1NNN(Chip8 *chip8,uint16_t nnn);
void chip8_2NNN(Chip8 *chip8,uint16_t nnn);
void chip8_3XKK(Chip8 *chip8,uint8_t x,uint8_t kk);
void chip8_4XKK(Chip8 *chip8,uint8_t x,uint8_t kk);
void chip8_5XY0(Chip8 *chip8,uint8_t x,uint8_t y);
void chip8_6XKK(Chip8 *chip8,uint8_t x,uint8_t kk);
void chip8_7XKK(Chip8 *chip8,uint8_t x,uint8_t kk);
void chip8_8XY0(Chip8 *chip8,uint8_t x, uint8_t y);
void chip8_8XY1(Chip8 *chip8,uint8_t x, uint8_t y);
void chip8_8XY2(Chip8 *chip8,uint8_t x, uint8_t y);
void chip8_8XY3(Chip8 *chip8,uint8_t x, uint8_t y);
void chip8_8XY4(Chip8 *chip8,uint8_t x, uint8_t y);
void chip8_8XY5(Chip8 *chip8,uint8_t x, uint8_t y);
void chip8_8XY6(Chip8 *chip8,uint8_t x, uint8_t y);
void chip8_8XY7(Chip8 *chip8,uint8_t x, uint8_t y);
void chip8_8XYE(Chip8 *chip8,uint8_t x, uint8_t y);
void chip8_9XY0(Chip8 *chip8,uint8_t x,uint8_t y);
void chip8_ANNN(Chip8 *chip8,uint16_t nnn);
void chip8_BNNN(Chip8 *chip8, uint16_t nnn);
void chip8_CXKK(Chip8 *chip8,uint8_t x,uint8_t kk);
void chip8_EX9E(Chip8 *chip8, uint8_t x);
void chip8_EXA1(Chip8 *chip8, uint8_t x);
void chip8_FX07(Chip8 *chip8, uint8_t x);
void chip8_FX15(Chip8 *chip8, uint8_t x);
void chip8_FX18(Chip8 *chip8, uint8_t x);
void chip8_FX1E(Chip8 *chip8, uint8_t x);
void chip8_FX0A(Chip8 *chip8, uint8_t x);
void chip8_FX29(Chip8 *chip8, uint8_t x);
void chip8_FX33(Chip8 *chip8, uint8_t x);
void chip8_FX55(Chip8 *chip8, uint8_t x);
void chip8_FX65(Chip8 *chip8, uint8_t x);
void chip8_DXYN(Chip8 *chip8,uint8_t x,uint8_t y,uint8_t n);

//SUPERCHIP instructions
void superchip_00FF(Chip8 *chip8);
void superchip_00FE(Chip8 *chip8);
void superchip_00CN(Chip8 *chip8, uint8_t n);
void superchip_00FB(Chip8 *chip8);
void superchip_00FC(Chip8 *chip8);
void superchip_FX30(Chip8 *chip8,uint8_t x);
void superchip_FX75(Chip8 *chip8,uint8_t x);
void superchip_FX85(Chip8 *chip8,uint8_t x);
