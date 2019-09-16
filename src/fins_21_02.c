/*
 * Library: libfins
 * File:    src/fins_21_02.c
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
 * The source file src/fins_21_02.c contains routines to read the error log
 * from a remote PLC over the FINS protocol.
 */

#define FINS_SOCKET_INCLUDE	1

#include "fins.h"

/*
 * int finslib_error_log_read( struct fins_sys_tp *sys, struct fins_errordata_tp, *errordata, uint16_t start_record, size_t *num_records, size_t *stored_records );
 *
 * The function finslib_error_log_read() reads a block of error log codes from
 * the error log in a remote PLC over the FINS protocol. The function takes the
 * number of records to read, and writes back in the same location the actual
 * number of records read. Optional the function can also provide the total
 * number of records waiting in the error log if a non NULL parameter is
 * specified to indicate where to store them.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_error_log_read( struct fins_sys_tp *sys, struct fins_errordata_tp *errordata, uint16_t start_record, size_t *num_records, size_t *stored_records ) {

	struct fins_command_tp fins_cmnd;
	size_t bodylen;
	size_t a;
	int retval;

	if ( num_records  == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( *num_records == 0              ) return FINS_RETVAL_SUCCESS;
	if ( *num_records >  20             ) return FINS_RETVAL_BODY_TOO_LONG;
	if ( sys          == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( errordata    == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd  == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	XX_finslib_init_command( sys, & fins_cmnd, 0x21, 0x02 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = (start_record >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (start_record     ) & 0xff;
	fins_cmnd.body[bodylen++] = (*num_records >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (*num_records     ) & 0xff;

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen, true ) ) != FINS_RETVAL_SUCCESS ) return retval;

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

		errordata[a].error_code[0]   = fins_cmnd.body[bodylen++];
		errordata[a].error_code[0] <<= 8;
		errordata[a].error_code[0]  += fins_cmnd.body[bodylen++];

		errordata[a].error_code[1]   = fins_cmnd.body[bodylen++];
		errordata[a].error_code[1] <<= 8;
		errordata[a].error_code[1]  += fins_cmnd.body[bodylen++];

		errordata[a].min             = finslib_bcd_to_int( fins_cmnd.body[bodylen++], FINS_DATA_TYPE_BCD16 );
		errordata[a].sec             = finslib_bcd_to_int( fins_cmnd.body[bodylen++], FINS_DATA_TYPE_BCD16 );
		errordata[a].day             = finslib_bcd_to_int( fins_cmnd.body[bodylen++], FINS_DATA_TYPE_BCD16 );
		errordata[a].hour            = finslib_bcd_to_int( fins_cmnd.body[bodylen++], FINS_DATA_TYPE_BCD16 );
		errordata[a].year            = finslib_bcd_to_int( fins_cmnd.body[bodylen++], FINS_DATA_TYPE_BCD16 ) + 1900;
		errordata[a].month           = finslib_bcd_to_int( fins_cmnd.body[bodylen++], FINS_DATA_TYPE_BCD16 );

		if ( errordata[a].year < 1998 ) errordata[a].year += 100;
	}

	return FINS_RETVAL_SUCCESS;

}  /* finslib_error_log_read */
