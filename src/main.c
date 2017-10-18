/**
 * @file main.c
 *
 * @author John Whitmore
 *
 * This file contains a very simple example of driving the GPIO Output pin
 * of the cinnamonBun to "transmit" Morse code using an LED. The example is
 * coded in a Synchronous manner to demonstrate the limitations.
 *
 * The example is used in the YouTube video:
 * "004 - cinnamonBun: Synchronous Program"
 * (Episode 4 in the cinnamonBun Getting Started series)
 * https://youtu.be/br9gOcxSU1U
 *
 * This example builds on the previous episode of the series:
 * "03 - cinnamonBun: Hello World"
 * (Episode 3 in the cinnamonBun Getting Started series)
 * https://youtu.be/MWarznfr7ts
 *
 * Copyright 2017 John Whitmore <jwhitmore@electronicsoup.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the version 3 of the GNU General Public License
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <xc.h>
#include <stdint.h>

#include "morse.h"

int main(void)
{
    uint32_t delay;

    /*
     * GPIO Setup
     * Port D lower Byte Digital IO Pins
     */
    ANSELD = 0x00;
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 1;
        
    delay = 0;
    LATDbits.LATD0 = 1;
    
    while(1) {
        delay++;
        if(delay == 0x3ffff) {
            delay = 0;
            LATDbits.LATD0 = ~LATDbits.LATD0;
        }
        
        if(PORTDbits.RD2 == 0)
            morse_tx_the();
    }
    return(0);
}
