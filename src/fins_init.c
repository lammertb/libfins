/*
 * Library: libfins
 * File:    src/fins_init.c
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
 * The source file src/fins_init.c contains routines to initialize the FINS
 * command structure of a message to be sent to a remote PLC over the Omron
 * FINS protocol.
 */

#define FINS_SOCKET_INCLUDE	1

#include "fins.h"

/*
 * void XX_finslib_init_command( fins_command_tp *command, uint8_t snn, uint8_t cnn );
 *
 * The function XX_finslib_init_command() initializes a FINS command structure
 * which will be used to contain a command which is to be sent to a remote FINS
 * server like an Omron PLC.
 */

void XX_finslib_init_command( struct fins_sys_tp *sys, struct fins_command_tp *command, uint8_t mrc, uint8_t src ) {

	command->header[FINS_ICF] = 0x80;
	command->header[FINS_RSV] = 0x00;
	command->header[FINS_GCT] = 0x02;
	command->header[FINS_DNA] = sys->remote_net;
	command->header[FINS_DA1] = sys->remote_node;
	command->header[FINS_DA2] = sys->remote_unit;
	command->header[FINS_SNA] = sys->local_net;
	command->header[FINS_SA1] = sys->local_node;
	command->header[FINS_SA2] = sys->local_unit;
	command->header[FINS_SID] = sys->sid++;
	command->header[FINS_MRC] = mrc;
	command->header[FINS_SRC] = src;

} /* XX_finslib_init_command */
