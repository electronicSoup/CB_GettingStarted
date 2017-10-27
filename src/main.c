/**
 * @file main.c
 *
 * @author John Whitmore
 *
 * This file contains the code to exercise the improved Morse API which accepts
 * a null terminated string, instead of hard coding "the".
 *
 * This example is used in the YouTube Video:
 * "011 - Platform Independence"
 * (Episode 11 in the cinnamonBun Getting Started series)
 * https://youtu.be/FAVVIP_ayYw
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

void morse_on(void);
void morse_off(void);

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
	
	morse_init(morse_on, morse_off);
	
	while(1) {
		delay++;
		if(delay == 0x3ffff) {
			delay = 0;
			LATDbits.LATD0 = ~LATDbits.LATD0;
		}
        
		if(PORTDbits.RD2 == 0)
			morse_tx("the quick brown fox.");
    }
    return(0);
}

void morse_on(void)
{
	LATDbits.LATD1 = 1;
}

void morse_off(void)
{
	LATDbits.LATD1 = 0;
}

