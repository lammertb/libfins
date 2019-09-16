/*
 * Library: libfins
 * File:    src/fins_decode.c
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
 * The source file src/fins_decode.c contains routines to decode a PLC memory
 * address in ASCII to a structure with the fields of those addresses separated
 * for use with devices over the Omron FINS protocol.
 */

#define FINS_SOCKET_INCLUDE	1

#include <ctype.h>
#include "fins.h"

/*
 * bool XX_finslib_decode_address( const char *str, fins_address_tp *address );
 *
 * The function XX_finslib_decode_address() takes a character string as
 * argument and decodes it to an areaname address pair. The function skips
 * spaces and other obvious nonfunctional characters. On success the value
 * false is returned and true is returned when problems arise at the
 * conversion.
 *
 * Bit references must use the DOT notation, for example H82.1 to generate
 * the proper address. Some applications use a notation without a dot like
 * H8201, but this is not supported by this function.
 */

bool XX_finslib_decode_address( const char *str, struct fins_address_tp *address ) {

	size_t num_char;
	uint32_t main_address;
	uint32_t sub_address;
	char name[4];
	const char *ptr;

	if ( str == NULL  ||  address == NULL ) return true;

	num_char     = 0;
	ptr          = str;
	main_address = 0;
	sub_address  = 0;

	while ( isspace( *ptr ) ) ptr++;

	while ( isalpha( *ptr )  &&  num_char < 3 ) {

		name[num_char] = (char) toupper( *ptr );
		num_char++;
		ptr++;
	}
	if ( isalpha( *ptr ) ) return true;
	
	while ( num_char < 4 ) name[num_char++] = 0;

	while ( isspace( *ptr ) ) ptr++;
	if ( ! isdigit( *ptr ) ) return true;

	while ( isdigit( *ptr ) ) {

		main_address *= 10;
		main_address += *ptr-'0';
		ptr++;
	}

	if ( *ptr == '.' ) {

		ptr++;
		while ( isdigit( *ptr ) ) {

			sub_address *= 10;
			sub_address += *ptr-'0';
			ptr++;
		}

		if ( sub_address > 15 ) return true;
	}

	while ( isdigit( *ptr ) ) ptr++;

	if ( *ptr ) return true;

	address->name[0]      = name[0];
	address->name[1]      = name[1];
	address->name[2]      = name[2];
	address->name[3]      = name[3];

	address->main_address = main_address;
	address->sub_address  = sub_address;

	return false;

}  /* XX_finslib_decode_address */
