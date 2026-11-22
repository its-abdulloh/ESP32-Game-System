#include "filesystem.h"
#include "HardwareSerial.h"
#include "WString.h"
#include <vector>
#include <LittleFS.h>


bool fs_init(){
    if (!LittleFS.begin(false)) {
        Serial.print("LittleFS mount failed");
        return false;
    }
    Serial.print("LittleFS mounted!");
    return true;
}

std::vector<String> fs_list_games(){
    std::vector<String> gameslist;
    File dir = LittleFS.open("/roms");
    
    if (!dir || !dir.isDirectory()) {
        Serial.println("ROM directory not found");
        return gameslist;
    }
    File entry = dir.openNextFile();
    while (entry) {
        if(entry.isDirectory()){
            gameslist.push_back(String(entry.name()));
        }
        entry.close();
        entry = dir.openNextFile();
    }
    dir.close();
    return gameslist;
}