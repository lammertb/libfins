/*
 * Library: libfins
 * File:    src/fins_23_01.c
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
 * The source file src/fins_23_01.c contains routines to set and reset forced
 * bits in a remote PLC over the FINS protocol.
 */

#define FINS_SOCKET_INCLUDE	1

#include "fins.h"

int finslib_force_bit( struct fins_sys_tp *sys, const struct fins_forcebit_tp *data, size_t num_bits ) {

	struct fins_command_tp fins_cmnd;
	struct fins_address_tp address;
	const struct fins_area_tp *area_ptr;
	int32_t area_start;
	size_t a;
	size_t bodylen;
	int retval;

	if ( num_bits    == 0              ) return FINS_RETVAL_SUCCESS;
	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( data        == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	XX_finslib_init_command( sys, & fins_cmnd, 0x23, 0x01 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = (num_bits >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (num_bits     ) & 0xff;

	for (a=0; a<num_bits; a++) {

		if ( data[a].force_command != FINS_FORCE_RESET           &&
		     data[a].force_command != FINS_FORCE_SET             &&
		     data[a].force_command != FINS_FORCE_RELEASE         &&
		     data[a].force_command != FINS_FORCE_RELEASE_TO_OFF  &&
		     data[a].force_command != FINS_FORCE_RELEASE_TO_ON       ) return FINS_RETVAL_INVALID_FORCE_COMMAND;

		fins_cmnd.body[bodylen++] = (data[a].force_command >> 8) & 0xff;
		fins_cmnd.body[bodylen++] = (data[a].force_command     ) & 0xff;

		if ( XX_finslib_decode_address( data[a].address, & address ) ) return FINS_RETVAL_INVALID_WRITE_ADDRESS;

		area_ptr = XX_finslib_search_area( sys, & address, 16, FI_FRC, false );
		if ( area_ptr == NULL ) return FINS_RETVAL_INVALID_WRITE_AREA;

		area_start  = address.main_address;
		area_start += area_ptr->low_addr >> 8;
		area_start -= area_ptr->low_id;

		fins_cmnd.body[bodylen++] =  area_ptr->area;
		fins_cmnd.body[bodylen++] = (area_start   >> 8) & 0xff;
		fins_cmnd.body[bodylen++] = (area_start       ) & 0xff;
		fins_cmnd.body[bodylen++] = address.sub_address & 0xff;
	}

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen, true ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen != 2 ) return FINS_RETVAL_BODY_TOO_SHORT;

	return FINS_RETVAL_SUCCESS;

}  /* finslib_force_bit */
