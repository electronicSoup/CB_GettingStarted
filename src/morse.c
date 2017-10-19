/**
 * @file morse.c
 *
 * @author John Whitmore
 *
 * This file contains the implementation of Morse API Functions. This is the 
 * "good" version of the API where the module does not impose a policy on the 
 * application code.
 *
 * The example is first used in the YouTube video:
 * "007 - Circular Buffer implementation in C"
 * (Episode 7 in the cinnamonBun Getting Started series)
 * https://youtu.be/I8xatqqza6s
 *
 * The file is used in subsiquent episodes in the cinnamonBun Getting Started
 * playlist.
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
#include <stdint.h>
#include <stddef.h>

#define MORSE_TX_BUFFER_SIZE  10

static char tx_buffer[MORSE_TX_BUFFER_SIZE];
static uint16_t  tx_buffer_write_index = 0;
static uint16_t  tx_buffer_read_index = 0;
static uint16_t  tx_buffer_count = 0;

void morse_init(void)
{
}

void morse_tx(char *msg)
{
	char *ptr = msg;
	
	while (*ptr) {
		/*
		 * Check that the tx buffer isn't full aready.
		 */
		if ( tx_buffer_count < MORSE_TX_BUFFER_SIZE ) {
			
			/*
			 * Check for a valid character
			 */
			if( (*ptr == ' ') || (*ptr >= 'a' && *ptr <= 'z') ) {
				tx_buffer[tx_buffer_write_index] = *ptr;
				tx_buffer_write_index = (tx_buffer_write_index + 1) % MORSE_TX_BUFFER_SIZE;
				tx_buffer_count++;
			}
			
			ptr++;
		}
	}
}

