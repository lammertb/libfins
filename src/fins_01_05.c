/*
 * Library: libfins
 * File:    src/fins_01_05.c
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
 * The source file src/fins_01_05.c contains routines to transfer data in a
 * remote PLC from one memory are to another through the FINS protocol.
 */

#include "fins.h"

/*
 * int finslib_memory_area_transfer( struct fins_sys_tp *sys, const char *source, const char *dest, size_t num_words );
 *
 * The function finslib_memory area_transfer() moves data from one area in a
 * remote PLC to another area through the FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_memory_area_transfer( struct fins_sys_tp *sys, const char *source, const char *dest, size_t num_words ) {

	int32_t source_start;
	int32_t dest_start;
	const struct fins_area_tp *source_area_ptr;
	const struct fins_area_tp *dest_area_ptr;
	struct fins_address_tp source_address;
	struct fins_address_tp dest_address;
	struct fins_command_tp fins_cmnd;
	size_t bodylen;
	int retval;

	if ( num_words   == 0                                      ) return FINS_RETVAL_SUCCESS;
	if ( sys         == NULL                                   ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( source      == NULL                                   ) return FINS_RETVAL_NO_READ_ADDRESS;
	if ( dest        == NULL                                   ) return FINS_RETVAL_NO_WRITE_ADDRESS;
	if ( sys->sockfd == INVALID_SOCKET                         ) return FINS_RETVAL_NOT_CONNECTED;
	if ( XX_finslib_decode_address( source, & source_address ) ) return FINS_RETVAL_INVALID_READ_ADDRESS;
	if ( XX_finslib_decode_address( dest,   & dest_address   ) ) return FINS_RETVAL_INVALID_WRITE_ADDRESS;

	source_area_ptr = XX_finslib_search_area( sys, & source_address, 16, FI_TRS, false );
	if ( source_area_ptr == NULL ) return FINS_RETVAL_INVALID_READ_AREA;

	dest_area_ptr = XX_finslib_search_area( sys, & dest_address, 16, FI_TRD, false );
	if ( dest_area_ptr == NULL ) return FINS_RETVAL_INVALID_WRITE_AREA;

	source_start  = source_address.main_address;
	source_start += source_area_ptr->low_addr >> 8;
	source_start -= source_area_ptr->low_id;

	dest_start    = dest_address.main_address;
	dest_start   += dest_area_ptr->low_addr >> 8;
	dest_start   -= dest_area_ptr->low_id;

	XX_finslib_init_command( sys, & fins_cmnd, 0x01, 0x05 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = source_area_ptr->area;
	fins_cmnd.body[bodylen++] = (source_start >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (source_start     ) & 0xff;
	fins_cmnd.body[bodylen++] = 0x00;
	fins_cmnd.body[bodylen++] = dest_area_ptr->area;
	fins_cmnd.body[bodylen++] = (dest_start   >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (dest_start       ) & 0xff;
	fins_cmnd.body[bodylen++] = 0x00;
	fins_cmnd.body[bodylen++] = (num_words    >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (num_words        ) & 0xff;

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen != 2 ) return FINS_RETVAL_BODY_TOO_SHORT;

	return FINS_RETVAL_SUCCESS;

}  /* finslib_memory_area_transfer */
