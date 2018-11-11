#include <msp430.h> 

void __LEDRedToggle()
{
    P4OUT ^= 0x40; //Toggle
}

void __LEDGreenOFF()
{
    P1OUT &= (0xFF-0x01);
}

void __LEDGreenON()
{
    P1OUT |= 0x01;
}

int __ButtonReadS2()
{
    int bucket = 0;
    int returnVal = 1;

    while((P1IN & 0x02) == 0)
    {
        bucket++;
        if(bucket > 100)
        {
            returnVal = 0;
        }
    }

    return returnVal;
}

int (*ButtonReadS2)() = __ButtonReadS2;
void (*LEDRedToggle)() = __LEDRedToggle;
void (*LEDGreenOFF)() = __LEDGreenOFF;
void (*LEDGreenON)() = __LEDGreenON;

int heartbeat = 1;

/**
 * main.c
 */
int mainOld(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	// init

	PM5CTL0 &= ~LOCKLPM5; // Do to drive ports

	P1DIR |= 0x01; // P1.0 = out, port 1 bit 0
	P4DIR |= 0x40; // port 4 bit 6 P4 (0010-0000 except flipped)

	P1DIR &= ~0x02; // P1.1 input (switch 2)
	P1REN |= 0x02; // P1.1 pull up resistor

	// Timer init
	TA0CCR0 = 1024;
	TA0CCTL0 = 0x10;
	TA0CTL = TASSEL_2 + MC_1;

	_BIS_SR(GIE);

    //bg loop
	for(;;)
	{
	    if (ButtonReadS2() == 0) // on press/down
	    {
	        LEDGreenON();
	    }
	    else
	    {
	        LEDGreenOFF();
	    }
	}

	return 0;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0(void) // Timer0 A0 1ms interrupt service routine
{
    heartbeat++;
    // toggle on and off every 500ms
    if(heartbeat >= 500)
    {
        heartbeat = 1;
        LEDRedToggle();

    }
}
