/*
 * Library: libfins
 * File:    src/fins_22_08.c
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
 * The source file src/fins_22_08.c contains routines to change the name of a
 * file in a remote PLC over the FINS protocol.
 */

#include <string.h>
#include "fins.h"

int finslib_file_rename( struct fins_sys_tp *sys, uint16_t disk, const char *path, const char *ofile, const char *nfile ) {

	struct fins_command_tp fins_cmnd;
	size_t a;
	size_t dirlen;
	size_t bodylen;
	char filename_83[13];
	int retval;

	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( ofile       == NULL           ) return FINS_RETVAL_INVALID_FILENAME;
	if ( nfile       == NULL           ) return FINS_RETVAL_INVALID_FILENAME;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	if ( disk != FINS_DISK_MEMORY_CARD  &&  disk != FINS_DISK_EM_FILE_MEMORY ) return FINS_RETVAL_INVALID_DISK;
	if ( ! finslib_valid_directory( path )                                   ) return FINS_RETVAL_INVALID_PATH;

	if ( path == NULL ) dirlen = 0;
	else                dirlen = strlen( path );

	XX_finslib_init_command( sys, & fins_cmnd, 0x22, 0x08 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = (disk >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (disk     ) & 0xff;

	if ( ( retval = finslib_filename_to_83( ofile, filename_83 ) ) != FINS_RETVAL_SUCCESS ) return retval;
	for (a=0; a<12; a++) fins_cmnd.body[bodylen++] = filename_83[a];

	if ( ( retval = finslib_filename_to_83( nfile, filename_83 ) ) != FINS_RETVAL_SUCCESS ) return retval;
	for (a=0; a<12; a++) fins_cmnd.body[bodylen++] = filename_83[a];

	fins_cmnd.body[bodylen++] = (dirlen >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (dirlen     ) & 0xff;

	for (a=0; a<dirlen; a++) fins_cmnd.body[bodylen++] = path[a];

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen != 2 ) return FINS_RETVAL_BODY_TOO_SHORT;

	return FINS_RETVAL_SUCCESS;

}  /* finslib_file_remove */
