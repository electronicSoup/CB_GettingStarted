/**
 * @file morse.c
 *
 * @author John Whitmore
 *
 * This file contains the implementation of Morse API Functions. This is the 
 * "good" version of the API where the module does not impose a policy on the 
 * application code.
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
#include <stdint.h>
#include <stddef.h>

/*
 * Definitions for the Transmit Circular Buffer
 */
#define MORSE_TX_BUFFER_SIZE  10

static char tx_buffer[MORSE_TX_BUFFER_SIZE];
static uint16_t  tx_buffer_write_index = 0;
static uint16_t  tx_buffer_read_index = 0;
static uint16_t  tx_buffer_count = 0;

/*
 * Definitions for the Binary Tree Structure representation of Morse Code
 * 
 * This structure is used to create a binary tree structure representing 
 * the Morse character set, check out the image on wikipedia:
 * https://en.wikipedia.org/wiki/Morse_code#/media/File:Morse_code_tree3.png 
 * Each node in the tree stores the character represented by the node, ch, as
 * well as the three pointers that identify adjacent nodes in the tree.
 */
struct character
{
	char ch;
	struct character *parent;
	struct character *dot;
	struct character *dash;
};

/*
 * Memory allocation for the nodes making up the binary tree structure.
 */
static struct character alphabet['z' - 'a' + 1];   // Plus 1 for Root Node

/*
 * MACRO to help calculating a node's index in the array given the character,
 * for example INDEX('a')
 */
#define INDEX(x)  ((x - 'a') + 1)                  // Plus 1 for Root Node

/*
 * Transmitter state:
 * 
 * This enumerated type is required to keep track of the transmitter's state so
 * it isn't clobbered by multiple requests to transmit.
 */
enum transmitter_state {
	tx_idle,
	transmitting_element,
	transmitting_space,
};

/*
 * Local variable to record the current state of the transmitter
 */
static enum transmitter_state tx_state;

/*
 * Buffer, 1 byte, to store the elements, Dot and Dash, which make up the 
 * current character being transmitted, by the module.
 * 
 * tx_elements      -  000001011
 * tx_elements_mask -       ^ 
 * 
 * The mask points to the current element being transmitted. The convention is 
 * that a 0 in the elements buffer represents a Dot element and a 1 represents
 * a Dash element. So in the example above the character is made up of _ . _ _
 * or the character 'y' 
 */
static uint8_t tx_elements;
static uint8_t tx_elements_mask;

/*
 * Function pointers to the functions which turn the Morse channel on and off.
 * These are passed into the initialisation function of the module, 
 * morse_init().
 */
static void (*channel_on)(void) = NULL;
static void (*channel_off)(void) = NULL;

/*
 * Function prototypes for local private functions.
 */
static void tx_char(char ch);

/*
 * The Code
 */
