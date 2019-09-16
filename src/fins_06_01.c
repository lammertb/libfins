/*
 * Library: libfins
 * File:    src/fins_06_01.c
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
 * The source file src/fins_06_01.c contains routines to read the actual status
 * of a remote PLC over the FINS protocol.
 */

#define FINS_SOCKET_INCLUDE	1

#include <ctype.h>
#include <string.h>
#include "fins.h"

/*
 * int finslib_cpu_unit_status_read( fins_sys_tp *sys, fins_cpustatus_tp *status );
 *
 * The function fins_cpu_unit_status_read() reads the status of the CPU unit of
 * a remote PLC. The status is stored in a structure. If the function succeeds
 * the value false is returned. The return value true is an indication of an
 * error.
 */

int finslib_cpu_unit_status_read( struct fins_sys_tp *sys, struct fins_cpustatus_tp *status ) {

	struct fins_command_tp fins_cmnd;
	int a;
	int retval;
	size_t bodylen;

	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( status      == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	XX_finslib_init_command( sys, & fins_cmnd, 0x06, 0x01 );

	bodylen = 0;

	if ( ( retval = XX_finslib_communicate( sys, & fins_cmnd, & bodylen, true ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen != 28 ) return FINS_RETVAL_BODY_TOO_SHORT;

	status->running                        = fins_cmnd.body[2] & 0x01;
	status->flash_writing                  = fins_cmnd.body[2] & 0x02;
	status->battery_present                = fins_cmnd.body[2] & 0x04;
	status->standby                        = fins_cmnd.body[2] & 0x80;

	status->run_mode                       = fins_cmnd.body[3];

	status->fatal_memory_error             = fins_cmnd.body[4] & 0x80;
	status->fatal_io_bus_error             = fins_cmnd.body[4] & 0x40;
	status->fatal_duplication_error        = fins_cmnd.body[4] & 0x20;
	status->fatal_inner_board_error        = fins_cmnd.body[4] & 0x10;
	status->fatal_io_point_overflow        = fins_cmnd.body[4] & 0x08;
	status->fatal_io_setting_error         = fins_cmnd.body[4] & 0x04;
	status->fatal_program_error            = fins_cmnd.body[4] & 0x02;
	status->fatal_cycle_time_over          = fins_cmnd.body[4] & 0x01;
	status->fatal_fals_error               = fins_cmnd.body[5] & 0x40;

	status->fal_error                      = fins_cmnd.body[6] & 0x80;
	status->duplex_error                   = fins_cmnd.body[6] & 0x40;
	status->interrupt_task_error           = fins_cmnd.body[6] & 0x20;
	status->basic_io_unit_error            = fins_cmnd.body[6] & 0x10;
	status->plc_setup_error                = fins_cmnd.body[6] & 0x04;
	status->io_verification_error          = fins_cmnd.body[6] & 0x02;
	status->inner_board_error              = fins_cmnd.body[6] & 0x01;
	status->cpu_bus_unit_error             = fins_cmnd.body[7] & 0x80;
	status->special_io_unit_error          = fins_cmnd.body[7] & 0x40;
	status->sysmac_bus_error               = fins_cmnd.body[7] & 0x20;
	status->battery_error                  = fins_cmnd.body[7] & 0x10;
	status->cs1_cpu_bus_unit_setting_error = fins_cmnd.body[7] & 0x08;
	status->special_io_unit_setting_error  = fins_cmnd.body[7] & 0x04;

	status->message_exists[0]              = fins_cmnd.body[9] & 0x01;
	status->message_exists[1]              = fins_cmnd.body[9] & 0x02;
	status->message_exists[2]              = fins_cmnd.body[9] & 0x04;
	status->message_exists[3]              = fins_cmnd.body[9] & 0x08;
	status->message_exists[4]              = fins_cmnd.body[9] & 0x10;
	status->message_exists[5]              = fins_cmnd.body[9] & 0x20;
	status->message_exists[6]              = fins_cmnd.body[9] & 0x40;
	status->message_exists[7]              = fins_cmnd.body[9] & 0x80;

	status->error_code                     = fins_cmnd.body[10];
	status->error_code                   <<= 8;
	status->error_code                    += fins_cmnd.body[11];

	memcpy( status->error_message, & fins_cmnd.body[12], 16 );
	status->error_message[16] = 0;

	a = 16;
	while ( a > 0  &&  isspace( status->error_message[a-1] ) ) a--;
	status->error_message[a] = 0;

	return FINS_RETVAL_SUCCESS;

}  /* finslib_cpu_unit_status_read */
