/*
 * Library: libfins
 * File:    src/fins_03_06.c
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
 * The source file src/fins_03_06.c contains routines to read the program area
 * from a remote PLC over the FINS protocol.
 */

#include "fins.h"

/*
 * int finslib_program_area_read( struct fins_sys_tp *sys, unsigned char *data, uint32_start_word, size_t *num_bytes );
 *
 * The function finslib_program_area_read() reads a block of data from the
 * program area of a remote PLC over the FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_program_area_read( struct fins_sys_tp *sys, unsigned char *data, uint32_t start_word, size_t *num_bytes ) {

	struct fins_command_tp fins_cmnd;
	size_t a;
	size_t bodylen;
	int retval;
	bool last_data;

	if ( num_bytes   == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( *num_bytes  == 0              ) return FINS_RETVAL_SUCCESS;
	if ( *num_bytes  >  992            ) return FINS_RETVAL_BODY_TOO_LONG;
	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( data        == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	XX_finslib_init_command( sys, & fins_cmnd, 0x03, 0x06 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = 0xff;
	fins_cmnd.body[bodylen++] = 0xff;
	fins_cmnd.body[bodylen++] = (start_word >> 24) & 0xff;
	fins_cmnd.body[bodylen++] = (start_word >> 16) & 0xff;
	fins_cmnd.body[bodylen++] = (start_word >>  8) & 0xff;
	fins_cmnd.body[bodylen++] = (start_word      ) & 0xff;
	fins_cmnd.body[bodylen++] = (*num_bytes >>  8) & 0xff;
	fins_cmnd.body[bodylen++] = (*num_bytes      ) & 0xff;

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen, true ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen < 10 ) return FINS_RETVAL_BODY_TOO_SHORT;

	bodylen = 8;

	last_data    = fins_cmnd.body[bodylen]   & 0x80;

	*num_bytes   = fins_cmnd.body[bodylen++] & 0x7f;
	*num_bytes <<= 8;
	*num_bytes  += fins_cmnd.body[bodylen++];

	for (a=0; a<*num_bytes; a++) data[a] = fins_cmnd.body[bodylen++];

	return (last_data) ? FINS_RETVAL_SUCCESS_LAST_DATA : FINS_RETVAL_SUCCESS;

}  /* finslib_program_area_read */