void morse_init(void (*fn_on)(void), void (*fn_off)(void))
{
	uint8_t loop;

	channel_on = fn_on;
	channel_off = fn_off;
	
	alphabet[0].ch = 0x00;
	
	for(loop = 'a'; loop <= 'z'; loop++) {
		alphabet[INDEX(loop)].ch = loop;
		alphabet[INDEX(loop)].parent = NULL;
		alphabet[INDEX(loop)].dot    = NULL;
		alphabet[INDEX(loop)].dash   = NULL;
	}
	
	/*
	 * The Initial element of the Array [0] is the Root Node
	 */
	alphabet[0].parent = NULL;
	alphabet[0].dot    = &alphabet[INDEX('e')];
	alphabet[0].dash   = &alphabet[INDEX('t')];

	alphabet[INDEX('a')].parent = &alphabet[INDEX('e')];
	alphabet[INDEX('a')].dot    = &alphabet[INDEX('r')];
	alphabet[INDEX('a')].dash   = &alphabet[INDEX('w')];

	alphabet[INDEX('b')].parent = &alphabet[INDEX('d')];
	
	alphabet[INDEX('c')].parent = &alphabet[INDEX('k')];
	
	alphabet[INDEX('d')].parent = &alphabet[INDEX('n')];
	alphabet[INDEX('d')].dot    = &alphabet[INDEX('b')];
	alphabet[INDEX('d')].dash   = &alphabet[INDEX('x')];

	alphabet[INDEX('e')].parent = &alphabet[0];  // Root Node
	alphabet[INDEX('e')].dot    = &alphabet[INDEX('i')];
	alphabet[INDEX('e')].dash   = &alphabet[INDEX('a')];

	alphabet[INDEX('f')].parent = &alphabet[INDEX('u')];

	alphabet[INDEX('g')].parent = &alphabet[INDEX('m')];
	alphabet[INDEX('g')].dot    = &alphabet[INDEX('z')];
	alphabet[INDEX('g')].dash   = &alphabet[INDEX('q')];

	alphabet[INDEX('h')].parent = &alphabet[INDEX('s')];

	alphabet[INDEX('i')].parent = &alphabet[INDEX('e')];
	alphabet[INDEX('i')].dot    = &alphabet[INDEX('s')];
	alphabet[INDEX('i')].dash   = &alphabet[INDEX('u')];

	alphabet[INDEX('j')].parent = &alphabet[INDEX('w')];

	alphabet[INDEX('k')].parent = &alphabet[INDEX('n')];
	alphabet[INDEX('k')].dot    = &alphabet[INDEX('c')];
	alphabet[INDEX('k')].dash   = &alphabet[INDEX('y')];

	alphabet[INDEX('l')].parent = &alphabet[INDEX('r')];

	alphabet[INDEX('m')].parent = &alphabet[INDEX('t')];
	alphabet[INDEX('m')].dot    = &alphabet[INDEX('g')];
	alphabet[INDEX('m')].dash   = &alphabet[INDEX('o')];

	alphabet[INDEX('n')].parent = &alphabet[INDEX('t')];
	alphabet[INDEX('n')].dot    = &alphabet[INDEX('d')];
	alphabet[INDEX('n')].dash   = &alphabet[INDEX('k')];

	alphabet[INDEX('o')].parent = &alphabet[INDEX('m')];

	alphabet[INDEX('p')].parent = &alphabet[INDEX('w')];
	
	alphabet[INDEX('q')].parent = &alphabet[INDEX('g')];
	
	alphabet[INDEX('r')].parent = &alphabet[INDEX('a')];
	alphabet[INDEX('r')].dot    = &alphabet[INDEX('l')];

	alphabet[INDEX('s')].parent = &alphabet[INDEX('i')];
	alphabet[INDEX('s')].dot    = &alphabet[INDEX('h')];
	alphabet[INDEX('s')].dash   = &alphabet[INDEX('v')];

	alphabet[INDEX('t')].parent = &alphabet[0];       // Root Node
	alphabet[INDEX('t')].dot    = &alphabet[INDEX('n')];
	alphabet[INDEX('t')].dash   = &alphabet[INDEX('m')];

	alphabet[INDEX('u')].parent = &alphabet[INDEX('i')];
	alphabet[INDEX('u')].dot    = &alphabet[INDEX('f')];

	alphabet[INDEX('v')].parent = &alphabet[INDEX('s')];

	alphabet[INDEX('w')].parent = &alphabet[INDEX('a')];
	alphabet[INDEX('w')].dot    = &alphabet[INDEX('p')];
	alphabet[INDEX('w')].dash   = &alphabet[INDEX('j')];

	alphabet[INDEX('x')].parent = &alphabet[INDEX('d')];

	alphabet[INDEX('y')].parent = &alphabet[INDEX('k')];

	alphabet[INDEX('z')].parent = &alphabet[INDEX('g')];
	
	/*
	 * Initialise the transmitter state
	 */
	tx_state = tx_idle;

	/*
	 */
	tx_elements = 0x00;
	tx_elements_mask = 0x00;
}

void morse_tx(char *msg)
{
	char *ptr = msg;

	/*
	 * If the transmitter is currently idle just transmit the first valid
	 * character directly without buffering it.
	 */
	if((tx_buffer_count == 0) && (tx_state == tx_idle)) {
		/*
		 * Ignore all the invalid characters at the start of the string.
		 */
		while (*ptr && (*ptr < 'a' || *ptr > 'z')) {
			ptr++;
		}
		if(*ptr) tx_char(*ptr++);
	}
	
	while (*ptr) {
		/*
		 * Check that the tx buffer isn't full already.
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

static void tx_char(char ch)
{
	struct character *child;
	struct character *parent;
	
	if(tx_elements_mask != 0x00) {
		/*
		 * Todo Error condition
		 */
		return;
	}
	
	tx_elements = 0x00;
	tx_elements_mask = 0b0001;
	
	child = &alphabet[INDEX(ch)];
	parent = child->parent;
	
	while(parent) {
		if(parent->dash == child) {
			/*
			 * Dash element
			 */
			tx_elements = tx_elements | tx_elements_mask;
		}
		
		tx_elements_mask = tx_elements_mask << 1;
		
		child = parent;
		parent = child->parent;
	}

	tx_elements_mask = tx_elements_mask >> 1;
	
	/*
	 * Turn on the channel and start a timer
	 */
	if(channel_on) channel_on();
	
	if(tx_elements & tx_elements_mask) {
		// Transmit Dash - start timer accordingly
	} else {
		// Transmit Dot - start timer accordingly		
	}
}
