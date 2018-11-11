#ifndef LCD1_H__
#define LCD1_H__

#include <msp430.h>

extern const char Font[8][16];
extern const char Tank[8][4];

char reverse(char inchar);
void initDisplayBuffer(char setting);
void outputDisplayBuffer();
void buildScene(char numberToDisplay);

#endif
