/*
 * Library: libfins
 * File:    src/fins_07_01.c
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
 * The source file src/fins_07_01.c contains the code to execute a FINS 07 01
 * command. This is the command to read the clock from a remote PLC.
 */

#include "fins.h"

/*
 * int finslib_clock_read( fins_sys_tp *sys, fins_datetime_tp *datetime );
 *
 * The function finslib_clock_read() reads the clock of a remotely connected PLC
 * and returns that information in a structure. The clock is read with the
 * FINS command 07 01.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_clock_read( struct fins_sys_tp *sys, struct fins_datetime_tp* datetime ) {

	struct fins_command_tp fins_cmnd;
	int retval;
	size_t bodylen;

	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( datetime    == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	XX_finslib_init_command( sys, & fins_cmnd, 0x07, 0x01 );

	bodylen = 0;

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen, true ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen != 9 ) return FINS_RETVAL_BODY_TOO_SHORT;

	bodylen = 2;

	datetime->year   = finslib_bcd_to_int( fins_cmnd.body[bodylen++], FINS_DATA_TYPE_BCD16 ) + 1900;
	datetime->month  = finslib_bcd_to_int( fins_cmnd.body[bodylen++], FINS_DATA_TYPE_BCD16 );
	datetime->day    = finslib_bcd_to_int( fins_cmnd.body[bodylen++], FINS_DATA_TYPE_BCD16 );
	datetime->hour   = finslib_bcd_to_int( fins_cmnd.body[bodylen++], FINS_DATA_TYPE_BCD16 );
	datetime->min    = finslib_bcd_to_int( fins_cmnd.body[bodylen++], FINS_DATA_TYPE_BCD16 );
	datetime->sec    = finslib_bcd_to_int( fins_cmnd.body[bodylen++], FINS_DATA_TYPE_BCD16 );
	datetime->dow    = finslib_bcd_to_int( fins_cmnd.body[bodylen++], FINS_DATA_TYPE_BCD16 );

	if ( datetime->year < 1998 ) datetime->year += 100;

	return FINS_RETVAL_SUCCESS;

}  /* finslib_clock_read */
