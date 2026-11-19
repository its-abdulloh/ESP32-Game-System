#include "filesystem.h"
#include "HardwareSerial.h"
#include "WString.h"
#include <vector>


bool fs_init(){
    if (!LittleFS.begin(false)) {
        Serial.print("LittleFS mount failed");
        return false;
    }
    Serial.print("LittleFS mounted!");
    return true;
}

std::vector<String> fs_list_games(){
    std::vector<String> games;
    File dir = LittleFS.open("/roms");
    
    if (!dir || !dir.isDirectory()) {
        Serial.println("ROM directory not found");
        return games;
    }
    File entry = dir.openNextFile();
    while (entry  && gameCount < MAX_GAMES) {
        if(entry.isDirectory()){
            games.push_back(String(entry.name()));
        }
        entry.close();
        entry = dir.openNextFile();
    }
    dir.close();
    return games;
}