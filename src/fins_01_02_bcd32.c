/*
 * Library: libfins
 * File:    src/fins_01_02_bcd32.c
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
 * The source file src/fins_01_02_bcd32.c contains routines to write blocks of
 * 32 bit BCD words to memory areas of a remote PLC.
 */

#include "fins.h"

static int	process_data( struct fins_sys_tp *sys, const char *start, const uint32_t *data, size_t num_bcd32, int type );

/*
 * int finslib_memory_area_write_sbcd32( struct fins_sys_tp *sys, const char *start, const int32_t *data, size_t num_sbcd32, int type );
 *
 * The function finslib_memory_area_write_sbcd32() writes a block of 32 bit
 * signed BCD values to a memory area in a remote PLC through the FIN protocol.
 * The data is provided as an array with binary values which are converted to
 * BCD before being sent to the PLC. If a value cannot be converted to a valid
 * BCD value, INT32_MAX is used instead. As there are several ways to encode
 * signed BCDs, the encoding type is provided as a parameter.
 */

int finslib_memory_area_write_sbcd32( struct fins_sys_tp *sys, const char *start, const int32_t *data, size_t num_sbcd32, int type ) {

	return process_data( sys, start, (uint32_t *)data, num_sbcd32, type );

}  /* finslib_memory_area_write_sbcd32 */

/*
 * int finslib_memory_area_write_bcd32( struct fins_sys_tp *sys, const char *start, const uint32_t *data, size_t num_bcd32 );
 *
 * The function finslib_memory_area_write_bcd32() writes a block of 32 bit BCD
 * values to a memory area in a remote PLC through the FINS protocol. The data
 * is delivered as an array with binary values which are converted to BCD
 * before being sent to the PLC. If a value cannot be converted to a valid BCD
 * value, INT32_MAX is used instead.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_memory_area_write_bcd32( struct fins_sys_tp *sys, const char *start, const uint32_t *data, size_t num_bcd32 ) {

	return process_data( sys, start, data, num_bcd32, FINS_DATA_TYPE_BCD32 );

}  /* finslib_memory_area_write_bcd32 */

/*
 * static int process_data( struct fins_sys_tp *sys, const char *start, const uint32_t *data, size_t num_bcd32, int type );
 *
 * The function process_data() is the workhorse to write 32 bit BCD data in a
 * memory area of a remote PLC. The function processes with proper casting  of
 * the parameters both signed and unsigned 32 bit BCD values.
 */

static int process_data( struct fins_sys_tp *sys, const char *start, const uint32_t *data, size_t num_bcd32, int type ) {

	uint32_t bcd_val;
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

	if ( num_bcd32   == 0                            ) return FINS_RETVAL_SUCCESS;
	if ( sys         == NULL                         ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( start       == NULL                         ) return FINS_RETVAL_NO_WRITE_ADDRESS;
	if ( data        == NULL                         ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET               ) return FINS_RETVAL_NOT_CONNECTED;
	if ( _finslib_decode_address( start, & address ) ) return FINS_RETVAL_INVALID_WRITE_ADDRESS;

	area_ptr = fins_search_area( sys, & address, 16, FI_WR, false );
	if ( area_ptr == NULL ) return FINS_RETVAL_INVALID_WRITE_AREA;

	offset       = 0;
	todo         = num_bcd32;
	chunk_start  = address.main_address;
	chunk_start += area_ptr->low_addr >> 8;
	chunk_start -= area_ptr->low_id;

	do {
		chunk_length = FINS_MAX_WRITE_WORDS_SYSWAY;
		if ( chunk_length > 2*todo ) chunk_length = 2*todo;

		chunk_length &= 0xFFFFFFFE;

		fins_init_command( sys, & fins_cmnd, 0x01, 0x02 );

		bodylen = 0;

		fins_cmnd.body[bodylen++] = area_ptr->area;
		fins_cmnd.body[bodylen++] = (chunk_start  >> 8) & 0xff;
		fins_cmnd.body[bodylen++] = (chunk_start      ) & 0xff;
		fins_cmnd.body[bodylen++] = 0x00;
		fins_cmnd.body[bodylen++] = (chunk_length >> 8) & 0xff;
		fins_cmnd.body[bodylen++] = (chunk_length     ) & 0xff;

		for (a=0; a<chunk_length/2; a++) {

			bcd_val = finslib_int_to_bcd( data[offset+a], type );

			fins_cmnd.body[bodylen+2] = (bcd_val >> 24) & 0xff;
			fins_cmnd.body[bodylen+3] = (bcd_val >> 16) & 0xff;
			fins_cmnd.body[bodylen+0] = (bcd_val >>  8) & 0xff;
			fins_cmnd.body[bodylen+1] = (bcd_val      ) & 0xff;

			bodylen += 4;
		}

		if ( ( retval = _finslib_communicate( sys, & fins_cmnd, & bodylen ) ) != FINS_RETVAL_SUCCESS ) return retval;

		if ( bodylen != 2 ) return FINS_RETVAL_BODY_TOO_SHORT;

		todo        -= chunk_length / 2;
		offset      += chunk_length / 2;
		chunk_start += chunk_length;

	} while ( todo > 0 );

	return FINS_RETVAL_SUCCESS;

}  /* finslib_memory_area_write_bcd32 */
