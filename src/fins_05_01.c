/*
 * Library: libfins
 * File:    src/fins_05_01.c
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
 * The source file src/fins_05_01.c contains routines to read the CPU unit data
 * from a remote PLC over the Omron FINS protocol. The routines use the FINS
 * function 05 01 for this task.
 */

#include <ctype.h>
#include <string.h>
#include "fins.h"

/*
 * int finslib_cpu_unit_data_read( fins_sys_tp *sys, fins_cpudata_tp *cpudata );
 *
 * The function finslib_cpu_unit_data_read() requests the CPU unit data from a
 * remote PLC over the Omron FINS protocol.
 * 
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_cpu_unit_data_read( struct fins_sys_tp *sys, struct fins_cpudata_tp *cpudata ) {

	int a;
	int retval;
	size_t bodylen;
	struct fins_command_tp fins_cmnd;

	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( cpudata     == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	XX_finslib_init_command( sys, & fins_cmnd, 0x05, 0x01 );

	bodylen = 0;

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen != 160 ) return FINS_RETVAL_BODY_TOO_SHORT;

	memcpy( cpudata->model, & fins_cmnd.body[2], 20 );
	cpudata->model[20] = 0;

	a = 20;
	while ( a > 0  &&  isspace( cpudata->model[a-1] ) ) a--;
	cpudata->model[a] = 0;

	memcpy( cpudata->version, & fins_cmnd.body[22], 20 );
	cpudata->version[20] = 0;

	a = 20;
	while ( a > 0  &&  isspace( cpudata->version[a-1] ) ) a--;
	cpudata->version[a] = 0;

	memcpy( sys->model,   cpudata->model,   21 );
	memcpy( sys->version, cpudata->version, 21 );

	if      ( cpudata->model[0] == 'C'  &&  cpudata->model[1] == 'S' ) sys->plc_mode = FINS_MODE_CS;
	else if ( cpudata->model[0] == 'C'  &&  cpudata->model[1] == 'J' ) sys->plc_mode = FINS_MODE_CS;
	else if ( cpudata->model[0] == 'C'  &&  cpudata->model[1] == 'V' ) sys->plc_mode = FINS_MODE_CV;
	else                                                               sys->plc_mode = FINS_MODE_UNKNOWN;

	memcpy( cpudata->system_block, & fins_cmnd.body[42], 40 );

	cpudata->dip_switch[0]           = fins_cmnd.body[42] & 0x01;
	cpudata->dip_switch[1]           = fins_cmnd.body[42] & 0x02;
	cpudata->dip_switch[2]           = fins_cmnd.body[42] & 0x04;
	cpudata->dip_switch[3]           = fins_cmnd.body[42] & 0x08;
	cpudata->dip_switch[4]           = fins_cmnd.body[42] & 0x10;
	cpudata->dip_switch[5]           = fins_cmnd.body[42] & 0x20;
	cpudata->dip_switch[6]           = fins_cmnd.body[42] & 0x40;
	cpudata->dip_switch[7]           = fins_cmnd.body[42] & 0x80;

	cpudata->largest_em_bank         = fins_cmnd.body[43];

	cpudata->program_area_size       = fins_cmnd.body[82];
	cpudata->program_area_size     <<= 8;
	cpudata->program_area_size      += fins_cmnd.body[83];

	cpudata->iom_size                = fins_cmnd.body[84];

	cpudata->number_of_dm_words      = fins_cmnd.body[85];
	cpudata->number_of_dm_words    <<= 8;
	cpudata->number_of_dm_words     += fins_cmnd.body[86];

	cpudata->timer_counter_size      = fins_cmnd.body[87];
	cpudata->em_non_file_memory_size = fins_cmnd.body[88];
	cpudata->memory_card_type        = fins_cmnd.body[91];

	cpudata->memory_card_size        = fins_cmnd.body[92];
	cpudata->memory_card_size      <<= 8;
	cpudata->memory_card_size       += fins_cmnd.body[93];

	cpudata->num_sysmac_bus_masters  = fins_cmnd.body[158];
	cpudata->num_racks               = fins_cmnd.body[159] & 0x0f;


	for (a=0; a<16; a++) {

		cpudata->bus_unit_id[a]      = fins_cmnd.body[94+2*a] & 0x7f;
		cpudata->bus_unit_id[a]    <<= 8;
		cpudata->bus_unit_id[a]     += fins_cmnd.body[95+2*a];

		cpudata->bus_unit_present[a] = fins_cmnd.body[94+2*a] & 0x80;
	}
	
	return FINS_RETVAL_SUCCESS;

}  /* finslib_cpu_unit_data_read */
