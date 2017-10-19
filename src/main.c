/**
 * @file main.c
 *
 * @author John Whitmore
 *
 * This file contains an example of a synchronous and asynchronous
 * implementation of a simple API function.
 *
 * This example is used in the YouTube Video:
 * "006 - cinnamonBun: Synchronous Vs Asynchronous Function"
 * (Episode 6 in the cinnamonBun Getting Started series)
 * https://youtu.be/UgQnLmEQNH0
 * 
 * This example builds on the previous episode of the series:
 * "005 - cinnamonBun: Hardware Timer Interrupt"
 * (Episode 5 in the cinnamonBun Getting Started series)
 * https://youtu.be/Yrnlxmp2it4
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
