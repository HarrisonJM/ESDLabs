#define INTERVALTHRESH 200
#define RELEASETHRESH 150
#define HELDTIMER0 250
#define HELDTIMER1 500
#define HELDTIMER2 750
#define HELDTIMER3 1000

typedef enum pressState{
    NULL,
    RELEASED,
    PRESSED,
    HELD0,
    HELD1,
    HELD2,
    HELD3
} PRESSSTATE_E;

PRESSSTATE_E currentState;
int pressedTimer = 0;
int releasedTimer = 0;
int counter = 0;

void counterTimer()
{
    switch(newState)
    {
    case NULL:
        break;
    case RELEASED:
        break;
    case PRESSED:
        break;
    case HELD0:
        break;
    case HELD1:
        break;
    case HELD2:
        break;
    case HELD3:
        break;
    }
}

void newButtonState(PRESSSTATE_E newState)
{
    switch(newState)
    {
    case NULL:
        break;
    case RELEASED:
        break;
    case PRESSED:
        break;
    case HELD0:
        break;
    case HELD1:
        break;
    case HELD2:
        break;
    case HELD3:
        break;
    }
}

void ReleasedTimer()
{

}

void PressedEnter()
{

}

void PressedTimer()
{

}

void HELD4Enter()
{

}

void HELD4Timer()
{

}

void HELD3Enter()
{

}

void HELD3Enter()
{

}

void HELD2Enter()
{

}

void HELD2Timer()
{

}

void HELD1ENTER()
{

}

void HELD1Timer()
{

}

void incrementCounter
{
    if (counter > 999)
        counter = 0;
    else
        counter++;
}
