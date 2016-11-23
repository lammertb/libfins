/*
 * Library: libfins
 * File:    src/fins_01_02.c
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
 * The source file src/fins_01_02.c contains routines to write blocks of data to
 * memory areas of a remote PLC.
 */

#include "fins.h"

/*
 * int finslib_memory_area_write_word( struct fins_sys_tp *sys, const char *start, const unsigned char *data, size_t num_words );
 *
 * The function finslib_memory_area_write_word() writes a block of words to a
 * memory area of a remote PLC over the FINS protocol. For very large blocks
 * the transfer is partitioned in multiple chunks.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_memory_area_write_word( struct fins_sys_tp *sys, const char *start, const unsigned char *data, size_t num_words ) {

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

	if ( num_words   == 0                            ) return FINS_RETVAL_SUCCESS;
	if ( sys         == NULL                         ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( start       == NULL                         ) return FINS_RETVAL_NO_WRITE_ADDRESS;
	if ( data        == NULL                         ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET               ) return FINS_RETVAL_NOT_CONNECTED;
	if ( _finslib_decode_address( start, & address ) ) return FINS_RETVAL_INVALID_WRITE_ADDRESS;

	area_ptr = fins_search_area( sys, & address, 16, FI_WR, false );
	if ( area_ptr == NULL ) return FINS_RETVAL_INVALID_WRITE_AREA;

	offset       = 0;
	todo         = num_words;
	chunk_start  = address.main_address;
	chunk_start += area_ptr->low_addr >> 8;
	chunk_start -= area_ptr->low_id;

	do {
		chunk_length = FINS_MAX_WRITE_WORDS_SYSWAY;
		if ( chunk_length > todo ) chunk_length = todo;

		fins_init_command( sys, & fins_cmnd, 0x01, 0x02 );

		bodylen = 0;

		fins_cmnd.body[bodylen++] = area_ptr->area;
		fins_cmnd.body[bodylen++] = (chunk_start  >> 8) & 0xff;
		fins_cmnd.body[bodylen++] = (chunk_start      ) & 0xff;
		fins_cmnd.body[bodylen++] = 0x00;
		fins_cmnd.body[bodylen++] = (chunk_length >> 8) & 0xff;
		fins_cmnd.body[bodylen++] = (chunk_length     ) & 0xff;

		for (a=0; a<2*chunk_length; a++) fins_cmnd.body[bodylen++] = data[offset+a];

		if ( ( retval = _finslib_communicate( sys, & fins_cmnd, & bodylen ) ) != FINS_RETVAL_SUCCESS ) return retval;

		if ( bodylen != 2 ) return FINS_RETVAL_BODY_TOO_SHORT;

		todo        -= chunk_length;
		offset      += chunk_length * 2;
		chunk_start += chunk_length;

	} while ( todo > 0 );

	return FINS_RETVAL_SUCCESS;

}  /* finslib_memory_area_write_word */
