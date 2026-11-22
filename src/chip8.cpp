#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <esp_timer.h>
#include <LittleFS.h>
#include "chip8instructions.h"
#include "display.h"
// #include "sound.h" //needs doing

#define MEMORY_SIZE 4*1024
#define WIDTH 128
#define HEIGHT 64

#define FONTSET_SIZE 80
#define BIG_FONTSIZE 160
#define FONTSET_START_ADDRESS 0x50
#define FONT_BIG_START_ADDRESS 0xA0

#define ROM_START 0x200


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

// int map_key(); //needs doing

void initialize();

void save_rpl(Chip8 *chip8);
void load_rpl(Chip8 *chip8);

int LoadROM(char const *filename);

void fetch();

void decode_exec();

void draw();

int main_chip8(const char *rom_name){
    initialize();
    if (!LoadROM(rom_name)) {
        fprintf(stderr, "Failed to load ROM. Exiting.\n");
        return EXIT_FAILURE;
    }
    bool running = true;
    uint64_t start = esp_timer_get_time();
    uint64_t frequency = 1000000; // 1 MHz (microseconds)
    while (running){
        int64_t now = esp_timer_get_time();
        double deltaTime = (double)(now - start) / 1000000.0;
        start = now;
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
            // sound_set_enabled(chip8.soundTimer > 0); //needs doing
            timerAccumulator -= timerInterval;
        }

        if (frameAccumulator >= frameTime) {
            if (chip8.draw_flag) {
                draw(chip8.video);
                chip8.draw_flag = 0;
            }

            frameAccumulator -= frameTime;
        }
        delay(1);
    }
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
    
    // sound_init(); //needs doing
    return;
}

int LoadROM(char const *rom_name){
    char filepath[128];
    snprintf(filepath, sizeof(filepath), "/roms/%s", rom_name);
    File dir = LittleFS.open(filepath);

    if (!dir || !dir.isDirectory()) {
        return 0;
    }

    File file = dir.openNextFile();
    while (file) {
        if (!file.isDirectory()) {
            String filename = file.name();

            if (filename.endsWith(".ch8")) {
                break;
            }
        }

        file.close();
        file = dir.openNextFile();
    }

    dir.close();

    if (!file) {
        Serial.println("No .ch8 ROM found");
        return 0;
    }

    size_t file_size = file.size();

    if (file_size > MEMORY_SIZE - ROM_START) {
        Serial.println("ROM is too big!");
        file.close();
        return 0;
    }

    size_t bytes_read = file.read(
        &chip8.memory[ROM_START],
        file_size
    );

    file.close();

    if (bytes_read != file_size) {
        Serial.println("Failed to read complete ROM");
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

void draw(uint8_t video[]) {
    display.clearBuffer();

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

            int index = y * WIDTH + x;

            if (video[index] == 1) {
                display.drawPixel(x, y);
            }
        }
    }

    display.sendBuffer();
}