/*
 * Library: libfins
 * File:    src/fins_raw.c
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
 * The source file src/fins_raw.c contains routines to execute raw FINS
 * functions and store the result in a buffer for further processing.
 */

#include "fins.h"

/*
 * int finslib_raw( struct fins_sys_tp *sys, uint16_t command, unsigned char *buffer, size_t send_len, size_t *recv_len );
 *
 * The function finslib_raw() executes a FINS function and stores the result in
 * a caller provided buffer.
 */

int finslib_raw( struct fins_sys_tp *sys, uint16_t command, unsigned char *buffer, size_t send_len, size_t *recv_len ) {

	struct fins_command_tp fins_cmnd;
	size_t a;
	size_t bodylen;
	int retval;

	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( buffer      == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( recv_len    == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( *recv_len   <  1              ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	XX_finslib_init_command( sys, & fins_cmnd, (command >> 8) & 0xff, command & 0xff );

	bodylen = 0;

	for (a=0; a<send_len; a++) fins_cmnd.body[bodylen++] = buffer[a];

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen, true ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen > *recv_len ) return FINS_RETVAL_BODY_TOO_LONG;

	for (a=0; a<bodylen; a++) buffer[a] = fins_cmnd.body[a];
	*recv_len = bodylen;

	return FINS_RETVAL_SUCCESS;

}  /* finslib_raw */
