/*
 * Library: libfins
 * File:    src/fins_21_40.c
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
 * The source file src/fins_21_40.c contains routines to read records from the
 * write access log file in a remote PLC over the FINS protocol.
 */

#include "fins.h"

int finslib_access_log_read( struct fins_sys_tp *sys, struct fins_accessdata_tp *accessdata, uint16_t start_record, size_t *num_records, size_t *stored_records ) {

	struct fins_command_tp fins_cmnd;
	size_t bodylen;
	size_t a;
	int retval;

	if ( num_records  == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( *num_records == 0              ) return FINS_RETVAL_SUCCESS;
	if ( *num_records >  20             ) return FINS_RETVAL_BODY_TOO_LONG;
	if ( sys          == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( accessdata   == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd  == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	fins_init_command( sys, & fins_cmnd, 0x21, 0x40 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = (start_record >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (start_record     ) & 0xff;
	fins_cmnd.body[bodylen++] = (*num_records >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (*num_records     ) & 0xff;

	if ( ( retval = _finslib_communicate( sys, & fins_cmnd, & bodylen ) ) != FINS_RETVAL_SUCCESS ) return retval;

	bodylen = 4;

	if ( stored_records != NULL ) {

		*stored_records   = fins_cmnd.body[bodylen++];
		*stored_records <<= 8;
		*stored_records  += fins_cmnd.body[bodylen++];
	}
	else bodylen += 2;

	*num_records   = fins_cmnd.body[bodylen++];
	*num_records <<= 8;
	*num_records  += fins_cmnd.body[bodylen++];

	for (a=0; a<*num_records; a++) {

		accessdata[a].network        = fins_cmnd.body[bodylen++];
		accessdata[a].node           = fins_cmnd.body[bodylen++];
		accessdata[a].unit           = fins_cmnd.body[bodylen++];

		bodylen++;

		accessdata[a].command_code   = fins_cmnd.body[bodylen++];
		accessdata[a].command_code <<= 8;
		accessdata[a].command_code  += fins_cmnd.body[bodylen++];

		accessdata[a].min            = fins_cmnd.body[bodylen++];
		accessdata[a].sec            = fins_cmnd.body[bodylen++];
		accessdata[a].day            = fins_cmnd.body[bodylen++];
		accessdata[a].hour           = fins_cmnd.body[bodylen++];
		accessdata[a].year           = fins_cmnd.body[bodylen++] + 1900;
		accessdata[a].month          = fins_cmnd.body[bodylen++];

		if ( accessdata[a].year < 1998 ) accessdata[a].year += 100;
	}

	return FINS_RETVAL_SUCCESS;

}  /* finslib_access_log_read */
