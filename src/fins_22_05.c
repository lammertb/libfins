/*
 * Library: libfins
 * File:    src/fins_22_05.c
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
 * The source file src/fins_22_05.c contains routines to delete files from a
 * remote PLC over the FINS protocol.
 */

#define FINS_SOCKET_INCLUDE	1

#include <string.h>
#include "fins.h"

int finslib_file_delete( struct fins_sys_tp *sys, uint16_t disk, const char *path, const char **filename, size_t *num_files ) {

	struct fins_command_tp fins_cmnd;
	size_t a;
	size_t b;
	size_t dirlen;
	size_t bodylen;
	char filename_83[13];
	int retval;

	if ( num_files   == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( *num_files  == 0              ) return FINS_RETVAL_SUCCESS;
	if ( *num_files  >  100            ) return FINS_RETVAL_BODY_TOO_LONG;
	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( filename    == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	if ( disk != FINS_DISK_MEMORY_CARD  &&  disk != FINS_DISK_EM_FILE_MEMORY ) return FINS_RETVAL_INVALID_DISK;
	if ( ! finslib_valid_directory( path )                                   ) return FINS_RETVAL_INVALID_PATH;

	if ( path == NULL ) dirlen = 0;
	else                dirlen = strlen( path );

	XX_finslib_init_command( sys, & fins_cmnd, 0x22, 0x05 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = (disk       >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (disk           ) & 0xff;
	fins_cmnd.body[bodylen++] = (*num_files >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (*num_files     ) & 0xff;

	for (a=0; a<*num_files; a++) {

		if ( ( retval = finslib_filename_to_83( filename[a], filename_83 ) ) != FINS_RETVAL_SUCCESS ) return retval;
		for (b=0; b<12; b++) fins_cmnd.body[bodylen++] = filename[a][b];
	}

	fins_cmnd.body[bodylen++] = (dirlen >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (dirlen     ) & 0xff;

	for (b=0; b<dirlen; b++) fins_cmnd.body[bodylen++] = path[b];

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen, true ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen != 4 ) return FINS_RETVAL_BODY_TOO_SHORT;

	bodylen = 2;

	*num_files   = fins_cmnd.body[bodylen++];
	*num_files <<= 8;
	*num_files  += fins_cmnd.body[bodylen++];

	return FINS_RETVAL_SUCCESS;

}  /* finslib_file_delete */
