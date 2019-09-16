/*
 * Library: libfins
 * File:    src/fins_09_20_read.c
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
 * The source file src/fins_09_20_read.c contains routines to read messages
 * from a remote PLC over the FINS protocol.
 */

#define FINS_SOCKET_INCLUDE	1

#include <ctype.h>
#include "fins.h"

static uint8_t mask_array[] = { FINS_MSG_0, FINS_MSG_1, FINS_MSG_2, FINS_MSG_3, FINS_MSG_4, FINS_MSG_5, FINS_MSG_6, FINS_MSG_7 };

/*
 * int finslib_message_read( struct fins_sys_tp *, struct fins_msgdata_tp *msgdata, uint8_t msg_mask );
 *
 * The function finslib_message_read() reads messages from a remote PLC over
 * the FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_message_read( struct fins_sys_tp *sys, struct fins_msgdata_tp *msgdata, uint8_t msg_mask ) {

	struct fins_command_tp fins_cmnd;
	size_t a;
	size_t b;
	size_t msg_index;
	size_t bodylen;
	uint8_t recv_mask;
	int retval;

	if ( msg_mask    == 0x00           ) return FINS_RETVAL_SUCCESS;
	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( msgdata     == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	XX_finslib_init_command( sys, & fins_cmnd, 0x09, 0x20 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = 0x00;
	fins_cmnd.body[bodylen++] = msg_mask;

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen, true ) ) != FINS_RETVAL_SUCCESS ) return retval;

	msg_index = 0;
	bodylen   = 3;
	recv_mask = fins_cmnd.body[bodylen++];

	for (a=0; a<8; a++) {

		if ( ! (recv_mask & mask_array[a]) ) continue;

		msgdata[msg_index].msg = mask_array[a];
		for (b=0; b<32; b++) msgdata[msg_index].text[b] = fins_cmnd.body[bodylen++];

		while ( b > 0  &&  isspace( msgdata[msg_index].text[b-1] ) ) b--;
		msgdata[msg_index].text[b] = 0;

		msg_index++;
	}

	return FINS_RETVAL_SUCCESS;

}  /* finslib_message_read */
