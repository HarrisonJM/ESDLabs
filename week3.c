#include <msp430.h>

#include "lcd1.h"

void enter_button_state(int new_state);

#define BUTTON_NULL 0
#define BUTTON_RELEASED 1
#define BUTTON_PRESSED 1
#define BUTTON_HELD 3

#define INTERVAL_BUTTON_HANDLER 10
#define MINIMUM_BUTTON_INTERACTION_TIME 100
#define BUTTON_HELD_TIME 200

#define LED_OFF 0
#define LED_ON 1
#define LED_PERIOD 50


int led_state = LED_OFF;
int led_period = 0;

int counter_button_handler = 0;
int number_of_button_presses = 0;
int button_state = BUTTON_RELEASED;

int counter_10ms = 0;

int button_state;
int number_of_button_presses;
int press_time;
int releaseTime = 0;

void init_button_handler()
{
    button_state = BUTTON_NULL;
    number_of_button_presses = 0;
    press_time = 0;
    releaseTime = 0;
}

void button_handler()
{
    switch (button_state)
    {
        case BUTTON_RELEASED:
            P1OUT &= (0xFF-0x01); // Set P1.0 off using AND
            if ((P1IN & 0x02) == 0)
            {
                if (releaseTime >= (MINIMUM_BUTTON_INTERACTION_TIME/INTERVAL_BUTTON_HANDLER))
                {
                    releaseTime = 0;
                    button_state = BUTTON_PRESSED;
                }
            }
            break;
        case BUTTON_PRESSED:
            press_time++;
            P1OUT |= 0x01; // Set P1.0 on using OR
            if ((P1IN & 0x02) != 0)
            {
                if (press_time >= (MINIMUM_BUTTON_INTERACTION_TIME/INTERVAL_BUTTON_HANDLER))
                {
                    press_time = 0;
                    number_of_button_presses++;
                }
                button_state = BUTTON_RELEASED;
            }
            else
            {
                if (press_time >= (BUTTON_HELD_TIME/INTERVAL_BUTTON_HANDLER))
                {
                    press_time = 0;
                    number_of_button_presses++;
                    button_state = BUTTON_HELD;
                }
            }
            break;
        case BUTTON_HELD:
            press_time++;
            if ((P1IN & 0x02) != 0)
            {
                press_time = 0;
                button_state = BUTTON_RELEASED;
            }
            else
            {
                if (press_time >= (BUTTON_HELD_TIME/INTERVAL_BUTTON_HANDLER))
                {
                    press_time = 0;
                    number_of_button_presses++;
                }
            }
            break;
        default:
            break;
    }
}

void led_handler()
{
    led_period++;
    if (led_period >= LED_PERIOD)
    {
        led_period = 0;
        switch (led_state)
        {
        case LED_OFF:
            if (number_of_button_presses > 0)
            {
                number_of_button_presses--;
                P1OUT |= 0x01; // Set P1.0 on using OR
                led_state = LED_ON;
            }
            else
            {
                led_state = LED_ON;
            }
            break;
        case LED_ON:
            P1OUT &= (0xFF-0x01); // Set P1.0 off using AND
            led_state = LED_OFF;
            break;
        default:
            break;
        }
    }
}

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

int main(void)
{
    initOther();

    for(;;)
    {
        button_handler();
    }
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

    counter_10ms++;
    if (counter_10ms >= 10)
    {
        counter_10ms = 0;
        button_handler();
        led_handler();
    }

    counter_button_handler ++;
    if (counter_button_handler >= INTERVAL_BUTTON_HANDLER)
    {
        counter_button_handler = 0;
        button_handler();
        led_handler();
    }
}
