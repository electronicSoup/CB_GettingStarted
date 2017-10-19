/**
 * @file morse_sync.c
 *
 * @author John Whitmore
 *
 * This file contains the synchronous implementation of the API function
 * morse_tx_the().
 *
 * This is NOT a good design of an API function as it is very tightly coupled
 * to the application, or application specific, but it's just being used to
 * illustrate a synchronous function call.
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

#define DELAY for(delay = 0x00; delay < 0x3ffff; delay++) Nop();
uint32_t delay;

static void tx_dot(void);
static void tx_dash(void);
static void tx_end_of_character_delay(void);
static void tx_end_of_word_delay(void);

void morse_tx_the(void)
{
    tx_dash();
    tx_end_of_character_delay();
    
    tx_dot();
    tx_dot();
    tx_dot();
    tx_dot();
    tx_end_of_character_delay();
    
    tx_dot();
    tx_end_of_word_delay();
}

static void tx_dot(void)
{
    LATDbits.LATD1 = 1;
    DELAY
    LATDbits.LATD1 = 0;
    DELAY
}

static void tx_dash(void)
{
    LATDbits.LATD1 = 1;
    DELAY
    DELAY
    DELAY
    LATDbits.LATD1 = 0;
    DELAY
}

static void tx_end_of_character_delay(void)
{
    DELAY
    DELAY    
}

static void tx_end_of_word_delay(void)
{
    DELAY
    DELAY
    DELAY
    DELAY
    DELAY
    DELAY    
}
