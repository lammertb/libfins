/*
 * Library: libfins
 * File:    src/fins_utils.c
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
 * The source file src/fins_utils.c contains handy routines used by routines in
 * the finslib library.
 */

#if defined(__FreeBSD__)
#include <unistd.h>
#include <sys/timespec.h>
#endif

#if defined(__linux__)
#include <unistd.h>
#endif

#include <string.h>
#include <time.h>
#include "fins.h"

static uint8_t bcdtoint_lut[] = {
/*	    .0   .1   .2   .3   .4   .5   .6   .7   .8   .9   .A   .B   .C   .D   .E   .F */
/* 0. */     0,   1,   2,   3,   4,   5,   6,   7,   8,   9, 255, 255, 255, 255, 255, 255,
/* 1. */    10,  11,  12,  13,  14,  15,  16,  17,  18,  19, 255, 255, 255, 255, 255, 255,
/* 2. */    20,  21,  22,  23,  24,  25,  26,  27,  28,  29, 255, 255, 255, 255, 255, 255,
/* 3. */    30,  31,  32,  33,  34,  35,  36,  37,  38,  39, 255, 255, 255, 255, 255, 255,
/* 4. */    40,  41,  42,  43,  44,  45,  46,  47,  48,  49, 255, 255, 255, 255, 255, 255,
/* 5. */    50,  51,  52,  53,  54,  55,  56,  57,  58,  59, 255, 255, 255, 255, 255, 255,
/* 6. */    60,  61,  62,  63,  64,  65,  66,  67,  68,  69, 255, 255, 255, 255, 255, 255,
/* 7. */    70,  71,  72,  73,  74,  75,  76,  77,  78,  79, 255, 255, 255, 255, 255, 255,
/* 8. */    80,  81,  82,  83,  84,  85,  86,  87,  88,  89, 255, 255, 255, 255, 255, 255,
/* 9. */    90,  91,  92,  93,  94,  95,  96,  97,  98,  99, 255, 255, 255, 255, 255, 255,
/* A. */   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
/* B. */   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
/* C. */   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
/* D. */   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
/* E. */   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
/* F. */   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

static uint8_t inttobcd_lut[] = {
	0x00,	0x01,	0x02,	0x03,	0x04,	0x05,	0x06,	0x07,	0x08,	0x09,
	0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17,	0x18,	0x19,
	0x20,	0x21,	0x22,	0x23,	0x24,	0x25,	0x26,	0x27,	0x28,	0x29,
	0x30,	0x31,	0x32,	0x33,	0x34,	0x35,	0x36,	0x37,	0x38,	0x39,
	0x40,	0x41,	0x42,	0x43,	0x44,	0x45,	0x46,	0x47,	0x48,	0x49,
	0x50,	0x51,	0x52,	0x53,	0x54,	0x55,	0x56,	0x57,	0x58,	0x59,
	0x60,	0x61,	0x62,	0x63,	0x64,	0x65,	0x66,	0x67,	0x68,	0x69,
	0x70,	0x71,	0x72,	0x73,	0x74,	0x75,	0x76,	0x77,	0x78,	0x79,
	0x80,	0x81,	0x82,	0x83,	0x84,	0x85,	0x86,	0x87,	0x88,	0x89,
	0x90,	0x91,	0x92,	0x93,	0x94,	0x95,	0x96,	0x97,	0x98,	0x99
};

static bool valid_dos_filename_char[] = {
/*	    .0     .1     .2     .3     .4     .5     .6     .7     .8     .9     .A     .B     .C     .D     .E     .F */
/* 0. */    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
/* 1. */    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
/* 2. */    true,  true,  false, true,  true,  true,  true,  true,  true,  true,  false, false, false, true,  false, false,
/* 3. */    true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  false, false, false, false, false, false,
/* 4. */    true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
/* 5. */    true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  false, false, false, true,  true,
/* 6. */    true,  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
/* 7. */    false, false, false, false, false, false, false, false, false, false, false, true,  false, true,  true,  false,
/* 8. */    true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
/* 9. */    true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
/* A. */    true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
/* B. */    true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
/* C. */    true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
/* D. */    true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
/* E. */    true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
/* F. */    true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true
};

/*
 * int32_t finslib_bcd_to_int( uint32_t value );
 *
 * The function finslib_bcd_to_int() converts a BCD encode value to a binary
 * integer. If undefined BCD characters are present in the input, the value -1
 * is returned instead of the converted value;
 */

int32_t finslib_bcd_to_int( uint32_t value ) {

	int32_t retval;
	int32_t mulval;
	int32_t addval;

	retval = 0;
	mulval = 1;

	while ( value != 0 ) {

		addval = bcdtoint_lut[ value & 0xff ];
		if ( addval >= 100 ) return -1;

		retval += mulval * addval;
		mulval *= 100;
		value >>= 8;
	}

	return retval;

}  /* finslib_bcd_to_int */

/*
 * uint32_t finslib_int_to_bcd( int32_t value );
 *
 * The function finslib_int_to_bcd() converts an binary coded integer to a BCD
 * value. If the function succeeds, the converted value is returned. Otherwise
 * the returned value is 0xFFFFFFFF to indicate that the value can not be
 * encoded in BCD.
 */

uint32_t finslib_int_to_bcd( int32_t value ) {

	int shiftval;
	uint32_t retval;
	uint32_t addval;

	if ( value < 0 ) return 0xFFFFFFFF;

	retval   = 0;
	shiftval = 0;

	while ( value != 0 ) {

		if ( shiftval > 24 ) return 0xFFFFFFFF;

		addval    = inttobcd_lut[ value % 100 ];
		retval   += addval << shiftval;
		shiftval += 8;
		value    /= 100;
	}

	return retval;

}  /* finslib_int_to_bcd */

/*
 * time_t finslib_monotonic_sec_timer( void );
 *
 * The function finslib monotonic_sec_timer() returns the value of a seconds
 * timer which is guaranteed to be monotonic, but has no connection with the
 * wall clock.
 */

time_t finslib_monotonic_sec_timer( void ) {

#if defined(_WIN32)
	return GetTickCount64() / 1000;
#else
	struct timespec ts;

	clock_gettime( CLOCK_MONOTONIC, & ts );
	return ts.tv_sec;
#endif

}  /* finslib_monotonic_sec_timer */

/*
 * void finslib_milli_second_sleep( int msec );
 *
 * The function finslib_milli_second_sleep() lets the thread sleep for a number
 * of milliseconds.
 */

void finslib_milli_second_sleep( int msec ) {

#ifdef _WIN32
	Sleep( msec );
#else
	usleep( 1000 * msec );
#endif

}  /* finslib_milli_second_sleep */

/*
 * The function finslib_valid_filename() checks if a provided filename is valid
 * to be used as a file in a remote PLC. If this is the case, true is returned.
 * Otherwise the return value is false.
 */

bool finslib_valid_filename( const char *filename ) {

	size_t len;
	size_t a;
	bool non_space;
	const unsigned char *unsigned_name;

	if ( filename == NULL ) return false;

	len           = strlen( filename );
	unsigned_name = (const unsigned char *) filename;

	if ( len              == 0                                ) return false;
	if ( unsigned_name[0] == 0xE5                             ) return false;
	if ( unsigned_name[0] == '~'  &&  unsigned_name[1] == '~' ) return false;

	non_space = false;

	for (a=0; a<8; a++) {

		if ( *unsigned_name == 0   ) break;
		if ( *unsigned_name == '.' ) break;

		if ( ! valid_dos_filename_char[ *unsigned_name ] ) return false;

		if ( *unsigned_name != ' ' ) non_space = true;

		unsigned_name++;
	}

	if ( a == 0                ) return false;
	if ( ! non_space           ) return false;
	if ( *unsigned_name == 0   ) return true;
	if ( *unsigned_name != '.' ) return false;

	unsigned_name++;

	non_space = false;

	for (a=0; a<3; a++) {

		if ( *unsigned_name == 0   ) break;
		if ( *unsigned_name == '.' ) return false;

		if ( ! valid_dos_filename_char[ *unsigned_name ] ) return false;

		if ( *unsigned_name != ' ' ) non_space = true;

		unsigned_name++;
	}

	if ( ! non_space         ) return false;
	if ( *unsigned_name != 0 ) return false;

	return true;

}  /* finslib_valid_filename */

/*
 * bool finslib_valid_directory( const char *path );
 *
 * The function finslib_valid_directory() checks if a directory path is a valid
 * DOS path to be used in a remote PLC as a file path. Both a NULL path and an
 * empty path are considered valid descriptions of the root directory. Longer
 * paths must begin with a backslash \ and be followed by a valid filename to
 * describe the subdirectory. For deeper directories the same rule applies. The
 * maximum length of a path is 65 characters and the path may not be terminated
 * with a backslash.
 */

bool finslib_valid_directory( const char *path ) {

	size_t a;
	size_t len;
	const char *ptr;
	char subdir[13];

	if ( path == NULL ) return true;

	len = strlen( path );
	if ( len == 0  ) return true;
	if ( len >  65 ) return false;

	ptr = path;

	while ( *ptr ) {

		if ( *ptr != '\\' ) return false;
		ptr++;

		for (a=0; a<12; a++) {

			if ( *ptr == 0  ||  *ptr == '\\' ) break;
			subdir[a] = *ptr++;
		}

		subdir[a] = 0;

		if ( a    == 0                          ) return false;
		if ( *ptr != 0  &&  *ptr != '\\'        ) return false;
		if ( ! finslib_valid_filename( subdir ) ) return false;
	}

	return true;

}  /* finslib_valid_directory */

/*
 * int finslib_filename_to_83( const char *infile, char *outfile ):
 *
 * The function finslib_filename_to_83() converts a filename to an expanded 8.3
 * filename format with spaces padded where necessary. At success the function
 * will return FINS_RETVAL_SUCCESS. Otherwise an error value is returned.
 */

int finslib_filename_to_83( const char *infile, char *outfile ) {

	const char *ptr;
	char *dest;
	size_t a;

	if ( infile  == NULL                    ) return FINS_RETVAL_INVALID_FILENAME;
	if ( outfile == NULL                    ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( ! finslib_valid_filename( infile ) ) return FINS_RETVAL_INVALID_FILENAME;

	ptr  = infile;
	dest = outfile;

	for (a=0; a<8; a++) {

		if ( *ptr  &&  *ptr != '.' ) *dest++ = *ptr++;
		else *dest++ = ' ';
	}

	if ( *ptr == '.' ) *dest++ = *ptr++;
	else               *dest++ = ' ';

	for (a=0; a<3; a++) {

		if ( *ptr ) *dest++ = *ptr++;
		else        *dest++ = ' ';
	}

	*dest = 0;

	return FINS_RETVAL_SUCCESS;

}  /* finslib_filename_to_83 */
