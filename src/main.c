/**
 * @file main.c
 *
 * @author John Whitmore
 *
 * This file contains an example of setting up a dsPIC33EP256MU806 Hardware
 * timer, and catching the resulting generated interrupt.
 *
 * This example is used in the YouTube Video:
 * "005 - cinnamonBun: Hardware Timer Interrupt"
 * (Episode 5 in the cinnamonBun Getting Started series)
 * https://youtu.be/Yrnlxmp2it4
 *
 * This example builds on the previous episode of the series:
 * "004 - cinnamonBun: Synchronous Program"
 * (Episode 4 in the cinnamonBun Getting Started series)
 * https://youtu.be/br9gOcxSU1U
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

void __attribute__((__interrupt__, __no_auto_psv__)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0;        // Clear the Interrupt flag
    TMR1 = 0x00;              // Reset the timer back to zero
    
    LATDbits.LATD1 = ~LATDbits.LATD1;
}

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
    
    /*
     * Timer Setup
     */
    T1CONbits.TCS = 0;       // Internal CPU Clock is input
    T1CONbits.TGATE = 0;     // Not a gated timer
    T1CONbits.TCKPS = 0b11;  // Divide by 256
    IPC0bits.T1IP = 0b111;   // Higest Prioirity
    IFS0bits.T1IF = 0;       // Clear the Interrupt flag
    IEC0bits.T1IE = 1;       // Enable Timer 1 Interrupt
    
    TMR1 = 0x00;             // Start counting at zero
    PR1 = 0x3fff;            // Count up to 0x3fff and then Interrupt
    T1CONbits.TON = 1;       // And turn on the timer
    
    delay = 0;
    LATDbits.LATD0 = 1;
    
    while(1) {
        delay++;
        if(delay == 0x3ffff) {
            delay = 0;
            LATDbits.LATD0 = ~LATDbits.LATD0;
        }
        
//        if(PORTDbits.RD2 == 0)
//            morse_tx_the();
    }
    return(0);
}
