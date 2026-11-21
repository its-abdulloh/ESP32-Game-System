#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "instructions.h" //needs doing
#include "display.h" //needs doing
#include "sound.h" //needs doing

#define MEMORY_SIZE 4*1024
#define WIDTH 128
#define HEIGHT 64

#define FONTSET_SIZE 80
#define BIG_FONTSIZE 160
#define FONTSET_START_ADDRESS 0x50
#define FONT_BIG_START_ADDRESS 0xA0

#define ROM_START 0x200

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


uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

uint8_t big_fontset[BIG_FONTSIZE] = {
    // 0
    0x3C, 0x7E, 0xE7, 0xC3, 0xC3,
    0xC3, 0xC3, 0xE7, 0x7E, 0x3C,

    // 1
    0x18, 0x38, 0x58, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x3C,

    // 2
    0x3E, 0x7F, 0xC3, 0x06, 0x0C,
    0x18, 0x30, 0x60, 0xFF, 0xFF,

    // 3
    0x3C, 0x7E, 0xC3, 0x03, 0x0E,
    0x0E, 0x03, 0xC3, 0x7E, 0x3C,

    // 4
    0x06, 0x0E, 0x1E, 0x36, 0x66,
    0xC6, 0xFF, 0xFF, 0x06, 0x06,

    // 5
    0xFF, 0xFF, 0xC0, 0xC0, 0xFC,
    0xFE, 0x03, 0xC3, 0x7E, 0x3C,

    // 6
    0x3E, 0x7C, 0xE0, 0xC0, 0xFC,
    0xFE, 0xC3, 0xC3, 0x7E, 0x3C,

    // 7
    0xFF, 0xFF, 0x03, 0x06, 0x0C,
    0x18, 0x30, 0x60, 0x60, 0x60,

    // 8
    0x3C, 0x7E, 0xC3, 0xC3, 0x7E,
    0x7E, 0xC3, 0xC3, 0x7E, 0x3C,

    // 9
    0x3C, 0x7E, 0xC3, 0xC3, 0x7F,
    0x3F, 0x03, 0x03, 0x3E, 0x7C,

    // A-F are zero in the supplied SCHIP font
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00
};

uint16_t wrong_opcode=0;

Chip8 chip8;

const double instructionTime = 1.0 / 1000.0;
const double timerInterval = 1.0/ 60.0;
double accumulator = 0.0;
double timerAccumulator = 0.0;
const double frameTime = 1.0 / 60.0;
double frameAccumulator = 0.0;

int map_key(); //needs doing

void initialize(); //needs doing

void save_rpl(Chip8 *chip8);
void load_rpl(Chip8 *chip8);

int LoadROM(char const *filename); //needs doing

void fetch();

void decode_exec();

int main_chip8(char *rom_name){
    initialize();
    if (!LoadROM(rom_name)) {
        fprintf(stderr, "Failed to load ROM. Exiting.\n");
        return EXIT_FAILURE;
    }
    bool running = true;
    //needs doing
    // SDL_Event event;
    // Uint64 start = SDL_GetPerformanceCounter();
    // Uint64 frequency = SDL_GetPerformanceFrequency();
    while (running){
        //needs doing
        // Uint64 now = SDL_GetPerformanceCounter();
        // double deltaTime = (double)(now - start) / frequency;
        // start = now;
        accumulator += deltaTime;
        timerAccumulator += deltaTime;
        frameAccumulator += deltaTime;


        //needs doing
        //handle keys 
        // while (SDL_PollEvent(&event)) {
        //     if (event.type == SDL_QUIT) {
        //         running = false;
        //     }
        //     if (event.type == SDL_KEYDOWN && !event.key.repeat) {
        //         int key = map_key(event.key.keysym.scancode);

        //         if (key != -1)
        //             chip8.keypad[key] = 1;
        //     }
        //     if (event.type==SDL_KEYUP) {
        //         int key = map_key(event.key.keysym.scancode);

        //         if (key!=-1)
        //             chip8.keypad[key]=0;
        //     }
        // }

        while (accumulator >= instructionTime){
            fetch();
            decode_exec();
            accumulator -= instructionTime;
        }

        while (timerAccumulator >= timerInterval){
            if (chip8.delayTimer>0)
                chip8.delayTimer--;
            if (chip8.soundTimer>0)
                chip8.soundTimer--;
            sound_set_enabled(chip8.soundTimer > 0); //needs doing
            timerAccumulator -= timerInterval;
        }

        if (frameAccumulator >= frameTime) {
            if (chip8.draw_flag) {
                draw(chip8.video); //needs doing
                chip8.draw_flag = 0;
            }

            frameAccumulator -= frameTime;
        }

        SDL_Delay(1); //needs doing
    }
    display_quit();//needs doing
    return EXIT_SUCCESS;
}

void initialize(){
    chip8 = (Chip8){0};
    srand((unsigned int)time(NULL));

    for (unsigned int i=0;i<FONTSET_SIZE;i++){
        chip8.memory[FONTSET_START_ADDRESS+i] = fontset[i];
    }

    for (unsigned int i=0;i<BIG_FONTSIZE; i++) {
        chip8.memory[FONT_BIG_START_ADDRESS+i] = big_fontset[i];
    }

    load_rpl(&chip8);
    
    display_init(); //needs doing
    sound_init(); //needs doing
    return;
}

