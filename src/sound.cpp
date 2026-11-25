#include "sound.h"
#include "esp32-hal-ledc.h"

#define BUZZER_PIN 32
const int frequency = 500; 

void sound_init(){
    ledcAttachPin(BUZZER_PIN,frequency);
}

void sound_play(int soundtimer){
    if (soundtimer > 0) {
        ledcWriteTone(BUZZER_PIN, frequency);
    } else {
        ledcWriteTone(BUZZER_PIN, 0);
    }
}