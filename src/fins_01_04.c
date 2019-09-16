/*
 * Library: libfins
 * File:    src/fins_01_04.c
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
 * The source file src/fins_01_04.c contains routines to read multiple data
 * types in one batch from a remote PLC over the FINS protocol.
 */

#define FINS_SOCKET_INCLUDE	1

#include "fins.h"

/*
 * int finslib_multiple_memory_area_read( struct fins_sys_tp *sys, struct fins_multidata_tp *item, size_t num_item );
 *
 * The function finslib_multiple_memory_area_read() can be used to read data
 * from different areas from a remote PLC with one call over the FINS protocol.
 * If more data is requested than can be handled, the request is split over
 * multiple sub requests.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_multiple_memory_area_read( struct fins_sys_tp *sys, struct fins_multidata_tp *item, size_t num_item ) {

	uint32_t bcd_val;
	size_t a;
	size_t bodylen;
	size_t recvlen;
	size_t chunk_start;
	size_t chunk_length;
	size_t offset;
	size_t todo;
	struct fins_command_tp fins_cmnd;
	struct fins_address_tp address;
	const struct fins_area_tp *area_ptr;
	union {
		unsigned char val_raw[4];
		float val_float;
	} sfloat;
	union {
		unsigned char val_raw[8];
		double val_double;
	} dfloat;
	int retval;

	if ( num_item    == 0              ) return FINS_RETVAL_SUCCESS;
	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( item        == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	offset = 0;
	todo   = num_item;

	do {
		chunk_length = 24;
		if ( chunk_length > todo ) chunk_length = todo;

		XX_finslib_init_command( sys, & fins_cmnd, 0x01, 0x04 );

		bodylen = 0;
		recvlen = 2;

		for (a=0; a<chunk_length; a++) {

			switch( item[offset+a].type ) {

				case FINS_DATA_TYPE_INT16    :
				case FINS_DATA_TYPE_UINT16   :
				case FINS_DATA_TYPE_BCD16    :
				case FINS_DATA_TYPE_SBCD16_0 :
				case FINS_DATA_TYPE_SBCD16_1 :
				case FINS_DATA_TYPE_SBCD16_2 :
				case FINS_DATA_TYPE_SBCD16_3 :

					if ( XX_finslib_decode_address( item[offset+a].address, & address ) ) return FINS_RETVAL_INVALID_READ_ADDRESS;

					area_ptr = XX_finslib_search_area( sys, & address, 16, FI_MRD, false );
					if ( area_ptr == NULL ) return FINS_RETVAL_INVALID_READ_AREA;

					chunk_start  = address.main_address;
					chunk_start += area_ptr->low_addr >> 8;
					chunk_start -= area_ptr->low_id;

					fins_cmnd.body[bodylen++] = area_ptr->area;
					fins_cmnd.body[bodylen++] = (chunk_start >> 8) & 0xff;
					fins_cmnd.body[bodylen++] = (chunk_start     ) & 0xff;
					fins_cmnd.body[bodylen++] = 0x00;

					recvlen += 3;

					break;



				case FINS_DATA_TYPE_BIT :

					if ( XX_finslib_decode_address( item[offset+a].address, & address ) ) return FINS_RETVAL_INVALID_READ_ADDRESS;

					area_ptr = XX_finslib_search_area( sys, & address, 1, FI_MRD, false );
					if ( area_ptr == NULL ) return FINS_RETVAL_INVALID_READ_AREA;

					chunk_start  = address.main_address;
					chunk_start += area_ptr->low_addr >> 8;
					chunk_start -= area_ptr->low_id;

					fins_cmnd.body[bodylen++] = area_ptr->area;
					fins_cmnd.body[bodylen++] = (chunk_start >> 8)  & 0xff;
					fins_cmnd.body[bodylen++] = (chunk_start     )  & 0xff;
					fins_cmnd.body[bodylen++] = address.sub_address & 0xff;

					recvlen += 2;

					break;



				case FINS_DATA_TYPE_BIT_FORCED :

					if ( XX_finslib_decode_address( item[offset+a].address, & address ) ) return FINS_RETVAL_INVALID_READ_ADDRESS;

					area_ptr = XX_finslib_search_area( sys, & address, 1, FI_MRD, true );
					if ( area_ptr == NULL ) return FINS_RETVAL_INVALID_READ_AREA;

					chunk_start  = address.main_address;
					chunk_start += area_ptr->low_addr >> 8;
					chunk_start -= area_ptr->low_id;

					fins_cmnd.body[bodylen++] = area_ptr->area;
					fins_cmnd.body[bodylen++] = (chunk_start >> 8)  & 0xff;
					fins_cmnd.body[bodylen++] = (chunk_start     )  & 0xff;
					fins_cmnd.body[bodylen++] = address.sub_address & 0xff;

					recvlen += 2;

					break;



				case FINS_DATA_TYPE_WORD_FORCED :

					if ( XX_finslib_decode_address( item[offset+a].address, & address ) ) return FINS_RETVAL_INVALID_READ_ADDRESS;

					area_ptr = XX_finslib_search_area( sys, & address, 16, FI_MRD, true );
					if ( area_ptr == NULL ) return FINS_RETVAL_INVALID_READ_AREA;

					chunk_start  = address.main_address;
					chunk_start += area_ptr->low_addr >> 8;
					chunk_start -= area_ptr->low_id;

					fins_cmnd.body[bodylen++] = area_ptr->area;
					fins_cmnd.body[bodylen++] = (chunk_start >> 8) & 0xff;
					fins_cmnd.body[bodylen++] = (chunk_start     ) & 0xff;
					fins_cmnd.body[bodylen++] = 0x00;

					recvlen += 5;

					break;



				case FINS_DATA_TYPE_INT32    :
				case FINS_DATA_TYPE_UINT32   :
				case FINS_DATA_TYPE_BCD32    :
				case FINS_DATA_TYPE_SBCD32_0 :
				case FINS_DATA_TYPE_SBCD32_1 :
				case FINS_DATA_TYPE_SBCD32_2 :
				case FINS_DATA_TYPE_SBCD32_3 :
				case FINS_DATA_TYPE_FLOAT    :

					if ( XX_finslib_decode_address( item[offset+a].address, & address ) ) return FINS_RETVAL_INVALID_READ_ADDRESS;

					area_ptr = XX_finslib_search_area( sys, & address, 16, FI_MRD, false );
					if ( area_ptr == NULL ) return FINS_RETVAL_INVALID_READ_AREA;

					chunk_start  = address.main_address;
					chunk_start += area_ptr->low_addr >> 8;
					chunk_start -= area_ptr->low_id;

					fins_cmnd.body[bodylen++] = area_ptr->area;
					fins_cmnd.body[bodylen++] = (chunk_start >> 8) & 0xff;
					fins_cmnd.body[bodylen++] = (chunk_start     ) & 0xff;
					fins_cmnd.body[bodylen++] = 0x00;

					chunk_start++;

					fins_cmnd.body[bodylen++] = area_ptr->area;
					fins_cmnd.body[bodylen++] = (chunk_start >> 8) & 0xff;
					fins_cmnd.body[bodylen++] = (chunk_start     ) & 0xff;
					fins_cmnd.body[bodylen++] = 0x00;

					recvlen += 6;

					break;



				case FINS_DATA_TYPE_DOUBLE :

					if ( XX_finslib_decode_address( item[offset+a].address, & address ) ) return FINS_RETVAL_INVALID_READ_ADDRESS;

					area_ptr = XX_finslib_search_area( sys, & address, 16, FI_MRD, false );
					if ( area_ptr == NULL ) return FINS_RETVAL_INVALID_READ_AREA;

					chunk_start  = address.main_address;
					chunk_start += area_ptr->low_addr >> 8;
					chunk_start -= area_ptr->low_id;

					fins_cmnd.body[bodylen++] = area_ptr->area;
					fins_cmnd.body[bodylen++] = (chunk_start >> 8) & 0xff;
					fins_cmnd.body[bodylen++] = (chunk_start     ) & 0xff;
					fins_cmnd.body[bodylen++] = 0x00;

					chunk_start++;

					fins_cmnd.body[bodylen++] = area_ptr->area;
					fins_cmnd.body[bodylen++] = (chunk_start >> 8) & 0xff;
					fins_cmnd.body[bodylen++] = (chunk_start     ) & 0xff;
					fins_cmnd.body[bodylen++] = 0x00;

					chunk_start++;

					fins_cmnd.body[bodylen++] = area_ptr->area;
					fins_cmnd.body[bodylen++] = (chunk_start >> 8) & 0xff;
					fins_cmnd.body[bodylen++] = (chunk_start     ) & 0xff;
					fins_cmnd.body[bodylen++] = 0x00;

					chunk_start++;

					fins_cmnd.body[bodylen++] = area_ptr->area;
					fins_cmnd.body[bodylen++] = (chunk_start >> 8) & 0xff;
					fins_cmnd.body[bodylen++] = (chunk_start     ) & 0xff;
					fins_cmnd.body[bodylen++] = 0x00;

					recvlen += 12;

					break;
			}
		}


		if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen, true ) ) != FINS_RETVAL_SUCCESS ) return retval;

		if ( bodylen != recvlen ) return FINS_RETVAL_BODY_TOO_SHORT;

		bodylen = 2;

		for (a=0; a<chunk_length; a++) {

			bodylen++;

			switch ( item[offset+a].type ) {

				case FINS_DATA_TYPE_INT16 :

					item[offset+a].int16   = fins_cmnd.body[bodylen+0];
					item[offset+a].int16 <<= 8;
					item[offset+a].int16  += fins_cmnd.body[bodylen+1];

					bodylen += 2;

					break;



				case FINS_DATA_TYPE_INT32 :

					item[offset+a].int32   = fins_cmnd.body[bodylen+3];
					item[offset+a].int32 <<= 8;
					item[offset+a].int32  += fins_cmnd.body[bodylen+4];
					item[offset+a].int32 <<= 8;
					item[offset+a].int32  += fins_cmnd.body[bodylen+0];
					item[offset+a].int32 <<= 8;
					item[offset+a].int32  += fins_cmnd.body[bodylen+1];

					bodylen += 5;

					break;



				case FINS_DATA_TYPE_UINT16 :

					item[offset+a].uint16   = fins_cmnd.body[bodylen+0];
					item[offset+a].uint16 <<= 8;
					item[offset+a].uint16  += fins_cmnd.body[bodylen+1];

					bodylen += 2;

					break;



				case FINS_DATA_TYPE_UINT32 :

					item[offset+a].uint32   = fins_cmnd.body[bodylen+3];
					item[offset+a].uint32 <<= 8;
					item[offset+a].uint32  += fins_cmnd.body[bodylen+4];
					item[offset+a].uint32 <<= 8;
					item[offset+a].uint32  += fins_cmnd.body[bodylen+0];
					item[offset+a].uint32 <<= 8;
					item[offset+a].uint32  += fins_cmnd.body[bodylen+1];

					bodylen += 5;

					break;



				case FINS_DATA_TYPE_FLOAT :

					sfloat.val_raw[0] = fins_cmnd.body[0];
					sfloat.val_raw[1] = fins_cmnd.body[1];
					sfloat.val_raw[2] = fins_cmnd.body[3];
					sfloat.val_raw[3] = fins_cmnd.body[4];

					item[offset+a].sfloat = sfloat.val_float;

					bodylen += 5;

					break;



				case FINS_DATA_TYPE_DOUBLE :

					dfloat.val_raw[0] = fins_cmnd.body[0];
					dfloat.val_raw[1] = fins_cmnd.body[1];
					dfloat.val_raw[2] = fins_cmnd.body[3];
					dfloat.val_raw[3] = fins_cmnd.body[4];
					dfloat.val_raw[4] = fins_cmnd.body[6];
					dfloat.val_raw[5] = fins_cmnd.body[7];
					dfloat.val_raw[6] = fins_cmnd.body[9];
					dfloat.val_raw[7] = fins_cmnd.body[10];

					item[offset+a].dfloat = dfloat.val_double;

					bodylen += 11;

					break;



				case FINS_DATA_TYPE_BCD16 :

					bcd_val   = fins_cmnd.body[bodylen+0];
					bcd_val <<= 8;
					bcd_val  += fins_cmnd.body[bodylen+1];
					bodylen  += 2;

					item[offset+a].uint16 = (uint16_t) finslib_bcd_to_int( bcd_val, FINS_DATA_TYPE_BCD16 );

					break;



				case FINS_DATA_TYPE_SBCD16_0 :
				case FINS_DATA_TYPE_SBCD16_1 :
				case FINS_DATA_TYPE_SBCD16_2 :
				case FINS_DATA_TYPE_SBCD16_3 :

					bcd_val   = fins_cmnd.body[bodylen+0];
					bcd_val <<= 8;
					bcd_val  += fins_cmnd.body[bodylen+1];
					bodylen  += 2;

					item[offset+a].int16 = (int16_t) finslib_bcd_to_int( bcd_val, item[offset+a].type );

					break;



				case FINS_DATA_TYPE_BCD32 :

					bcd_val   = fins_cmnd.body[bodylen+3];
					bcd_val <<= 8;
					bcd_val  += fins_cmnd.body[bodylen+4];
					bcd_val <<= 8;
					bcd_val  += fins_cmnd.body[bodylen+0];
					bcd_val <<= 8;
					bcd_val  += fins_cmnd.body[bodylen+1];
					bodylen  += 5;

					item[offset+a].uint32 = finslib_bcd_to_int( bcd_val, FINS_DATA_TYPE_BCD32 );

					break;



				case FINS_DATA_TYPE_SBCD32_0 :
				case FINS_DATA_TYPE_SBCD32_1 :
				case FINS_DATA_TYPE_SBCD32_2 :
				case FINS_DATA_TYPE_SBCD32_3 :

					bcd_val   = fins_cmnd.body[bodylen+3];
					bcd_val <<= 8;
					bcd_val  += fins_cmnd.body[bodylen+4];
					bcd_val <<= 8;
					bcd_val  += fins_cmnd.body[bodylen+0];
					bcd_val <<= 8;
					bcd_val  += fins_cmnd.body[bodylen+1];
					bodylen  += 5;

					item[offset+a].int32 = finslib_bcd_to_int( bcd_val, item[offset+a].type );

					break;



				case FINS_DATA_TYPE_BIT :

					item[offset+a].bit     = fins_cmnd.body[bodylen] & 0x01;
					item[offset+a].b_force = false;

					bodylen++;

					break;



				case FINS_DATA_TYPE_BIT_FORCED :

					item[offset+a].bit     = fins_cmnd.body[bodylen] & 0x01;
					item[offset+a].b_force = fins_cmnd.body[bodylen] & 0x02;

					bodylen++;

					break;



				case FINS_DATA_TYPE_WORD_FORCED :

					item[offset+a].w_force   = fins_cmnd.body[bodylen+0];
					item[offset+a].w_force <<= 8;
					item[offset+a].w_force  += fins_cmnd.body[bodylen+1];

					item[offset+a].word      = fins_cmnd.body[bodylen+2];
					item[offset+a].word    <<= 8;
					item[offset+a].word     += fins_cmnd.body[bodylen+3];

					bodylen += 4;

					break;
			}
		}


		todo   -= chunk_length;
		offset += chunk_length;

	} while ( todo > 0 );

	return FINS_RETVAL_SUCCESS;

}  /* finslib_multiple_memory_area_read */
