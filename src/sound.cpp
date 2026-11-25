#include "sound.h"
#include "esp32-hal-ledc.h"

#define BUZZER_PIN 32
const int frequency = 500; 

void sound_init(){
    ledcSetup(0, frequency, 8);
    ledcAttachPin(BUZZER_PIN, 0);
}

void sound_play(int soundtimer){
    if (soundtimer > 0) {
        ledcWriteTone(0, frequency);
    } else {
        ledcWriteTone(0, 0);
    }
}