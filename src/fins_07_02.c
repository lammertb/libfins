/*
 * Library: libfins
 * File:    src/fins_07_02.c
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
 * The source file src/fins_07_02.c contains routines to set the internal clock
 * of a remote PLC over the FINS protocol.
 */

#define FINS_SOCKET_INCLUDE	1

#include "fins.h"

/*
 *
 * 
 * The function finslib_clock_write() sets the internal clock of a remote PLC
 * to a desired value over the FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_clock_write( struct fins_sys_tp *sys, const struct fins_datetime_tp *datetime, bool do_sec, bool do_day_of_week ) {

	size_t bodylen;
	int retval;
	struct fins_command_tp fins_cmnd;

	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( datetime    == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	if ( datetime->year  < 1998  ||  datetime->year  > 2097 ) return FINS_RETVAL_INVALID_DATE;
	if ( datetime->month <    1  ||  datetime->month >   12 ) return FINS_RETVAL_INVALID_DATE;
	if ( datetime->day   <    1  ||  datetime->day   >   31 ) return FINS_RETVAL_INVALID_DATE;
	if ( datetime->hour  <    0  ||  datetime->hour  >   23 ) return FINS_RETVAL_INVALID_DATE;
	if ( datetime->min   <    0  ||  datetime->min   >   59 ) return FINS_RETVAL_INVALID_DATE;

	if ( do_sec ) {

		if (                      datetime->sec < 0  ||  datetime->sec > 59  ) return FINS_RETVAL_INVALID_DATE;
		if ( do_day_of_week  &&  (datetime->dow < 0  ||  datetime->dow >  6) ) return FINS_RETVAL_INVALID_DATE;
	}

	XX_finslib_init_command( sys, & fins_cmnd, 0x07, 0x02 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = finslib_int_to_bcd( datetime->year % 100, FINS_DATA_TYPE_BCD16 ) & 0xff;
	fins_cmnd.body[bodylen++] = finslib_int_to_bcd( datetime->month,      FINS_DATA_TYPE_BCD16 ) & 0xff;
	fins_cmnd.body[bodylen++] = finslib_int_to_bcd( datetime->day,        FINS_DATA_TYPE_BCD16 ) & 0xff;
	fins_cmnd.body[bodylen++] = finslib_int_to_bcd( datetime->hour,       FINS_DATA_TYPE_BCD16 ) & 0xff;
	fins_cmnd.body[bodylen++] = finslib_int_to_bcd( datetime->min,        FINS_DATA_TYPE_BCD16 ) & 0xff;

	if ( do_sec ) {

		fins_cmnd.body[bodylen++]                       = finslib_int_to_bcd( datetime->sec, FINS_DATA_TYPE_BCD16 ) & 0xff;
		if ( do_day_of_week ) fins_cmnd.body[bodylen++] = finslib_int_to_bcd( datetime->dow, FINS_DATA_TYPE_BCD16 ) & 0xff;
	}

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen, true ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen != 2 ) return FINS_RETVAL_BODY_TOO_SHORT;

	return FINS_RETVAL_SUCCESS;

}  /* finslib_clock_write */
