/*
 * Library: libfins
 * File:    src/fins_01_02_int32.c
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
 * The source file src/fins_01_02_int32.c contains routines to write blocks of
 * 32 bit signed and unsigned integers to memory areas of a remote PLC.
 */

#define FINS_SOCKET_INCLUDE	1

#include "fins.h"

/*
 * int finslib_memory_area_write_int32( struct fins_sys_tp *sys, const char *start, const int32_t *data, size_t num_int32 );
 *
 * The function finslib_memory_area_write_uint32() writes a block of 32 bit
 * signed integer values to a memory area in a remote PLC through the FINS
 * protocol. As signed an unsigned integers have the same structure, the
 * function finslib_memory_area_write_uint32() is used for the dirty work.
 */

int finslib_memory_area_write_int32( struct fins_sys_tp *sys, const char *start, const int32_t *data, size_t num_int32 ) {

	return finslib_memory_area_write_uint32( sys, start, (const uint32_t *) data, num_int32 );

}  /* finslib_memory_area_write_int32 */

/*
 * int finslib_memory_area_write_uint32( struct fins_sys_tp *sys, const char *start, const uint32_t *data, size_t num_uint32 );
 *
 * The function finslib_memory_area_write_uint32() writes a block of 32 bit
 * unsigned integer values to a memory area in a remote PLC through the FINS
 * protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_memory_area_write_uint32( struct fins_sys_tp *sys, const char *start, const uint32_t *data, size_t num_uint32 ) {

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

	if ( num_uint32  == 0                              ) return FINS_RETVAL_SUCCESS;
	if ( sys         == NULL                           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( start       == NULL                           ) return FINS_RETVAL_NO_WRITE_ADDRESS;
	if ( data        == NULL                           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET                 ) return FINS_RETVAL_NOT_CONNECTED;
	if ( XX_finslib_decode_address( start, & address ) ) return FINS_RETVAL_INVALID_WRITE_ADDRESS;

	area_ptr = XX_finslib_search_area( sys, & address, 16, FI_WR, false );
	if ( area_ptr == NULL ) return FINS_RETVAL_INVALID_WRITE_AREA;

	offset       = 0;
	todo         = num_uint32;
	chunk_start  = address.main_address;
	chunk_start += area_ptr->low_addr >> 8;
	chunk_start -= area_ptr->low_id;

	do {
		chunk_length = FINS_MAX_WRITE_WORDS_SYSWAY;
		if ( chunk_length > 2*todo ) chunk_length = 2*todo;

		chunk_length &= 0xFFFFFFFE;

		XX_finslib_init_command( sys, & fins_cmnd, 0x01, 0x02 );

		bodylen = 0;

		fins_cmnd.body[bodylen++] = area_ptr->area;
		fins_cmnd.body[bodylen++] = (chunk_start  >> 8) & 0xff;
		fins_cmnd.body[bodylen++] = (chunk_start      ) & 0xff;
		fins_cmnd.body[bodylen++] = 0x00;
		fins_cmnd.body[bodylen++] = (chunk_length >> 8) & 0xff;
		fins_cmnd.body[bodylen++] = (chunk_length     ) & 0xff;

		for (a=0; a<chunk_length/2; a++) {

			fins_cmnd.body[bodylen+2] = (data[offset+a] >> 24) & 0xff;
			fins_cmnd.body[bodylen+3] = (data[offset+a] >> 16) & 0xff;
			fins_cmnd.body[bodylen+0] = (data[offset+a] >>  8) & 0xff;
			fins_cmnd.body[bodylen+1] = (data[offset+a]      ) & 0xff;

			bodylen += 4;
		}

		if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen, true ) ) != FINS_RETVAL_SUCCESS ) return retval;

		if ( bodylen != 2 ) return FINS_RETVAL_BODY_TOO_SHORT;

		todo        -= chunk_length / 2;
		offset      += chunk_length / 2;
		chunk_start += chunk_length;

	} while ( todo > 0 );

	return FINS_RETVAL_SUCCESS;

}  /* finslib_memory_area_write_uint32 */
