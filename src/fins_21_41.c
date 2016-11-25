/*
 * Library: libfins
 * File:    src/fins_21_41.c
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
 * The source file src/fins_21_41.c contains routines to clear the write access
 * log of a remote PLC over the FINS protocol.
 */

#include "fins.h"

/*
 * int finslib_write_access_log_clear( struct fins_sys_tp *sys );
 *
 * The function finslib_write_access_log_clear() clears the write access log of
 * a remote PLC over the FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_write_access_log_clear( struct fins_sys_tp *sys ) {

	struct fins_command_tp fins_cmnd;
	size_t bodylen;
	int retval;

	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	XX_finslib_init_command( sys, & fins_cmnd, 0x21, 0x41 );

	bodylen = 0;

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen != 2 ) return FINS_RETVAL_BODY_TOO_SHORT;

	return FINS_RETVAL_SUCCESS;

}  /* finslib_write_access_log_clear */
