/********************************************************************/
/***  LCD Handler                                                 ***/
/********************************************************************/
#include <msp430.h>

#include "lcd1.h"

const char Font[8][16] =
{
        //  0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x28, 0x08, 0x18, 0x3C, 0x10, 0x3C, 0x08, 0x3E, 0x3C, 0x3C, 0x18, 0x38, 0x38, 0x30, 0x3C, 0x3C},
        {0x44, 0x18, 0x24, 0x04, 0x10, 0x20, 0x10, 0x02, 0x24, 0x44, 0x24, 0x24, 0x44, 0x28, 0x20, 0x20},
        {0x44, 0x08, 0x08, 0x08, 0x20, 0x38, 0x20, 0x04, 0x18, 0x44, 0x24, 0x24, 0x40, 0x24, 0x38, 0x3C},
        {0x44, 0x08, 0x10, 0x18, 0x24, 0x04, 0x3C, 0x08, 0x18, 0x3C, 0x3C, 0x38, 0x40, 0x24, 0x20, 0x20},
        {0x44, 0x08, 0x10, 0x04, 0x3E, 0x04, 0x24, 0x10, 0x24, 0x04, 0x24, 0x24, 0x44, 0x24, 0x20, 0x20},
        {0x28, 0x3C, 0x3C, 0x38, 0x04, 0x38, 0x18, 0x10, 0x3C, 0x04, 0x42, 0x38, 0x38, 0x38, 0x3C, 0x20},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};

const char Tank[8][4] =
{
        {0x49, 0xFF, 0x82, 0x00},
        {0x49, 0x1C, 0x82, 0xFF},
        {0x49, 0x1C, 0xFE, 0x38},
        {0x7F, 0xFC, 0xFE, 0x38},
        {0x7F, 0x1C, 0xFE, 0x3F},
        {0x7F, 0x1C, 0x92, 0x38},
        {0x41, 0xFF, 0x92, 0x38},
        {0x41, 0x00, 0x92, 0xFF}
};

char DisplayBuffer[96][96/8];
int k = 0;

char reverse(char inchar)
{
    char outchar;

    outchar = 0;

    if ((inchar & BIT0) > 0) outchar |= BIT7;
    if ((inchar & BIT1) > 0) outchar |= BIT6;
    if ((inchar & BIT2) > 0) outchar |= BIT5;
    if ((inchar & BIT3) > 0) outchar |= BIT4;
    if ((inchar & BIT4) > 0) outchar |= BIT3;
    if ((inchar & BIT5) > 0) outchar |= BIT2;
    if ((inchar & BIT6) > 0) outchar |= BIT1;
    if ((inchar & BIT7) > 0) outchar |= BIT0;

    return outchar;
}


void initDisplayBuffer(char setting)
{
    int i;
    int j;

    for (i=0; i<96; i++)
    {
        for(j=0; j<12; j++)
        {
            DisplayBuffer[i][j] = setting;
        }
    }
}


void outputDisplayBuffer()
{
    int line;
    int column;
    char command = 0x80;            // Write lines

    command = command ^ 0x40;       // VCOM bit

    P2OUT |= 0x10;                  // LCD CS high

    while (!(UCB0IFG & UCTXIFG));
    UCB0TXBUF = command;

    for (line=0; line<96; line++)
    {
        while (!(UCB0IFG & UCTXIFG));
        UCB0TXBUF = reverse(line+1);

        for (column=0; column<12; column++)
        {
            while (!(UCB0IFG & UCTXIFG));
            UCB0TXBUF = DisplayBuffer[line][column];
        }

        while (!(UCB0IFG & UCTXIFG));
        UCB0TXBUF = 0x00;            // End of line signal
    }

    while (!(UCB0IFG & UCTXIFG));
    UCB0TXBUF = 0x00;               // End of block signal

    while ((UCB0STATW & UCBBUSY));

    __delay_cycles(4);              //Ensure a 2us min delay to meet the LCD's thSCS

    P2OUT &= ~0x10;                 // LCD CS low
}

void buildScene(char numberToDisplay)
{
    int i; //Iterates over columns
    int j; // iterates

    initDisplayBuffer(0xFF);
    // Displays the first 10 chracters fo the font
    for (i=0; i<8; i++)
    {
        for(j=0; j<10; j++)
        {
            DisplayBuffer[i+72][j] = ~Font[i][j];
        }
    }
    // Displays the last 6 characters of the font
    for (i=0; i<8; i++)
    {
        for(j=0; j<6; j++)
        {
            DisplayBuffer[i+80][j] = ~Font[i][j+10];
        }
    }

    // Tank directions
    for (i=0; i<8; i++)
    {
        DisplayBuffer[i+10][3] = ~Tank[i][0];
        DisplayBuffer[i+10][5] = ~Tank[i][1];
        DisplayBuffer[i+10][7] = ~Tank[i][2];
        DisplayBuffer[i+10][9] = ~Tank[i][3];
    }

    // Moves tank down
    // i iterates through array
    for (i=0; i<8; i++)
    {
        // To stopt he tank being weird we divide by 8 because the screen is broken up
        //into 8x8 pixel chunks across the 96x96 display.
        DisplayBuffer[i/8+k+10][3] = ~Tank[i][2];
        //k is speed/distance ratio thingy
        k = (k+1) % 160;
    }

    int charToPrint = 0;
    // Uses a switch to select the char to print
    switch(numberToDisplay)
    {
    case '0':
        charToPrint = 0;
        break;
    case '1':
        charToPrint = 1;
        break;
    }

    // indexes based on that
    for (i=0; i<8; i++)
    {
        for(j=0; j<charToPrint+1; j++)
        {
            // Adding 64 selects the 8th line (and the 0th column)
            DisplayBuffer[i+64][0] = ~Font[i][charToPrint];
        }
    }
}


/********************************************************************/
/***  End of LCD Handler                                          ***/
/********************************************************************/

