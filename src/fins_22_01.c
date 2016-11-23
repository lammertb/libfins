/*
 * Library: libfins
 * File:    src/fins_22_01.c
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
 * The source file src/fins_22_01.c contains routines to read filenames from a
 * directory on a remote PLC over the FINS protocol.
 */

#include <string.h>
#include "fins.h"

/*
 * int finslib_file_name_read( struct fins_sys_tp *sys, struct fins_diskinfo_tp *diskinfo, struct fins_fileinfo_tp *fileinfo, uint16_t disk, const char *path, uint16_t start_file, size_t *num_file );
 *
 * The function finslib_file_name_read() can be used to read filenames and
 * volume information from a disk from a remote PLC over FINS. It is possible
 * to read only volume information by setting the fileinfo parameter to NULL
 * and the number of files to 0. It is also possible to only read file info by
 * setting the diskinfo parameter to NULL.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_file_name_read( struct fins_sys_tp *sys, struct fins_diskinfo_tp *diskinfo, struct fins_fileinfo_tp *fileinfo, uint16_t disk, const char *path, uint16_t start_file, size_t *num_files ) {

	struct fins_command_tp fins_cmnd;
	size_t bodylen;
	size_t a;
	size_t b;
	size_t dirlen;
	uint32_t datetime;
	int retval;

	if ( sys         == NULL                    ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( *num_files  >  0  &&  fileinfo == NULL ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( *num_files  == 0  &&  diskinfo == NULL ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET          ) return FINS_RETVAL_NOT_CONNECTED;

	if ( disk != FINS_DISK_MEMORY_CARD  &&  disk != FINS_DISK_EM_FILE_MEMORY ) return FINS_RETVAL_INVALID_DISK;

	if ( ! finslib_valid_directory( path ) ) return FINS_RETVAL_INVALID_PATH;

	if ( path == NULL ) dirlen = 0;
	else                dirlen = strlen( path );

	fins_init_command( sys, & fins_cmnd, 0x22, 0x01 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = (disk       >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (disk           ) & 0xff;
	fins_cmnd.body[bodylen++] = (start_file >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (start_file     ) & 0xff;
	fins_cmnd.body[bodylen++] = (*num_files >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (*num_files     ) & 0xff;
	fins_cmnd.body[bodylen++] = (dirlen     >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (dirlen         ) & 0xff;

	for (b=0; b<dirlen; b++) fins_cmnd.body[bodylen++] = path[b];

	if ( ( retval = _finslib_communicate( sys, & fins_cmnd, & bodylen ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen < 30 ) return FINS_RETVAL_BODY_TOO_SHORT;

	bodylen = 2;

	if ( diskinfo != NULL ) {

		for (b=0; b<12; b++) diskinfo->volume_label[b] = fins_cmnd.body[bodylen++];
		diskinfo->volume_label[12] = 0;

		datetime                   = fins_cmnd.body[bodylen++];
		datetime                 <<= 8;
		datetime                  += fins_cmnd.body[bodylen++];
		datetime                 <<= 8;
		datetime                  += fins_cmnd.body[bodylen++];
		datetime                 <<= 8;
		datetime                  += fins_cmnd.body[bodylen++];

		diskinfo->year             = ((datetime >> 25) & 0x7f) + 1980;
		diskinfo->month            =  (datetime >> 21) & 0x0f;
		diskinfo->day              =  (datetime >> 16) & 0x1f;
		diskinfo->hour             =  (datetime >> 11) & 0x1f;
		diskinfo->min              =  (datetime >>  5) & 0x3f;
		diskinfo->sec              = ((datetime      ) & 0x1f) * 2;

		diskinfo->total_capacity   = fins_cmnd.body[bodylen++];
		diskinfo->total_capacity <<= 8;
		diskinfo->total_capacity  += fins_cmnd.body[bodylen++];
		diskinfo->total_capacity <<= 8;
		diskinfo->total_capacity  += fins_cmnd.body[bodylen++];
		diskinfo->total_capacity <<= 8;
		diskinfo->total_capacity  += fins_cmnd.body[bodylen++];

		diskinfo->free_capacity    = fins_cmnd.body[bodylen++];
		diskinfo->free_capacity  <<= 8;
		diskinfo->free_capacity   += fins_cmnd.body[bodylen++];
		diskinfo->free_capacity  <<= 8;
		diskinfo->free_capacity   += fins_cmnd.body[bodylen++];
		diskinfo->free_capacity  <<= 8;
		diskinfo->free_capacity   += fins_cmnd.body[bodylen++];

		diskinfo->total_files      = fins_cmnd.body[bodylen++];
		diskinfo->total_files    <<= 8;
		diskinfo->total_files     += fins_cmnd.body[bodylen++];
	}
	else bodylen += 26;

	*num_files   = fins_cmnd.body[bodylen++] & 0x7f;
	*num_files <<= 8;
	*num_files  += fins_cmnd.body[bodylen++];

	for (a=0; a<*num_files; a++) {

		for (b=0; b<12; b++) fileinfo[a].filename[b] = fins_cmnd.body[bodylen++];
		fileinfo[a].filename[12] = 0;

		datetime               = fins_cmnd.body[bodylen++];
		datetime             <<= 8;
		datetime              += fins_cmnd.body[bodylen++];
		datetime             <<= 8;
		datetime              += fins_cmnd.body[bodylen++];
		datetime             <<= 8;
		datetime              += fins_cmnd.body[bodylen++];

		fileinfo[a].year       = ((datetime >> 25) & 0x7f) + 1980;
		fileinfo[a].month      =  (datetime >> 21) & 0x0f;
		fileinfo[a].day        =  (datetime >> 16) & 0x1f;
		fileinfo[a].hour       =  (datetime >> 11) & 0x1f;
		fileinfo[a].min        =  (datetime >>  5) & 0x3f;
		fileinfo[a].sec        = ((datetime      ) & 0x1f) * 2;

		fileinfo[a].size       = fins_cmnd.body[bodylen++];
		fileinfo[a].size     <<= 8;
		fileinfo[a].size      += fins_cmnd.body[bodylen++];
		fileinfo[a].size     <<= 8;
		fileinfo[a].size      += fins_cmnd.body[bodylen++];
		fileinfo[a].size     <<= 8;
		fileinfo[a].size      += fins_cmnd.body[bodylen++];

		bodylen++;

		fileinfo[a].read_only    = fins_cmnd.body[bodylen] & 0x01;
		fileinfo[a].hidden       = fins_cmnd.body[bodylen] & 0x02;
		fileinfo[a].system       = fins_cmnd.body[bodylen] & 0x04;
		fileinfo[a].volume_label = fins_cmnd.body[bodylen] & 0x08;
		fileinfo[a].directory    = fins_cmnd.body[bodylen] & 0x10;
		fileinfo[a].archive      = fins_cmnd.body[bodylen] & 0x20;

		bodylen++;
	}

	return FINS_RETVAL_SUCCESS;

}  /* finslib_file_name_read */
