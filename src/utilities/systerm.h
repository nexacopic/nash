#ifndef SYSTERM_H
#define SYSTERM_H
#include "display/vga.h"

void initsysterm(struct limine_file *font);

void put_text(const char* txt);
void put_char(char ch);

void renderchar(int row, int col, char ch);
#endif

