/*
 * Library: libfins
 * File:    src/fins_21_01.c
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
 * The source file  src/fins_21_01.c contains routines to clear errors or error
 * messages in a remote PLC over the FINS protocol.
 */

#define FINS_SOCKET_INCLUDE	1

#include "fins.h"

/*
 * int finslib_error_clear_all( struct fins_sys_tp *sys );
 *
 * The function finslib_error_clear_all() clears all errors from a remote PLC
 * over the FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_error_clear_all( struct fins_sys_tp *sys ) {

	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	return finslib_error_clear( sys, 0xFFFF );

}  /* finslib_error_clear_all */

/*
 * int finslib_error_clear_current( struct fins_sys_tp *sys );
 *
 * The function finslib_error_clear_current() clears the current error with the
 * highest priority in a remote PLC over the FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_error_clear_current( struct fins_sys_tp *sys ) {

	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	return finslib_error_clear( sys, 0xFFFE );

}  /* finslib_error_clear_current */

/*
 * int finslib_error_clear_fal( struct fins_sys_tp *sys, uint16_t fal_number );
 *
 * The function finslib_error_clear_fal() clears a non-fatal FAL error in a
 * remote PLC over the FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_error_clear_fal( struct fins_sys_tp *sys, uint16_t fal_number ) {

	if ( fal_number  <  1              ) return FINS_RETVAL_NO_WRITE_ADDRESS;
	if ( fal_number  >  511            ) return FINS_RETVAL_NO_WRITE_ADDRESS;
	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	return finslib_error_clear( sys, 0x4100 + fal_number );

}  /* finslib_error_clear_fal */

/*
 * int finslib_error_clear_fals( struct fins_sys_tp *sys, uint16_t fals_number );
 *
 * The function finslib_error_clear_fals() clears a fatal FALS message in a
 * remote PLC over the FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_error_clear_fals( struct fins_sys_tp *sys, uint16_t fals_number ) {

	if ( fals_number <  1              ) return FINS_RETVAL_NO_WRITE_ADDRESS;
	if ( fals_number >  511            ) return FINS_RETVAL_NO_WRITE_ADDRESS;
	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	return finslib_error_clear( sys, 0xC100 + fals_number );

}  /* finslib_error_clear_fals */

/*
 * int finslib_error_clear( struct fins_sys_tp *sys, uint16_t error_code );
 *
 * The function finslib_error_clear( clears an error message from a
 * remote PLC over the FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_error_clear( struct fins_sys_tp *sys, uint16_t error_code ) {

	struct fins_command_tp fins_cmnd;
	size_t bodylen;
	int retval;

	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	XX_finslib_init_command( sys, & fins_cmnd, 0x21, 0x01 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = (error_code >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (error_code     ) & 0xff;

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen, true ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen != 2 ) return FINS_RETVAL_BODY_TOO_SHORT;

	return FINS_RETVAL_SUCCESS;

}  /* finslib_error_clear */
