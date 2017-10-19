/**
 * @file morse.c
 *
 * @author John Whitmore
 *
 * This file contains the implementaiton of a very simple API Function to
 * transmit the Morse Code for the word "the" using an LED. The function
 * implementated as a synchronous function to demonstrate the limitations
 * of this style of code.
 *
 * The example is first used in the YouTube video:
 * "004 - cinnamonBun: Synchronous Program"
 * (Episode 4 in the cinnamonBun Getting Started series)
 * https://youtu.be/br9gOcxSU1U
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
