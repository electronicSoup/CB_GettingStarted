/**
 * @file morse.h
 *
 * @author John Whitmore
 *
 * This file contains the public API declarations of Functions provided by
 * the Morse Module implemeted in morse.c.
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

/*
 * The module's initilaisation function, morse_init(), must be called prior to
 * using any of the other API functions of this module. Failure to do so will
 * result in unknown behaviour.
 */
extern void morse_init(void);
extern void morse_tx(char *);
