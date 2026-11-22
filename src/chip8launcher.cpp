#include "chip8.h"
#include "WString.h"
#include "display.h"
#include "input.h"
#include "filesystem.h"
#include "menu.h"
#include <vector>


void menu_chip8(){
    std::vector<String> gameslist = fs_list_games();
    int gamescount = sizeof(gameslist)/sizeof(gameslist[0]);
    std::vector<const char*> items;
    for (const String& game : gameslist) {
        items.push_back(game.c_str());
    }
    menu_reset();
    while (!b_pressed()) {
        int selected = menu(items.data(),gamescount);
        if (selected!=-1){
            main_chip8(gameslist[selected].c_str());
        }
    }
}


