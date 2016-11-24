/*
 * Library: libfins
 * File:    src/fins_01_01_bcd16.c
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
 * The source file src/fins_01_01_bcd16.c contains routines to read 16 bit BCD
 * data from a remote PLC over the FINS protocol with the function 01 01.
 */

#include "fins.h"

/*
 * int finslib_memory_area_read_bcd16( struct fins_sys_tp *sys, const char *start, uint16_t *data, size_t num_bcd16 );
 *
 * The function fins_memory_area_read_bcd16() reads an amount of BCD encoded
 * 16 bits words and puts them as converted binary values in a data array.
 * If an input value contains undefined bytes, the value UINT16_MAX is put in
 * the output array to indicate that the value was invalid.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_memory_area_read_bcd16( struct fins_sys_tp *sys, const char *start, uint16_t *data, size_t num_bcd16 ) {

	int32_t bin_val;
	uint16_t bcd_val;
	size_t chunk_start;
	size_t chunk_length;
	size_t offset;
	size_t a;
	size_t todo;
	size_t bodylen;
	struct fins_command_tp fins_cmnd;
	const struct fins_area_tp *area_ptr;
	struct fins_address_tp address;
	int retval;

	if ( num_bcd16   == 0                            ) return FINS_RETVAL_SUCCESS;
	if ( sys         == NULL                         ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( start       == NULL                         ) return FINS_RETVAL_NO_READ_ADDRESS;
	if ( data        == NULL                         ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET               ) return FINS_RETVAL_NOT_CONNECTED;
	if ( _finslib_decode_address( start, & address ) ) return FINS_RETVAL_INVALID_READ_ADDRESS;

	area_ptr = fins_search_area( sys, & address, 16, FI_RD, false );
	if ( area_ptr == NULL ) return FINS_RETVAL_INVALID_READ_AREA;

	offset       = 0;
	todo         = num_bcd16;
	chunk_start  = address.main_address;
	chunk_start += area_ptr->low_addr >> 8;
	chunk_start -= area_ptr->low_id;

	do {
		chunk_length = FINS_MAX_READ_WORDS_SYSWAY;
		if ( chunk_length > todo ) chunk_length = todo;

		fins_init_command( sys, & fins_cmnd, 0x01, 0x01 );

		bodylen = 0;

		fins_cmnd.body[bodylen++] = area_ptr->area;
		fins_cmnd.body[bodylen++] = (chunk_start  >> 8) & 0xff;
		fins_cmnd.body[bodylen++] = (chunk_start      ) & 0xff;
		fins_cmnd.body[bodylen++] = 0x00;
		fins_cmnd.body[bodylen++] = (chunk_length >> 8) & 0xff;
		fins_cmnd.body[bodylen++] = (chunk_length     ) & 0xff;

		if ( ( retval = _finslib_communicate( sys, & fins_cmnd, & bodylen ) ) != FINS_RETVAL_SUCCESS ) return retval;

		if ( bodylen != 2+2*chunk_length ) return FINS_RETVAL_BODY_TOO_SHORT;

		bodylen = 2;

		for (a=0; a<chunk_length; a++) {

			bcd_val   = fins_cmnd.body[bodylen++];
			bcd_val <<= 8;
			bcd_val  += fins_cmnd.body[bodylen++];

			bin_val   = finslib_bcd_to_int( bcd_val, FINS_DATA_TYPE_BCD16 );

			if ( bin_val == INT32_MAX ) data[offset+a] = UINT16_MAX;
			else                        data[offset+a] = (uint16_t) bin_val;
		}

		todo        -= chunk_length;
		offset      += chunk_length;
		chunk_start += chunk_length;

	} while ( todo > 0 );

	return FINS_RETVAL_SUCCESS;

}  /* finslib_memory_area_read_bcd16 */
