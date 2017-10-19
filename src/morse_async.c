/**
 * @file morse_async.c
 *
 * @author John Whitmore
 *
 * This file contains the asynchronous implementation of the API function
 * morse_tx_the().
 *
 * This is NOT a good design of an API function as it is very tightly coupled
 * to the application, or application specific, but it's just being used to
 * illustrate an asynchronous function call.
 *
 * The example is used in the YouTube video:
 * "006 - cinnamonBun: Synchronous Vs Asynchronous Function"
 * (Episode 6 in the cinnamonBun Getting Started series)
 * https://youtu.be/UgQnLmEQNH0
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

#define DOT_DURATION   0x3FFF

enum element {
	dot,
	dash,
	inter_element,
	inter_char,
	inter_word,
	msg_end,
};

static enum element tx_buffer[20];
static uint8_t tx_index;

static void step(void);
static void start_timer(enum element e);

void __attribute__((__interrupt__, __no_auto_psv__)) _T3Interrupt(void)
{
	/*
	 * WARNING - 32 Timer Mode so Interrup is T3
	 *                            Control is T2
	 */
	IFS0bits.T3IF = 0;        // Clear the Interrupt flag
	T2CONbits.TON = 0;        // Turn timer off for the moment
	
	step();
}

void morse_tx_the(void)
{
	uint8_t index = 0;
	
	// T
	tx_buffer[index++] = dash;
	tx_buffer[index++] = inter_char;

	// H	
	tx_buffer[index++] = dot;
	tx_buffer[index++] = inter_element;
	tx_buffer[index++] = dot;
	tx_buffer[index++] = inter_element;
	tx_buffer[index++] = dot;
	tx_buffer[index++] = inter_element;
	tx_buffer[index++] = dot;
	tx_buffer[index++] = inter_char;
    
	// E
	tx_buffer[index++] = dot;
	tx_buffer[index++] = inter_word;
	
	tx_buffer[index++] = msg_end;
	
	tx_index = 0;
	
	step();
}

static void step(void)
{
	switch (tx_buffer[tx_index]) {
	case dot:
	case dash:
		LATDbits.LATD1 = 1;
		break;
		
	case inter_element:
	case inter_char:
	case inter_word:
		LATDbits.LATD1 = 0;
		break;

	case msg_end:
		return;
		break;
	}
	
	start_timer(tx_buffer[tx_index]);
	tx_index++;
}	

static void start_timer(enum element e)
{
	uint32_t duration;
	
	/*
         * Timer Setup
         */
	T2CONbits.TCS = 0;       // Internal CPU Clock is input
	T2CONbits.TGATE = 0;     // Not a gated timer
	T2CONbits.TCKPS = 0b11;  // Divide by 256
	T2CONbits.T32 = 1;       // 32 Bit Timer Mode
	IPC2bits.T3IP = 0b111;   // Highest Priority
	IFS0bits.T3IF = 0;       // Clear the Interrupt flag
	IEC0bits.T3IE = 1;       // Enable Timer 3 Interrupt
    
	TMR2 = 0x00;             // Start counting at zero
	TMR3 = 0x00;             // Start counting at zero
	
	switch (e) {
	case dot:
	case inter_element:
		duration = DOT_DURATION;
		break;
		
	case dash:
	case inter_char:
		duration = (uint32_t) 3 * DOT_DURATION;
		break;

	case inter_word:
		duration = (uint32_t) 7 * DOT_DURATION;
		break;

	case msg_end:
		return;
		break;
	}
	
	PR2 = (uint16_t)(duration & 0xffff);          // LSW
	PR3 = (uint16_t)((duration >> 16) & 0xffff);  // MSW
	
	T2CONbits.TON = 1;       // And turn on the timer    
}

