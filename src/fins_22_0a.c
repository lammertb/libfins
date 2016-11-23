/*
 * Library: libfins
 * File:    src/fins_22_0a.c
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
 * The source file src/fins_22_0a.c contains routines to transfer data from a
 * memory area to a file in a remote PLC over the FINS protocol.
 */

#include <string.h>
#include "fins.h"

static int transfer_func( struct fins_sys_tp *sys, const char *start, uint16_t disk, const char *path, const char *file, size_t *num_items, uint16_t mode );

/*
 * int finslib_area_to_file_transfer( struct fins_sys_tp *sys, const char *start, uint16_t disk, const char *path, const char *file, size_t *num_items );
 *
 * The function finslib_area_to_file_transfer() can be used to transfer items
 * from a memory area to a file on a remote PLC with the FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_area_to_file_transfer( struct fins_sys_tp *sys, const char *start, uint16_t disk, const char *path, const char *file, size_t *num_items ) {

	return transfer_func( sys, start, disk, path, file, num_items, 0x0000 );

}  /* finslib_area_to_file_transfer */

/*
 * int finslib_file_to_area_transfer( struct fins_sys_tp *sys, const char *start, uint16_t disk, const char *path, const char *file, size_t *num_lines );
 *
 * The function finslib_file_to_area_transfer() can be used to transfer items
 * from a file to a memory area on a remote PLC with the FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_file_to_area_transfer( struct fins_sys_tp *sys, const char *start, uint16_t disk, const char *path, const char *file, size_t *num_items ) {

	return transfer_func( sys, start, disk, path, file, num_items, 0x0001 );

}  /* finslib_file_to_area_transfer */

/*
 * int finslib_area_file_compare( struct fins_sys_tp *sys, const char *start, uint16_t disk, const char *path, const char *file, size_t *num_items );
 *
 * The function finslib_area_file_compare() can be used to compare the contents
 * of a memory area with a file on a remote PLC with the FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_area_file_compare( struct fins_sys_tp *sys, const char *start, uint16_t disk, const char *path, const char *file, size_t *num_items ) {

	return transfer_func( sys, start, disk, path, file, num_items, 0x0002 );

}  /* finslib_area_file_compare */

/*
 * static int transfer_func( struct fins_sys_tp *sys, const char *start, uint16_t disk, const char *path, const char *file, size_t *num_items, uint16_t mode );
 *
 * The function transfer_func() is the worker function which transfers or
 * compares data between memory areas and files on a remote PLC with FINS.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

static int transfer_func( struct fins_sys_tp *sys, const char *start, uint16_t disk, const char *path, const char *file, size_t *num_items, uint16_t mode ) {

	struct fins_command_tp fins_cmnd;
	struct fins_address_tp address;
	const struct fins_area_tp *area_ptr;
	int32_t area_start;
	size_t a;
	size_t dirlen;
	size_t bodylen;
	char filename_83[13];
	int retval;

	if ( num_items   == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( *num_items  == 0              ) return FINS_RETVAL_SUCCESS;
	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( file        == NULL           ) return FINS_RETVAL_INVALID_FILENAME;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	if ( mode == 0x0001 ) {

		if ( start == NULL ) return FINS_RETVAL_NO_WRITE_ADDRESS;
		if ( _finslib_decode_address( start, & address ) ) return FINS_RETVAL_INVALID_WRITE_ADDRESS;

		area_ptr = fins_search_area( sys, & address, 16, FI_TRD, false );
		if ( area_ptr == NULL ) return FINS_RETVAL_INVALID_WRITE_AREA;
	}

	else {

		if ( start == NULL ) return FINS_RETVAL_NO_READ_ADDRESS;
		if ( _finslib_decode_address( start, & address ) ) return FINS_RETVAL_INVALID_READ_ADDRESS;

		area_ptr = fins_search_area( sys, & address, 16, FI_TRS, false );
		if ( area_ptr == NULL ) return FINS_RETVAL_INVALID_READ_AREA;
	}

	if ( disk != FINS_DISK_MEMORY_CARD  &&  disk != FINS_DISK_EM_FILE_MEMORY             ) return FINS_RETVAL_INVALID_DISK;
	if ( ! finslib_valid_directory( path )                                               ) return FINS_RETVAL_INVALID_PATH;
	if ( ( retval = finslib_filename_to_83( file, filename_83 ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( path == NULL ) dirlen = 0;
	else                dirlen = strlen( path );

	area_start  = address.main_address;
	area_start += area_ptr->low_addr >> 8;
	area_start -= area_ptr->low_id;

	fins_init_command( sys, & fins_cmnd, 0x22, 0x0a );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = (mode       >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (mode           ) & 0xff;
	fins_cmnd.body[bodylen++] = area_ptr->area;
	fins_cmnd.body[bodylen++] = (area_start >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (area_start     ) & 0xff;
	fins_cmnd.body[bodylen++] = 0x00;
	fins_cmnd.body[bodylen++] = (*num_items >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (*num_items     ) & 0xff;
	fins_cmnd.body[bodylen++] = (disk       >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (disk           ) & 0xff;

	for (a=0; a<12; a++) fins_cmnd.body[bodylen++] = filename_83[a];

	fins_cmnd.body[bodylen++] = (dirlen >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (dirlen     ) & 0xff;

	for (a=0; a<dirlen; a++) fins_cmnd.body[bodylen++] = path[a];

	if ( ( retval = _finslib_communicate( sys, & fins_cmnd, & bodylen ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen != 4 ) return FINS_RETVAL_BODY_TOO_SHORT;

	bodylen = 2;

	*num_items   = fins_cmnd.body[bodylen++];
	*num_items <<= 8;
	*num_items  += fins_cmnd.body[bodylen++];

	return FINS_RETVAL_SUCCESS;

}  /* transfer_func */
