#include "menu.h"
#include "esp32-hal-gpio.h"
#include "input.h"

const char* games[] = {"Meow", "Flappy bird"};
// std::string games[] = {"Meow", "Flappy bird", "Tasks", "Games", "Clock", "Reader"};
int games_count = sizeof(games)/sizeof(games[0]);
int selected = 0;


void update_selected(){
    if (up_pressed() && selected > 0){
        selected--;
    }
    else if (down_pressed()  && selected < games_count-1) {
        selected++;
    }
}

void draw_menu(){
    int x = 20;
    int y = 10;
    int distance = 10;
    display.clearBuffer();
    for (int i=0;i<games_count;i++){
        if (i==selected){
            display.setCursor(x,y);
            display.print(games[i]);
            //print selected
            display.setCursor(x-5,y);
            display.print(">");
            y +=distance; 
        }
        else {
            display.setCursor(x,y);
            display.print(games[i]);
            y +=distance;
        }
    }
    display.sendBuffer();
}

int menu(){
    update_selected();
    draw_menu();
    if (a_pressed()) {
        return selected;
    }
    return -1;
}