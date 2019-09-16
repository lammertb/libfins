/*
 * Library: libfins
 * File:    src/fins_22_07.c
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
 * The source file src/fins_22_07.c contains routines to copy files on a remote
 * PLC over the FINS protocol.
 */

#define FINS_SOCKET_INCLUDE	1

#include <string.h>
#include "fins.h"

/*
 * int finslib_file_copy( struct fins_sys_tp *sys, uint16_t sdisk, const char *spath, const char *sfile, uint16_t ddisk, const char *dpath, const char *dfile );
 *
 * The function finslib_file_copy() copies a file from one location to another
 * on a remote PLC over the FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_file_copy( struct fins_sys_tp *sys, uint16_t sdisk, const char *spath, const char *sfile, uint16_t ddisk, const char *dpath, const char *dfile ) {

	struct fins_command_tp fins_cmnd;
	size_t a;
	size_t sdirlen;
	size_t ddirlen;
	size_t bodylen;
	char filename_83[13];
	int retval;

	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( sfile       == NULL           ) return FINS_RETVAL_INVALID_FILENAME;
	if ( dfile       == NULL           ) return FINS_RETVAL_INVALID_FILENAME;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	if ( sdisk != FINS_DISK_MEMORY_CARD  &&  sdisk != FINS_DISK_EM_FILE_MEMORY ) return FINS_RETVAL_INVALID_DISK;
	if ( ddisk != FINS_DISK_MEMORY_CARD  &&  ddisk != FINS_DISK_EM_FILE_MEMORY ) return FINS_RETVAL_INVALID_DISK;
	if ( ! finslib_valid_directory( spath )                                    ) return FINS_RETVAL_INVALID_PATH;
	if ( ! finslib_valid_directory( dpath )                                    ) return FINS_RETVAL_INVALID_PATH;

	if ( spath == NULL ) sdirlen = 0;
	else                 sdirlen = strlen( spath );

	if ( dpath == NULL ) ddirlen = 0;
	else                 ddirlen = strlen( dpath );

	XX_finslib_init_command( sys, & fins_cmnd, 0x22, 0x07 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = (sdisk >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (sdisk     ) & 0xff;

	if ( ( retval = finslib_filename_to_83( sfile, filename_83 ) ) != FINS_RETVAL_SUCCESS ) return retval;
	for (a=0; a<12; a++) fins_cmnd.body[bodylen++] = filename_83[a];

	fins_cmnd.body[bodylen++] = (ddisk >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (ddisk     ) & 0xff;

	if ( ( retval = finslib_filename_to_83( dfile, filename_83 ) ) != FINS_RETVAL_SUCCESS ) return retval;
	for (a=0; a<12; a++) fins_cmnd.body[bodylen++] = filename_83[a];

	fins_cmnd.body[bodylen++] = (sdirlen >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (sdirlen     ) & 0xff;

	for (a=0; a<sdirlen; a++) fins_cmnd.body[bodylen++] = spath[a];

	fins_cmnd.body[bodylen++] = (ddirlen >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (ddirlen     ) & 0xff;

	for (a=0; a<ddirlen; a++) fins_cmnd.body[bodylen++] = dpath[a];

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen, true ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen != 2 ) return FINS_RETVAL_BODY_TOO_SHORT;

	return FINS_RETVAL_SUCCESS;

}  /* finslib_file_copy */
