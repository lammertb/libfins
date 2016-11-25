/*
 * Library: libfins
 * File:    src/fins_22_02.c
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
 * The source file src/fins_22_02.c contains routine to read information from a
 * file on a remote PLC with the FINS protocol.
 */

#include <string.h>
#include "fins.h"

/*
 * int finslib_file_read( struct fins_sys_tp *sys, uint16_t disk, const char *path, const char *filename, unsigned char *data, size_t file_position, size_t *num_bytes );
 *
 * The function finslib_file_read() reads a block of data from a single
 * file.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_file_read( struct fins_sys_tp *sys, uint16_t disk, const char *path, const char *filename, unsigned char *data, size_t file_position, size_t *num_bytes ) {

	struct fins_command_tp fins_cmnd;
	size_t a;
	size_t dirlen;
	size_t bodylen;
	char filename_83[13];
	int retval;

	if ( sys         == NULL                ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( num_bytes   == NULL                ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( *num_bytes  >  0  &&  data == NULL ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( *num_bytes  >  1900                ) return FINS_RETVAL_BODY_TOO_LONG;
	if ( sys->sockfd == INVALID_SOCKET      ) return FINS_RETVAL_NOT_CONNECTED;

	if ( disk != FINS_DISK_MEMORY_CARD  &&  disk != FINS_DISK_EM_FILE_MEMORY                 ) return FINS_RETVAL_INVALID_DISK;
	if ( ! finslib_valid_directory( path )                                                   ) return FINS_RETVAL_INVALID_PATH;
	if ( ( retval = finslib_filename_to_83( filename, filename_83 ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( path == NULL ) dirlen = 0;
	else                dirlen = strlen( path );

	XX_finslib_init_command( sys, & fins_cmnd, 0x22, 0x02 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = (disk >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (disk     ) & 0xff;

	for (a=0; a<12; a++) fins_cmnd.body[bodylen++] = filename_83[a];

	fins_cmnd.body[bodylen++] = (file_position >> 24) & 0xff;
	fins_cmnd.body[bodylen++] = (file_position >> 16) & 0xff;
	fins_cmnd.body[bodylen++] = (file_position >>  8) & 0xff;
	fins_cmnd.body[bodylen++] = (file_position      ) & 0xff;
	fins_cmnd.body[bodylen++] = (*num_bytes    >>  8) & 0xff;
	fins_cmnd.body[bodylen++] = (*num_bytes         ) & 0xff;
	fins_cmnd.body[bodylen++] = (dirlen        >>  8) & 0xff;
	fins_cmnd.body[bodylen++] = (dirlen             ) & 0xff;

	for (a=0; a<dirlen; a++) fins_cmnd.body[bodylen++] = path[a];

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen < 12 ) return FINS_RETVAL_BODY_TOO_SHORT;

	bodylen = 10;

	*num_bytes   = fins_cmnd.body[bodylen++];
	*num_bytes <<= 8;
	*num_bytes  += fins_cmnd.body[bodylen++];

	for (a=0; a<*num_bytes; a++) data[a] = fins_cmnd.body[bodylen++];

	return FINS_RETVAL_SUCCESS;

}  /* finslib_file_read */
