#include <Arduino.h>
#include <cstring>
#include "Wire.h"
#include "esp32-hal.h"
#include "pgmspace.h"
#include "display.h"
#include "input.h"
#include "menu.h"
#include "filesystem.h"
#include "flappy.h"
#include "meow.h"
#include "chip8launcher.h"


const char* games[] = {"Meow", "Flappy bird","Chip8"};
int gamesLen = sizeof(games)/sizeof(games[0]);
// put function declarations here:

void setup() {
  Wire.begin();
  randomSeed(analogRead(10));
  //initilize buttons
  input_init();
  fs_init();
  Serial.begin(115200);
  Serial.println("ESP32 STARTED");
  display.setBusClock(400000);
  display.begin();
  display.clearBuffer();
  display.setFont(u8g2_font_tiny5_tf);
  display.setCursor(40,34);
  display.printf("Game-system");
  display.sendBuffer();
  delay(1000);
}

void loop() {
  int selected = menu(games,gamesLen);
  if (selected!=-1){
    if(strcmp(games[selected],"Flappy bird")==0){
      while (1) {
        FlappyBird::play();
      }
    }
    if (strcmp(games[selected], "Meow")==0) {
      while (1) {
        Meow::play();
      }
    }
    if (strcmp(games[selected],"Chip8")==0) {
      menu_chip8();
      menu_reset();
    }
  }

  delay(100);
}

// put function definitions here:
