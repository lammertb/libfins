/*
 * Library: libfins
 * File:    src/fins_error.c
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
 * The module fins_error.c contains routines to create human readable error
 * messages from FINS error codes.
 */

#define FINS_SOCKET_INCLUDE	1

#include <stdio.h>
#include <string.h>
#include "fins.h"

#ifndef STRERROR
#if defined(_WIN32)
#define STRERROR(erno,buf,len)	strerror_s((buf),(len),(erno))
#else
#define STRERROR(erno,buf,len)	strerror_r((erno),(buf),(len))
#endif
#endif

/*
 * const char *finslib_errmsg( int error_code, char *buffer, size_t buffer_len );
 *
 * The function finslib_errmsg() generates human readable error messages for
 * the error codes which can be returned by finslib functions.
 */

const char *finslib_errmsg( int error_code, char *buffer, size_t buffer_len ) {

	if ( buffer == NULL  ||  buffer_len < 1 ) return NULL;

	if ( error_code > FINS_RETVAL_ERRNO_BASE ) {
		
		STRERROR( error_code - FINS_RETVAL_ERRNO_BASE, buffer, buffer_len );
		return buffer;
	}

	snprintf( buffer, buffer_len, "Unknown error" );

	switch ( error_code ) {

		case FINS_RETVAL_SUCCESS                     : snprintf( buffer, buffer_len, "Operation was successful"                           ); break;
		case FINS_RETVAL_SUCCESS_LAST_DATA           : snprintf( buffer, buffer_len, "Read successful and last data delivered"            ); break;
		case FINS_RETVAL_CANCELED                    : snprintf( buffer, buffer_len, "Service was canceled"                               ); break;
		case FINS_RETVAL_ABORTED                     : snprintf( buffer, buffer_len, "Services was aborted"                               ); break;
		case FINS_RETVAL_MAX_ERROR_COUNT             : snprintf( buffer, buffer_len, "Connection closed: error count exceeded"            ); break;
		case FINS_RETVAL_SYNC_ERROR                  : snprintf( buffer, buffer_len, "Synchronization error"                              ); break;

		case FINS_RETVAL_CLOSED_BY_REMOTE            : snprintf( buffer, buffer_len, "FINS/TCP connection closed by remote"               ); break;
		case FINS_RETVAL_NO_FINS_HEADER              : snprintf( buffer, buffer_len, "FINS/TCP missing FINS header"                       ); break;
		case FINS_RETVAL_DATA_LENGTH_TOO_LONG        : snprintf( buffer, buffer_len, "FINS/TCP data length too long"                      ); break;
		case FINS_RETVAL_COMMAND_NOT_SUPPORTED       : snprintf( buffer, buffer_len, "FINS/TCP command not supported"                     ); break;
		case FINS_RETVAL_ALL_CONNECTIONS_IN_USE      : snprintf( buffer, buffer_len, "FINS/TCP all connections in use"                    ); break;
		case FINS_RETVAL_NODE_ALREADY_CONNECTED      : snprintf( buffer, buffer_len, "FINS/TCP node already connected"                    ); break;
		case FINS_RETVAL_NODE_IP_PROTECTED           : snprintf( buffer, buffer_len, "FINS/TCP node IP protected"                         ); break;
		case FINS_RETVAL_CLIENT_NODE_OUT_OF_RANGE    : snprintf( buffer, buffer_len, "FINS/TCP client node out of range"                  ); break;
		case FINS_RETVAL_SAME_NODE_ADDRESS           : snprintf( buffer, buffer_len, "FINS/TCP client and server same address"            ); break;
		case FINS_RETVAL_NO_NODE_ADDRESS_AVAILABLE   : snprintf( buffer, buffer_len, "FINS/TCP no node address available"                 ); break;
		case FINS_RETVAL_TRY_LATER                   : snprintf( buffer, buffer_len, "Please try again later"                             ); break;

		case FINS_RETVAL_NOT_INITIALIZED             : snprintf( buffer, buffer_len, "Connection not initialized"                         ); break;
		case FINS_RETVAL_NOT_CONNECTED               : snprintf( buffer, buffer_len, "TCP connection not established"                     ); break;
		case FINS_RETVAL_OUT_OF_MEMORY               : snprintf( buffer, buffer_len, "Out of memory"                                      ); break;
		case FINS_RETVAL_INVALID_IP_ADDRESS          : snprintf( buffer, buffer_len, "Invalid IP address"                                 ); break;

		case FINS_RETVAL_NO_READ_ADDRESS             : snprintf( buffer, buffer_len, "No remote read address specified"                   ); break;
		case FINS_RETVAL_NO_WRITE_ADDRESS            : snprintf( buffer, buffer_len, "No remote write address specified"                  ); break;
		case FINS_RETVAL_NO_DATA_BLOCK               : snprintf( buffer, buffer_len, "No local data storage specified"                    ); break;

		case FINS_RETVAL_INVALID_READ_ADDRESS        : snprintf( buffer, buffer_len, "Read address syntax error"                          ); break;
		case FINS_RETVAL_INVALID_WRITE_ADDRESS       : snprintf( buffer, buffer_len, "Write address syntax error"                         ); break;

		case FINS_RETVAL_INVALID_READ_AREA           : snprintf( buffer, buffer_len, "Invalid reading area"                               ); break;
		case FINS_RETVAL_INVALID_WRITE_AREA          : snprintf( buffer, buffer_len, "Invalid writing area"                               ); break;
		case FINS_RETVAL_INVALID_FILL_AREA           : snprintf( buffer, buffer_len, "Invalid filling area"                               ); break;
		case FINS_RETVAL_INVALID_PARAMETER_AREA      : snprintf( buffer, buffer_len, "Invalid parameter area"                             ); break;
		case FINS_RETVAL_INVALID_DISK                : snprintf( buffer, buffer_len, "Invalid disk"                                       ); break;
		case FINS_RETVAL_INVALID_PATH                : snprintf( buffer, buffer_len, "Invalid path"                                       ); break;
		case FINS_RETVAL_INVALID_FILENAME            : snprintf( buffer, buffer_len, "Invalid filename"                                   ); break;

		case FINS_RETVAL_INVALID_DATE                : snprintf( buffer, buffer_len, "Invalid date"                                       ); break;

		case FINS_RETVAL_NO_COMMAND                  : snprintf( buffer, buffer_len, "No command specified"                               ); break;
		case FINS_RETVAL_NO_COMMAND_LENGTH           : snprintf( buffer, buffer_len, "No command length specified"                        ); break;
		case FINS_RETVAL_BODY_TOO_SHORT              : snprintf( buffer, buffer_len, "Command body too short"                             ); break;
		case FINS_RETVAL_BODY_TOO_LONG               : snprintf( buffer, buffer_len, "Command body too long"                              ); break;
		case FINS_RETVAL_HEADER_SEND_ERROR           : snprintf( buffer, buffer_len, "Header send error"                                  ); break;
		case FINS_RETVAL_COMMAND_SEND_ERROR          : snprintf( buffer, buffer_len, "Command send error"                                 ); break;
		case FINS_RETVAL_RESPONSE_INCOMPLETE         : snprintf( buffer, buffer_len, "Response incomplete"                                ); break;
		case FINS_RETVAL_ILLEGAL_FINS_COMMAND        : snprintf( buffer, buffer_len, "Illegal command"                                    ); break;
		case FINS_RETVAL_RESPONSE_HEADER_INCOMPLETE  : snprintf( buffer, buffer_len, "Response header incomplete"                         ); break;
		case FINS_RETVAL_INVALID_FORCE_COMMAND       : snprintf( buffer, buffer_len, "Invalid force command"                              ); break;

		case FINS_RETVAL_LOCAL_NODE_NOT_IN_NETWORK   : snprintf( buffer, buffer_len, "Local node not in network"                          ); break;
		case FINS_RETVAL_LOCAL_TOKEN_TIMEOUT         : snprintf( buffer, buffer_len, "Local node token timeout"                           ); break;
		case FINS_RETVAL_LOCAL_RETRIES_FAILED        : snprintf( buffer, buffer_len, "Local node retries failed"                          ); break;
		case FINS_RETVAL_LOCAL_TOO_MANY_SEND_FRAMES  : snprintf( buffer, buffer_len, "Local node maximum number of event frames exceeded" ); break;
		case FINS_RETVAL_LOCAL_ADDRESS_RANGE_ERROR   : snprintf( buffer, buffer_len, "Local node address setting error"                   ); break;
		case FINS_RETVAL_LOCAL_ADDRESS_DUPLICATION   : snprintf( buffer, buffer_len, "Local node address duplication"                     ); break;

		case FINS_RETVAL_DEST_NOT_IN_NETWORK         : snprintf( buffer, buffer_len, "Destination not in network"                         ); break;
		case FINS_RETVAL_DEST_UNIT_MISSING           : snprintf( buffer, buffer_len, "Destination unit missing"                           ); break;
		case FINS_RETVAL_DEST_THIRD_NODE_MISSING     : snprintf( buffer, buffer_len, "Destination third node missing"                     ); break;
		case FINS_RETVAL_DEST_NODE_BUSY              : snprintf( buffer, buffer_len, "Destination node is busy"                           ); break;
		case FINS_RETVAL_DEST_TIMEOUT                : snprintf( buffer, buffer_len, "Destination response timeout"                       ); break;

		case FINS_RETVAL_CONTR_COMM_ERROR            : snprintf( buffer, buffer_len, "Communications controller error"                    ); break;
		case FINS_RETVAL_CONTR_CPU_UNIT_ERROR        : snprintf( buffer, buffer_len, "CPU error in destination CPU unit"                  ); break;
		case FINS_RETVAL_CONTR_BOARD_ERROR           : snprintf( buffer, buffer_len, "Error in controller board"                          ); break;
		case FINS_RETVAL_CONTR_UNIT_NUMBER_ERROR     : snprintf( buffer, buffer_len, "Invalid unit number on controller"                  ); break;

		case FINS_RETVAL_UNSUPPORTED_COMMAND         : snprintf( buffer, buffer_len, "Service not supported by the unit/board"            ); break;
		case FINS_RETVAL_UNSUPPORTED_VERSION         : snprintf( buffer, buffer_len, "Service not supported by this model/version"        ); break;

		case FINS_RETVAL_ROUTING_ADDRESS_ERROR       : snprintf( buffer, buffer_len, "Destination not present in routing tables"          ); break;
		case FINS_RETVAL_ROUTING_NO_TABLES           : snprintf( buffer, buffer_len, "No routing tables present"                          ); break;
		case FINS_RETVAL_ROUTING_TABLE_ERROR         : snprintf( buffer, buffer_len, "Routing tables contain errors"                      ); break;
		case FINS_RETVAL_ROUTING_TOO_MANY_RELAYS     : snprintf( buffer, buffer_len, "Too many hops to destination"                       ); break;

		case FINS_RETVAL_COMMAND_TOO_LONG            : snprintf( buffer, buffer_len, "Command too long"                                   ); break;
		case FINS_RETVAL_COMMAND_TOO_SHORT           : snprintf( buffer, buffer_len, "Command too short"                                  ); break;
		case FINS_RETVAL_COMMAND_ELEMENT_MISMATCH    : snprintf( buffer, buffer_len, "Mismatch between data and number of elements"       ); break;
		case FINS_RETVAL_COMMAND_FORMAT_ERROR        : snprintf( buffer, buffer_len, "Command format error"                               ); break;
		case FINS_RETVAL_COMMAND_HEADER_ERROR        : snprintf( buffer, buffer_len, "Routing tables inconsistent with header"            ); break;

		case FINS_RETVAL_PARAM_AREA_MISSING          : snprintf( buffer, buffer_len, "Incorrect area specified"                           ); break;
		case FINS_RETVAL_PARAM_ACCESS_SIZE_ERROR     : snprintf( buffer, buffer_len, "Access size specification is incorrect"             ); break;
		case FINS_RETVAL_PARAM_START_ADDRESS_ERROR   : snprintf( buffer, buffer_len, "Start address out of range"                         ); break;
		case FINS_RETVAL_PARAM_END_ADDRESS_ERROR     : snprintf( buffer, buffer_len, "End address out of range"                           ); break;
		case FINS_RETVAL_PARAM_PROGRAM_MISSING       : snprintf( buffer, buffer_len, "Program number missing"                             ); break;
		case FINS_RETVAL_PARAM_RELATIONAL_ERROR      : snprintf( buffer, buffer_len, "Relational error in parameters"                     ); break;
		case FINS_RETVAL_PARAM_DUPLICATE_ACCESS      : snprintf( buffer, buffer_len, "Duplicate data access"                              ); break;
		case FINS_RETVAL_PARAM_RESPONSE_TOO_LONG     : snprintf( buffer, buffer_len, "Response too long"                                  ); break;
		case FINS_RETVAL_PARAM_PARAMETER_ERROR       : snprintf( buffer, buffer_len, "Error in parameters"                                ); break;

		case FINS_RETVAL_RD_ERR_PROTECTED            : snprintf( buffer, buffer_len, "Program area is protected"                          ); break;
		case FINS_RETVAL_RD_ERR_TABLE_MISSING        : snprintf( buffer, buffer_len, "Table is missing or faulty"                         ); break;
		case FINS_RETVAL_RD_ERR_DATA_MISSING         : snprintf( buffer, buffer_len, "Data does not exist"                                ); break;
		case FINS_RETVAL_RD_ERR_PROGRAM_MISSING      : snprintf( buffer, buffer_len, "Program does not exist"                             ); break;
		case FINS_RETVAL_RD_ERR_FILE_MISSING         : snprintf( buffer, buffer_len, "File does not exist"                                ); break;
		case FINS_RETVAL_RD_ERR_DATA_MISMATCH        : snprintf( buffer, buffer_len, "Data comparison error"                              ); break;

		case FINS_RETVAL_WR_ERR_READ_ONLY            : snprintf( buffer, buffer_len, "Area is read only"                                  ); break;
		case FINS_RETVAL_WR_ERR_PROTECTED            : snprintf( buffer, buffer_len, "Program area is protected"                          ); break;
		case FINS_RETVAL_WR_ERR_CANNOT_REGISTER      : snprintf( buffer, buffer_len, "Maximum file number reached"                        ); break;
		case FINS_RETVAL_WR_ERR_PROGRAM_MISSING      : snprintf( buffer, buffer_len, "Non-existing program number"                        ); break;
		case FINS_RETVAL_WR_ERR_FILE_MISSING         : snprintf( buffer, buffer_len, "File does not exist"                                ); break;
		case FINS_RETVAL_WR_ERR_FILE_EXISTS          : snprintf( buffer, buffer_len, "File already exists"                                ); break;
		case FINS_RETVAL_WR_ERR_CANNOT_CHANGE        : snprintf( buffer, buffer_len, "Data cannot be changed"                             ); break;

		case FINS_RETVAL_MODE_NOT_DURING_EXECUTION   : snprintf( buffer, buffer_len, "Not possible during execution"                      ); break;
		case FINS_RETVAL_MODE_NOT_DURING_RUN         : snprintf( buffer, buffer_len, "Not possible while running"                         ); break;
		case FINS_RETVAL_MODE_IS_PROGRAM             : snprintf( buffer, buffer_len, "PLC in program mode"                                ); break;
		case FINS_RETVAL_MODE_IS_DEBUG               : snprintf( buffer, buffer_len, "PLC in debug mode"                                  ); break;
		case FINS_RETVAL_MODE_IS_MONITOR             : snprintf( buffer, buffer_len, "PLC in monitor mode"                                ); break;
		case FINS_RETVAL_MODE_IS_RUN                 : snprintf( buffer, buffer_len, "PLC in run mode"                                    ); break;
		case FINS_RETVAL_MODE_NODE_NOT_POLLING       : snprintf( buffer, buffer_len, "Node is not polling node"                           ); break;
		case FINS_RETVAL_MODE_NO_STEP                : snprintf( buffer, buffer_len, "Step cannot be executed"                            ); break;

		case FINS_RETVAL_DEVICE_FILE_MISSING         : snprintf( buffer, buffer_len, "No file device"                                     ); break;
		case FINS_RETVAL_DEVICE_MEMORY_MISSING       : snprintf( buffer, buffer_len, "No file memory"                                     ); break;
		case FINS_RETVAL_DEVICE_CLOCK_MISSING        : snprintf( buffer, buffer_len, "No clock"                                           ); break;

		case FINS_RETVAL_DATALINK_TABLE_MISSING      : snprintf( buffer, buffer_len, "Datalink tables missing or faulty"                  ); break;

		case FINS_RETVAL_UNIT_MEMORY_CONTENT_ERROR   : snprintf( buffer, buffer_len, "Memory content error"                               ); break;
		case FINS_RETVAL_UNIT_IO_SETTING_ERROR       : snprintf( buffer, buffer_len, "I/O setting mismatch"                               ); break;
		case FINS_RETVAL_UNIT_TOO_MANY_IO_POINTS     : snprintf( buffer, buffer_len, "Too many I/O points"                                ); break;
		case FINS_RETVAL_UNIT_CPU_BUS_ERROR          : snprintf( buffer, buffer_len, "CPU bus error"                                      ); break;
		case FINS_RETVAL_UNIT_IO_DUPLICATION         : snprintf( buffer, buffer_len, "I/O duplication"                                    ); break;
		case FINS_RETVAL_UNIT_IO_BUS_ERROR           : snprintf( buffer, buffer_len, "I/O bus error"                                      ); break;
		case FINS_RETVAL_UNIT_SYSMAC_BUS2_ERROR      : snprintf( buffer, buffer_len, "Sysmac BUS/2 error"                                 ); break;
		case FINS_RETVAL_UNIT_CPU_BUS_UNIT_ERROR     : snprintf( buffer, buffer_len, "CPU bus unit error"                                 ); break;
		case FINS_RETVAL_UNIT_SYSMAC_BUS_DUPLICATION : snprintf( buffer, buffer_len, "Sysmac bus duplication"                             ); break;
		case FINS_RETVAL_UNIT_MEMORY_ERROR           : snprintf( buffer, buffer_len, "Memory error"                                       ); break;
		case FINS_RETVAL_UNIT_SYSMAC_BUS_TERMINATOR  : snprintf( buffer, buffer_len, "Sysmac terminator missing"                          ); break;

		case FINS_RETVAL_COMMAND_NO_PROTECTION       : snprintf( buffer, buffer_len, "Area is not protected"                              ); break;
		case FINS_RETVAL_COMMAND_WRONG_PASSWORD      : snprintf( buffer, buffer_len, "Incorrect password"                                 ); break;
		case FINS_RETVAL_COMMAND_PROTECTED           : snprintf( buffer, buffer_len, "Area protected or node busy"                        ); break;
		case FINS_RETVAL_COMMAND_SERVICE_EXECUTING   : snprintf( buffer, buffer_len, "Service already executing"                          ); break;
		case FINS_RETVAL_COMMAND_SERVICE_STOPPED     : snprintf( buffer, buffer_len, "Service is stopped"                                 ); break;
		case FINS_RETVAL_COMMAND_NO_EXECUTION_RIGHT  : snprintf( buffer, buffer_len, "No execution rights"                                ); break;
		case FINS_RETVAL_COMMAND_SETTINGS_INCOMPLETE : snprintf( buffer, buffer_len, "Settings incomplete"                                ); break;
		case FINS_RETVAL_COMMAND_ITEMS_NOT_SET       : snprintf( buffer, buffer_len, "Items not set"                                      ); break;
		case FINS_RETVAL_COMMAND_ALREADY_DEFINED     : snprintf( buffer, buffer_len, "Action number already defined"                      ); break;
		case FINS_RETVAL_COMMAND_ERROR_WONT_CLEAR    : snprintf( buffer, buffer_len, "Cause still present, error will not clear"          ); break;

		case FINS_RETVAL_ACCESS_NO_RIGHTS            : snprintf( buffer, buffer_len, "Access right granted to other device"               ); break;

		case FINS_RETVAL_WSA_UNRECOGNIZED_ERROR      : snprintf( buffer, buffer_len, "WSA Unrecognized error"                             ); break;

		case FINS_RETVAL_WSA_NOT_INITIALIZED         : snprintf( buffer, buffer_len, "WSA Not initialized"                                ); break;
		case FINS_RETVAL_WSA_SYS_NOT_READY           : snprintf( buffer, buffer_len, "WSA Subsystem not ready"                            ); break;
		case FINS_RETVAL_WSA_VER_NOT_SUPPORTED       : snprintf( buffer, buffer_len, "WSA Version not supported"                          ); break;

		case FINS_RETVAL_WSA_E_ACCES                 : snprintf( buffer, buffer_len, "WSA Socket access violation"                        ); break;
		case FINS_RETVAL_WSA_E_ADDR_IN_USE           : snprintf( buffer, buffer_len, "WSA Address in use"                                 ); break;
		case FINS_RETVAL_WSA_E_ADDR_NOT_AVAIL        : snprintf( buffer, buffer_len, "WSA Address not available"                          ); break;
		case FINS_RETVAL_WSA_E_AF_NO_SUPPORT         : snprintf( buffer, buffer_len, "WSA Address familiy not supported"                  ); break;
		case FINS_RETVAL_WSA_E_ALREADY               : snprintf( buffer, buffer_len, "WSA Already in progress"                            ); break;
		case FINS_RETVAL_WSA_E_CONN_REFUSED          : snprintf( buffer, buffer_len, "WSA Connection refused"                             ); break;
		case FINS_RETVAL_WSA_E_FAULT                 : snprintf( buffer, buffer_len, "WSA Invalid parameter"                              ); break;
		case FINS_RETVAL_WSA_E_HOST_UNREACH          : snprintf( buffer, buffer_len, "WSA Host unreacheable"                              ); break;
		case FINS_RETVAL_WSA_E_IN_PROGRESS           : snprintf( buffer, buffer_len, "WSA Blocking 1.1 call in progress"                  ); break;
		case FINS_RETVAL_WSA_E_INTR                  : snprintf( buffer, buffer_len, "WSA Socket call cancelled"                          ); break;
		case FINS_RETVAL_WSA_E_INVAL                 : snprintf( buffer, buffer_len, "WSA Invalid argument"                               ); break;
		case FINS_RETVAL_WSA_E_INVALID_PROCTABLE     : snprintf( buffer, buffer_len, "WSA Invalid procedure table"                        ); break;
		case FINS_RETVAL_WSA_E_INVALID_PROVIDER      : snprintf( buffer, buffer_len, "WSA Invalid service provider"                       ); break;

		case FINS_RETVAL_WSA_E_IS_CONN               : snprintf( buffer, buffer_len, "WSA Already connected"                              ); break;
		case FINS_RETVAL_WSA_E_MFILE                 : snprintf( buffer, buffer_len, "WSA No more socket descriptors"                     ); break;
		case FINS_RETVAL_WSA_E_NET_DOWN              : snprintf( buffer, buffer_len, "WSA Network down"                                   ); break;
		case FINS_RETVAL_WSA_E_NET_RESET             : snprintf( buffer, buffer_len, "WSA Timeout during Keep Alive"                      ); break;
		case FINS_RETVAL_WSA_E_NET_UNREACH           : snprintf( buffer, buffer_len, "WSA Network unreacheable"                           ); break;
		case FINS_RETVAL_WSA_E_NOBUFS                : snprintf( buffer, buffer_len, "WSA No buffer space available"                      ); break;
		case FINS_RETVAL_WSA_E_NO_PROTO_OPT          : snprintf( buffer, buffer_len, "WSA Unsupported option"                             ); break;
		case FINS_RETVAL_WSA_E_NOT_CONN              : snprintf( buffer, buffer_len, "WSA Connection reset during Keep Alive"             ); break;
		case FINS_RETVAL_WSA_E_NOT_SOCK              : snprintf( buffer, buffer_len, "WSA Descriptor is not a socket"                     ); break;
		case FINS_RETVAL_WSA_E_PROC_LIM              : snprintf( buffer, buffer_len, "WSA Process limit reached"                          ); break;
		case FINS_RETVAL_WSA_E_PROTO_NO_SUPPORT      : snprintf( buffer, buffer_len, "WSA Protocol not supported"                         ); break;
		case FINS_RETVAL_WSA_E_PROTO_TYPE            : snprintf( buffer, buffer_len, "WSA Protocol is wrong type"                         ); break;
		case FINS_RETVAL_WSA_E_PROVIDER_FAILED_INIT  : snprintf( buffer, buffer_len, "WSA Service provider failed initialization"         ); break;
		case FINS_RETVAL_WSA_E_SOCKT_NO_SUPPORT      : snprintf( buffer, buffer_len, "WSA Socket type not supported"                      ); break;
		case FINS_RETVAL_WSA_E_TIMED_OUT             : snprintf( buffer, buffer_len, "WSA Conenction timed out"                           ); break;
		case FINS_RETVAL_WSA_E_WOULD_BLOCK           : snprintf( buffer, buffer_len, "WSA Operation would block"                          ); break;
	}

	return buffer;

}  /* finslib_errmsg */
