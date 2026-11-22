#pragma once
#include "display.h"
#include "input.h"

extern int selected;
extern int scroll_offset;

int menu(const char* items[], int count);
void menu_reset();