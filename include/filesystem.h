#pragma once

#include "WString.h"
#include <Arduino.h>
#include <vector>

bool fs_init();
std::vector<String> fs_list_games();