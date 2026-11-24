#include "input.h"
#include <Arduino.h>
#include "esp32-hal-gpio.h"

#define BUTTON1 25
#define BUTTON2 26
#define BUTTON3 27
#define BUTTON4 33
#define BUTTON5 13
#define BUTTON6 14

int lastbtnstate1 = HIGH;
int lastbtnstate2 = HIGH;
int lastbtnstate3 = HIGH;
int lastbtnstate4 = HIGH;
int lastbtnstate5 = HIGH;
int lastbtnstate6 = HIGH;

void input_init(){
    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);
    pinMode(BUTTON3, INPUT_PULLUP);
    pinMode(BUTTON4, INPUT_PULLUP);
    pinMode(BUTTON5, INPUT_PULLUP);
    pinMode(BUTTON6, INPUT_PULLUP);
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

bool right_pressed(){
    int btnState = digitalRead(BUTTON2);
    if(btnState==LOW && lastbtnstate2==HIGH){
        lastbtnstate2 = btnState;
        return true;
    }
    lastbtnstate2 = btnState;
    return false;
}

bool down_pressed(){
    int btnState = digitalRead(BUTTON3);
    if(btnState==LOW && lastbtnstate3==HIGH){
        lastbtnstate3 = btnState;
        return true;
    }
    lastbtnstate3 = btnState;
    return false;
}

bool left_pressed(){
    int btnState = digitalRead(BUTTON4);
    if(btnState==LOW && lastbtnstate4==HIGH){
        lastbtnstate4 = btnState;
        return true;
    }
    lastbtnstate4 = btnState;
    return false;
}

bool a_pressed(){
    int btnState = digitalRead(BUTTON5);
    if(btnState==LOW && lastbtnstate5==HIGH){
        lastbtnstate5 = btnState;
        return true;
    }
    lastbtnstate5 = btnState;
    return false;
}


bool b_pressed(){
    int btnState = digitalRead(BUTTON6);
    if(btnState==LOW && lastbtnstate6==HIGH){
        lastbtnstate6 = btnState;
        return true;
    }
    lastbtnstate6 = btnState;
    return false;
}

bool up_held() {
    return digitalRead(BUTTON1) == LOW;
}

bool right_held() {
    return digitalRead(BUTTON2) == LOW;
}

bool down_held() {
    return digitalRead(BUTTON3) == LOW;
}

bool left_held() {
    return digitalRead(BUTTON4) == LOW;
}

bool a_held() {
    return digitalRead(BUTTON5) == LOW;
}

bool b_held() {
    return digitalRead(BUTTON6) == LOW;
}