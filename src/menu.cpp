#include "menu.h"
#include "esp32-hal-gpio.h"
#include "input.h"

int selected = 0;
int scroll_offset = 0;

const int visible_items = 6;
const int distance = 10;


int menu(const char* items[], int count){
    if (up_pressed() && selected > 0){
        selected--;
    }
    else if (down_pressed()  && selected < games_count-1) {
        selected++;
    }

    // Scroll down when selection moves below visible area
    if (selected >= scroll_offset + visible_items) {
        scroll_offset = selected - visible_items + 1;
    }

    // Scroll up when selection moves above visible area
    if (selected < scroll_offset) {
        scroll_offset = selected;
    }
    int x = 20;
    int y = 10;
    display.clearBuffer();
    for (int i=scroll_offset;i< scroll_offset+visible_items;i++){
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

    if (a_pressed()) {
        return selected;
    }
    return -1;
}

void menu_reset() {
    selected = 0;
    scroll_offset = 0;
}