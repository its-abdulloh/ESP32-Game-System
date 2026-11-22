#pragma once
#include <stdint.h>

#define MEMORY_SIZE 4*1024
#define WIDTH 128
#define HEIGHT 64


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

int main_chip8(const char *rom_name);

void save_rpl(Chip8 *chip8);