#include "input.h"
#include "esp32-hal-gpio.h"

#define BUTTON1 13
#define BUTTON2 15
#define BUTTON3 17

int lastbtnstate1 = HIGH;
int lastbtnstate2 = HIGH;
int lastbtnstate3 = HIGH;

void input_init(){
    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);
    pinMode(BUTTON3, INPUT_PULLUP);
}

bool up_pressed(){
    int btnState = digitalRead(BUTTON1);
    if(btnState==LOW && lastbtnstate1==HIGH){
        lastbtnstate1 = btnState;
        return true;
    }
    lastbtnstate1 = btnState;
    return false;
}

bool down_pressed(){
    int btnState = digitalRead(BUTTON2);
    if(btnState==LOW && lastbtnstate2==HIGH){
        lastbtnstate2 = btnState;
        return true;
    }
    lastbtnstate2 = btnState;
    return false;
}

bool a_pressed(){
    int btnState = digitalRead(BUTTON3);
    if(btnState==LOW && lastbtnstate3==HIGH){
        lastbtnstate3 = btnState;
        return true;
    }
    lastbtnstate3 = btnState;
    return false;
}