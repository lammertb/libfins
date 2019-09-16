/*
 * Library: libfins
 * File:    src/fins_09_20_clear.c
 * Author:  Lammert Bies
 *
 * This file is licensed under the MIT License as stated below
 *
 * Copyright (c) 2016-2019 Lammert Bies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Description
 * -----------
 * The source file src/fins_09_20_clear.c contains routines to clear messages
 * from a remote PLC over the FINS protocol.
 */

#define FINS_SOCKET_INCLUDE	1

#include <ctype.h>
#include "fins.h"

/*
 * int finslib_message_clear( struct fins_sys_tp *sys, uint8_t msg_mask );
 *
 * The function finslib_message_clear() clears messages from a remote PLC with
 * the FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_message_clear( struct fins_sys_tp *sys, uint8_t msg_mask ) {

	struct fins_command_tp fins_cmnd;
	size_t bodylen;
	int retval;

	if ( msg_mask    == 0x00           ) return FINS_RETVAL_SUCCESS;
	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	XX_finslib_init_command( sys, & fins_cmnd, 0x09, 0x20 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = 0x40;
	fins_cmnd.body[bodylen++] = msg_mask;

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen, true ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen != 2 ) return FINS_RETVAL_BODY_TOO_SHORT;

	return FINS_RETVAL_SUCCESS;

}  /* finslib_message_clear */
