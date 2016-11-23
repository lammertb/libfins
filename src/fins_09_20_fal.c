/*
 * Library: libfins
 * File:    src/fins_09_20_fal.c
 * Author:  Lammert Bies
 *
 * This file is licensed under the MIT License as stated below
 *
 * Copyright (c) 2016 Lammert Bies
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
 * The source file src/fins_09_20_fal.c contains routines to read FAL or FALS
 * messages from a remote PLC over the FINS protocol.
 */

#include <ctype.h>
#include "fins.h"

/*
 * int finslib_message_fal_fals_read( struct fins_sys_tp *sys, char *faldata, uint16_t fal_number );
 *
 * The function finslib_message_fal_fals_read() reads a user program generated
 * failure message from the FAL or FALS command from a remote PLC using the
 * FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_message_fal_fals_read( struct fins_sys_tp *sys, char *faldata, uint16_t fal_number ) {

	struct fins_command_tp fins_cmnd;
	size_t bodylen;
	size_t a;
	int retval;

	if ( fal_number  <  1              ) return FINS_RETVAL_NO_READ_ADDRESS;
	if ( fal_number  >  511            ) return FINS_RETVAL_NO_READ_ADDRESS;
	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( faldata     == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	fins_init_command( sys, & fins_cmnd, 0x09, 0x20 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = 0x80 | ((fal_number >> 8) & 0x3f);
	fins_cmnd.body[bodylen++] =         (fal_number     ) & 0xff;

	if ( ( retval = _finslib_communicate( sys, & fins_cmnd, & bodylen ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen != 20 ) return FINS_RETVAL_BODY_TOO_SHORT;

	bodylen = 4;

	for (a=0; a<16; a++) faldata[a] = fins_cmnd.body[bodylen++];

	while ( a > 0  &&  isspace( faldata[a-1] ) ) a--;
	faldata[a] = 0;

	return FINS_RETVAL_SUCCESS;

}  /* finslib_message_fal_fals_read */
