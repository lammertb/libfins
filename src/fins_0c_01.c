/*
 * Library: libfins
 * File:    src/fins_0c_01.c
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
 * The source file src/fins_0c_01.c contains routines to acquire access rights
 * to a remote PLC over the FINS protocol.
 */

#include "fins.h"

/*
 * int finslib_access_right_acquire( struct fins_sys_tp *sys, struct fins_nodedata_tp *nodedata );
 *
 * The function finslib_access_right_acquire() tries to acquire access rights
 * to a remote PLC over the FINS protocol. If this succeeds, the return value
 * FINS_RETVAL_SUCCESS is returned. If another network node holds the access
 * rights, the value FINS_RETVAL_ACCESS_NO_RIGHTS is returned and the detail
 * information of that other node is put in a structure. Other errors are
 * reported with their respective FINS_RETVAL_... values.
 */

int finslib_access_right_acquire( struct fins_sys_tp *sys, struct fins_nodedata_tp *nodedata ) {

	struct fins_command_tp fins_cmnd;
	size_t bodylen;
	int retval;

	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( nodedata    == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	XX_finslib_init_command( sys, & fins_cmnd, 0x0c, 0x01 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = 0xff;
	fins_cmnd.body[bodylen++] = 0xff;

	retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen, true );

	if ( retval == FINS_RETVAL_ACCESS_NO_RIGHTS ) {

		bodylen = 2;

		nodedata->network = fins_cmnd.body[bodylen++];
		nodedata->node    = fins_cmnd.body[bodylen++];
		nodedata->unit    = fins_cmnd.body[bodylen++];
	}

	return retval;

}  /* finslib_access_right acquire */
