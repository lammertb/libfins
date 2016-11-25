/*
 * Library: libfins
 * File:    src/fins_01_03.c
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
 * The source file src/fins_01_03.c contains routines to fill a memory area of
 * a remote PLC with data through the FINS protocol.
 */

#include "fins.h"

/*
 * int finslib_memory_area_fill( struct fins_sys_tp *sys, const char *start, uint16_t fill_data, size_t num_words );
 *
 * The function finslib_memory_area_fill() fills a memory area of a remote PLC
 * with a fixed word through the FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_memory_area_fill( struct fins_sys_tp *sys, const char *start, uint16_t fill_data, size_t num_words ) {

	uint32_t block_start;
	const struct fins_area_tp *area_ptr;
	struct fins_address_tp address;
	struct fins_command_tp fins_cmnd;
	size_t bodylen;
	int retval;

	if ( num_words   == 0                              ) return FINS_RETVAL_SUCCESS;
	if ( sys         == NULL                           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( start       == NULL                           ) return FINS_RETVAL_NO_WRITE_ADDRESS;
	if ( sys->sockfd == INVALID_SOCKET                 ) return FINS_RETVAL_NOT_CONNECTED;
	if ( XX_finslib_decode_address( start, & address ) ) return FINS_RETVAL_INVALID_WRITE_ADDRESS;

	area_ptr = XX_finslib_search_area( sys, & address, 16, FI_FILL, false );
	if ( area_ptr == NULL ) return FINS_RETVAL_INVALID_FILL_AREA;

	block_start  = address.main_address;
	block_start += area_ptr->low_addr >> 8;
	block_start -= area_ptr->low_id;

	XX_finslib_init_command( sys, & fins_cmnd, 0x01, 0x03 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = area_ptr->area;
	fins_cmnd.body[bodylen++] = (block_start >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (block_start     ) & 0xff;
	fins_cmnd.body[bodylen++] = 0x00;
	fins_cmnd.body[bodylen++] = (num_words   >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (num_words       ) & 0xff;
	fins_cmnd.body[bodylen++] = (fill_data   >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (fill_data       ) & 0xff;

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen != 2 ) return FINS_RETVAL_BODY_TOO_SHORT;

	return FINS_RETVAL_SUCCESS;

}  /* finslib_memory_area_fill */
