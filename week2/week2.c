#include <msp430.h>
#include "../init.h"

int week2main(void)
{
    initOther();
    initScreen();

    for(;;)
    {
        // Char passed in here displays on the screen
        buildScene('1');
        outputDisplayBuffer();
    }
}
