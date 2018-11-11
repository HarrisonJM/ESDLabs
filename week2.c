#include <msp430.h>

#include "lcd1.h"

void initOther()
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    // init

    PM5CTL0 &= ~LOCKLPM5; // Do to drive ports

    P1DIR |= 0x01; // P1.0 = out, port 1 bit 0
    P4DIR |= 0x40; // port 4 bit 6 P4 (0010-0000 except flipped)

    P1DIR &= ~0x02; // P1.1 input (switch 2)
    P1REN |= 0x02; // P1.1 pull up resistor

    TA0CCR0 = 1024;
    TA0CCTL0 = 0x10;
    TA0CTL = TASSEL_2 + MC_1;

    _BIS_SR(GIE);
}
void initScreen()
{
    // LCD
    P4DIR  |= 0x04;
    // Set P4.2 to output direction (LCD Power On)
    P4DIR  |= 0x08;
    // Set P4.3 to output direction (LCD Enable)
    // SPI Ports
    P1SEL0 &= ~0x40;
    // Set P1.6 to output direction (SPI MOSI)
    P1SEL1 |= 0x40;
    // Set P1.6 to output direction (SPI MOSI)
    P1DIR  |= 0x40;
    // Set P1.6 to output direction (SPI MOSI)
    P2SEL0 &= ~0x04;
    // Set P2.2 to SPI mode (SPI CLK)
    P2SEL1 |= 0x04;
    // Set P2.2 to SPI mode (SPI CLK)
    P2DIR  |= 0x04;
    // Set P2.2 to output direction (SPI CLK)
    P2DIR  |= 0x10;
    // Set P2.4 to output direction (SPI CS)
    // SPI Interface
    UCB0CTLW0 |= UCSWRST;
    UCB0CTLW0 &= ~(UCCKPH + UCCKPL + UC7BIT + UCMSB);
    UCB0CTLW0 &= ~(UCSSEL_3);
    UCB0CTLW0 |= UCSSEL__SMCLK;
    UCB0BRW    = 8;
    UCB0CTLW0 |= (UCMSB + UCCKPH + 0x00 + UCMST + UCSYNC + UCMODE_0);
    UCB0CTLW0 &= ~(UCSWRST);
    P4OUT |= 0x04;
    // Turn LCD Power On
    P4OUT |= 0x08;
    // Enable LCD
    P1OUT &= ~0x01;
    // Set P1.0 off (Green LED)
}

int main(void)
{
    initOther();
    initScreen();

    for(;;)
    {
        buildScene('1');
        outputDisplayBuffer();
    }
}
