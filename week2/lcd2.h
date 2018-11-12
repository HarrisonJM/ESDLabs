#include <msp430.h>

#ifndef LCD2_H__
#define LCD2_H__

extern char DisplayBuffer[96][96/8];

void LCD_Handler();

#endif
