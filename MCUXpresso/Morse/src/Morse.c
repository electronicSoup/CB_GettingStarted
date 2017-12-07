/*
===============================================================================
 Name        : Morse.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here
#include "libmorse/morse.h"

// TODO: insert other definitions and declarations here
void morse_on(void);
void morse_off(void);

#if defined(__GNUC__) && defined(LPC1769)
int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
#endif
#endif

    // TODO: insert code here
    /*
     * Turn off the RGB Led
     */
    Chip_GPIO_Init(LPC_GPIO);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 22);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, 3, 26);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, 3, 25);
    Chip_GPIO_SetPinOutHigh(LPC_GPIO, 0, 22);
    Chip_GPIO_SetPinOutHigh(LPC_GPIO, 3, 26);
    Chip_GPIO_SetPinOutHigh(LPC_GPIO, 3, 25);

    // Force the counter to be placed into memory
    volatile static int i = 0 ;

    morse_init(morse_on, morse_off);

    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}
#endif  // __GNUC__ && LPC1769

void morse_on(void)
{

}

void morse_off(void)
{

}
