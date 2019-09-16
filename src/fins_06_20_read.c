/*
 * Library: libfins
 * File:    src/fins_06_20_read.c
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
 * The source file src/fins_06_20_read.c contains the routines to perform the
 * client command 06 20 on FINS. This command is used to measure the minimum,
 * average and maximum cycle times of a remote PLC.
 */

#include "fins.h"

/*
 * int finslib_cycle_time_read( fins_sys_tp *sys, fins_cycletime_tp *cyc_time );
 *
 * The function finslib_cycle_time_read() reads the minimum, average and maximum
 * cycle times of a remote PLC.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_cycle_time_read( struct fins_sys_tp *sys, struct fins_cycletime_tp *cyc_time ) {

	struct fins_command_tp fins_cmnd;
	uint32_t avg;
	uint32_t min;
	uint32_t max;
	size_t bodylen;
	int retval;

	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( cyc_time    == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	XX_finslib_init_command( sys, & fins_cmnd, 0x06, 0x20 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = 0x01;

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen, true ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen != 14 ) return FINS_RETVAL_BODY_TOO_SHORT;

	avg   = fins_cmnd.body[2];
	avg <<= 8;
	avg  += fins_cmnd.body[3];
	avg <<= 8;
	avg  += fins_cmnd.body[4];
	avg <<= 8;
	avg  += fins_cmnd.body[5];

	max   = fins_cmnd.body[6];
	max <<= 8;
	max  += fins_cmnd.body[7];
	max <<= 8;
	max  += fins_cmnd.body[8];
	max <<= 8;
	max  += fins_cmnd.body[9];

	min   = fins_cmnd.body[10];
	min <<= 8;
	min  += fins_cmnd.body[11];
	min <<= 8;
	min  += fins_cmnd.body[12];
	min <<= 8;
	min  += fins_cmnd.body[13];

	cyc_time->avg = avg;
	cyc_time->min = min;
	cyc_time->max = max;

	return FINS_RETVAL_SUCCESS;

}  /* finslib_cycle_time_read */