int LoadROM(char const *filename){
    FILE *file = fopen(filename,"rb");

    if (file==NULL){
        fprintf(stderr,"Failed to open ROM\n");
        return 0;
    }

    if (fseek(file, 0, SEEK_END)!=0){
        fclose(file);
        return 0;
    }

    long file_size = ftell(file);

    if (file_size<0){
        fclose(file);
        return 0;
    }

    if (file_size>MEMORY_SIZE - ROM_START) {
        fprintf(stderr,"ROM is too big!\n");
        fclose(file);
        return 0;
    }

    rewind(file);
    size_t bytes_read = fread(&chip8.memory[ROM_START], 1, (size_t) file_size, file);

    fclose(file);

    if (bytes_read!=(size_t)file_size){
        fprintf(stderr,"Failed to read complete ROM\n");
        return 0;
    }
    
    chip8.pc = ROM_START;

    return 1;
}

void fetch(){
    chip8.opcode = chip8.memory[chip8.pc]*256+chip8.memory[chip8.pc+1];
    chip8.pc+=2;
}

void decode_exec(){
    uint8_t op = (chip8.opcode & 0xF000)>>12;
    uint8_t x = (chip8.opcode & 0x0F00)>>8;
    uint8_t y = (chip8.opcode & 0x00F0)>>4;
    uint8_t n = chip8.opcode & 0x000F;
    uint8_t kk = chip8.opcode & 0x00FF;
    uint16_t nnn = chip8.opcode & 0x0FFF;

    switch (op) {
        case 0:
        if (kk==0xE0) {
            chip8_00E0(&chip8);
        }
        else if (kk==0xEE) {
            chip8_00EE(&chip8);
        }
        else if (kk==0xFF) {
            superchip_00FF(&chip8);
        }
        else if (kk==0xFE) {
            superchip_00FE(&chip8);
        }
        else if (kk==0xFB) {
            superchip_00FB(&chip8);
        }
        else if (kk==0xFC) {
            superchip_00FC(&chip8);
        }
        else if ((chip8.opcode & 0xFFF0) == 0x00C0) {
            superchip_00CN(&chip8, n);
        }
        break;
        case 1:
        chip8_1NNN(&chip8, nnn);
        break;
        case 2:
        chip8_2NNN(&chip8, nnn);
        break;
        case 3:
        chip8_3XKK(&chip8, x, kk);
        break;
        case 4:
        chip8_4XKK(&chip8, x, kk);
        break;
        case 5:
        if(n==0){
            chip8_5XY0(&chip8, x, y);
        }
        break;
        case 6:
        chip8_6XKK(&chip8, x, kk);
        break;
        case 7:
        chip8_7XKK(&chip8, x, kk);
        break;
        case 8:
        switch (n) {
            case 0:
            chip8_8XY0(&chip8, x, y);
            break;        
            case 1:
            chip8_8XY1(&chip8, x, y);
            break;
            case 2:
            chip8_8XY2(&chip8, x, y);
            break;
            case 3:
            chip8_8XY3(&chip8, x, y);
            break;
            case 4:
            chip8_8XY4(&chip8, x, y);
            break;
            case 5:
            chip8_8XY5(&chip8, x, y);
            break;
            case 6:
            chip8_8XY6(&chip8, x, y);
            break;
            case 7:
            chip8_8XY7(&chip8, x, y);
            break;
            case 0xE:
            chip8_8XYE(&chip8, x, y);
            break;
        }
        break;
        case 9:
        chip8_9XY0(&chip8, x, y);
        break;
        case 0xA:
        chip8_ANNN(&chip8, nnn);
        break;
        case 0xB:
        chip8_BNNN(&chip8,nnn);
        break;
        case 0xC:
        chip8_CXKK(&chip8, x, kk);
        break;
        case 0xD:
        chip8_DXYN(&chip8, x, y, n);
        break;
        case 0xE:
        if (kk==0x9E){
            chip8_EX9E(&chip8, x);
        }else if (kk==0xA1){
            chip8_EXA1(&chip8, x);
        }
        break;
        case 0xF:
            switch (kk) {
                case 0x07:
                chip8_FX07(&chip8, x);
                break;
                case 0x0A:
                chip8_FX0A(&chip8, x);
                break;
                case 0x15:
                chip8_FX15(&chip8, x);
                break;
                case 0x18:
                chip8_FX18(&chip8, x);
                break;
                case 0x1E:
                chip8_FX1E(&chip8, x);
                break;
                case 0x29:
                chip8_FX29(&chip8, x);
                break;
                case 0x30:
                superchip_FX30(&chip8, x);
                break;
                case 0x33:
                chip8_FX33(&chip8, x);
                break;
                case 0x55:
                chip8_FX55(&chip8, x);
                break;
                case 0x65:
                chip8_FX65(&chip8, x);
                break;
                case 0x75:
                superchip_FX75(&chip8, x);
                break;
                case 0x85:
                superchip_FX85(&chip8, x);
                break;
            }
        }
        break;
    }
}

void save_rpl(Chip8 *chip8) {
    File file = LittleFS.open("/chip8_rpl.bin", "w");

    if (!file) return;

    file.write(chip8->rpl_flags, 16);

    file.close();
}

void load_rpl(Chip8 *chip8) {
    File file = LittleFS.open("/chip8_rpl.bin", "r");

    if (!file) {
        memset(chip8->rpl_flags, 0, 16);
        return;
    }

    size_t count = file.read(chip8->rpl_flags, 16);

    file.close();

    if (count != 16) {
        memset(chip8->rpl_flags, 0, 16);
    }
}