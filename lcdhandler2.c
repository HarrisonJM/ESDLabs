#include <msp430.h>
#include "lcd2.h"
#include "lcd1.h"

int display_state = 0;
char line;
char column;

void LCD_Handler()
{
    switch (display_state)
    {
    case 0: // build scene
        buildScene('c');
        display_state = 1;
        break;

    case 1: // display scene
        if (UCB0IFG & UCTXIFG)
        {
            if (line == 0)
            {
                P2OUT |= 0x10;                  // Enable LCD CS
                UCB0TXBUF = 0x80;
                column = 0;
                line++;
            }
            else if ((line >= 1) && (line <= 96))
            {
                if (column == 0)
                {
                    UCB0TXBUF = reverse(line);
                    column++;
                }
                else if ((column >= 1) && (column <= 12))
                {
                    UCB0TXBUF =  DisplayBuffer[line-1][column-1];
                    column++;
                }
                else
                {
                    UCB0TXBUF = 0x00;
                    column = 0;
                    line++;
                }
            }
            else if (line == 97)
            {
                UCB0TXBUF = 0x00;
                line++;
            }
            else if (line == 98)
            {
                line++;
            }
            else
            {
                if ((UCB0STATW & UCBBUSY) == 0)
                {
                    //Ensure a 2us min delay to meet the LCD's thSCS
                    //__delay_cycles(16);
                    line = 0;
                    P2OUT &= ~0x10;                 // Disable LCD CS
                    display_state = 0;
                }
            }
        }
        break;

    default:
        display_state = 0;
        break;
    }
}

