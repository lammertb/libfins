/*
 * Library: libfins
 * File:    src/fins_05_02.c
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
 * The source file src/fins_05_02.c contains routines to request information
 * about modules on a remote PLC over the FINS protocol.
 */

#include <ctype.h>
#include "fins.h"

/*
 * int finslib_connection_data_read( struct fins_sys_tp *sys, struct, fins_unitdata_tp *data, uint8_t start_unit, size_t num_units );
 *
 * The function funslib_connection_data_read() reads the data of one or more
 * units connected to a remote PLC over the FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_connection_data_read( struct fins_sys_tp *sys, struct fins_unitdata_tp *unitdata, uint8_t start_unit, size_t *num_units ) {

	struct fins_command_tp fins_cmnd;
	size_t a;
	size_t b;
	size_t bodylen;
	int retval;
	bool last_data;

	if ( num_units   == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( *num_units  == 0              ) return FINS_RETVAL_SUCCESS;
	if ( *num_units  >  25             ) return FINS_RETVAL_BODY_TOO_LONG;
	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( unitdata    == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	XX_finslib_init_command( sys, & fins_cmnd, 0x05, 0x02 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = start_unit;
	fins_cmnd.body[bodylen++] = *num_units & 0xff;

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen ) ) != FINS_RETVAL_SUCCESS ) return retval;

	bodylen    = 2;
	last_data  = fins_cmnd.body[bodylen]   & 0x80;
	*num_units = fins_cmnd.body[bodylen++] & 0x7f;

	for (a=0; a<*num_units; a++) {

		unitdata[a].unit = fins_cmnd.body[bodylen++];
		for (b=0; b<20; b++) unitdata[a].model[b] = fins_cmnd.body[bodylen++];

		while ( b > 0  &&  isspace( unitdata[a].model[b-1] ) ) b--;
		unitdata[a].model[b] = 0; 
	}

	return (last_data) ? FINS_RETVAL_SUCCESS_LAST_DATA : FINS_RETVAL_SUCCESS;

}  /* finslib_connection_data_read */
