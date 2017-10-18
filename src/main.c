/**
 * @file main.c
 *
 * @author John Whitmore
 *
 * This file contains simple example code to drive the GPIO pins of the
 * cinnamonBun. Basically illuminating LEDs by driving GPIO Output pins
 * and monitoring the status of a switch by reading a GPIO Input pin. 
 *
 * The example is used in the YouTube video:
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

#define DELAY for(delay = 0x00; delay < 0x3ffff; delay++) Nop();

int main(void)
{
    long delay;

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
        
        LATDbits.LATD1 = ~PORTDbits.RD2;
    }
    return(0);
}
