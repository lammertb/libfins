/*
 * Library: libfins
 * File:    include/fins.h
 * Author:  Lammert Bies
 *
 * This file is licensed under the MIT License as stated below
 *
 * Copyright (c) 2016-2020 Lammert Bies
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
 * The header file include/fins.h contains global definitions and prototypes
 * used for the communication over the Omron FINS protocol.
 */

#ifndef INCLUDED_FINSLIB_FINS_H
#define INCLUDED_FINSLIB_FINS_H

#if defined(_WIN32)
#include <winsock2.h>
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#if defined(_WIN32)
#define inet_pton				InetPton
#else
#define INVALID_SOCKET				(-1)
typedef int					SOCKET;
#define closesocket				close
#endif

									/********************************************************/
									/*							*/
#define FINS_ICF				0			/* Information Control Field				*/
#define FINS_RSV				1			/* Reserved						*/
#define FINS_GCT				2			/* Gateway Counter. Init op 0x07 naar CPU v2.0 of 0x02	*/
#define FINS_DNA				3			/* Destination Network Address (0..127) 0 = lokaal	*/
#define FINS_DA1				4			/* Destination Node Address (0..254) 0 = lokaal		*/
#define FINS_DA2				5			/* Destination Unit Address (0..31) 0 = CPU unit	*/
#define FINS_SNA				6			/* Source Network Address (0..127) 0 = lokaal		*/
#define FINS_SA1				7			/* Source Node Address (0..254) 0 = intern in PLC	*/
#define FINS_SA2				8			/* Source Unit Address (0..31) 0 = CPU unit		*/
#define FINS_SID				9			/* Service ID, uniek nummer 0..FF per commando		*/
#define FINS_MRC				10			/* Main Request Code					*/
#define FINS_SRC				11			/* Sub Request Code					*/
									/*							*/
									/********************************************************/
#define FINS_MRES				0
#define FINS_SRES				1
									/********************************************************/
									/*							*/
#define FINS_HEADER_LEN				12			/* Length of a FINS header				*/
#define FINS_BODY_LEN				2000			/* Maximum length of a FINS body			*/
#define FINS_MAX_TCP_HEADER			32			/* Maximum length of a FINS/TCP header			*/
									/*							*/
									/********************************************************/

									/********************************************************/
									/*							*/
#define FINS_PORT_MIN				0			/* Lowest port number for TCP/UDP communications	*/
#define FINS_PORT_RESERVED			1024			/* End of reserved TCP/UDP port number block		*/
#define FINS_PORT_MAX				65535			/* Highest port number for TCP/UDP communications	*/
#define FINS_DEFAULT_PORT			9600			/* Default port for FINS TCP and UDP communications	*/
									/*							*/
									/********************************************************/

#define FINS_TIMEOUT				60


									/********************************************************/
									/*							*/
#define FINS_MODE_UNKNOWN			0			/* PLC communication mode unknown			*/
#define FINS_MODE_CV				1			/* PLC communicates like a CV PLC			*/
#define FINS_MODE_CS				2			/* PLC communicates like a CS/CJ PLC			*/
									/*							*/
									/********************************************************/

									/********************************************************/
									/*							*/
#define FINS_CPU_MODE_PROGRAM			0			/* The CPU is in program mode				*/
#define FINS_CPU_MODE_MONITOR			2			/* The CPU is in monitor mode				*/
#define FINS_CPU_MODE_RUN			4			/* The CPU is in run mode				*/
									/*							*/
									/********************************************************/

#define FI_RD					0x01
#define FI_WR					0x02
#define FI_FILL					0x04
#define FI_MRD					0x08
#define FI_TRS					0x10
#define FI_TRD					0x20
#define FI_FRC					0x40

									/********************************************************/
									/*							*/
#define FINS_MAX_READ_WORDS_SYSWAY		269			/* Max number of read words reading over SYSWAY		*/
#define FINS_MAX_READ_WORDS_ETHERNET		999			/* Max number of read words reading over Ethernet	*/
#define FINS_MAX_READ_WORDS_CLINK		999			/* Max number of read words reading over C-Link		*/
#define FINS_MAX_READ_WORDS_SYSMAC_LINK		269			/* Max number of read words reading over Sysmac Link	*/
#define FINS_MAX_READ_WORDS_DEVICENET		269			/* Max number of read words reading over DeviceNet	*/
									/*							*/
#define FINS_MAX_WRITE_WORDS_SYSWAY		267			/* Max number of write words writing over SYSWAY	*/
#define FINS_MAX_WRITE_WORDS_ETHERNET		996			/* Max number of write words writing over Ethernet	*/
#define FINS_MAX_WRITE_WORDS_CLINK		996			/* Max numver of write words writing over C-Link	*/
#define FINS_MAX_WRITE_WORDS_SYSMAC_LINK	267			/* Max number of write words writing over Sysmac Link	*/
#define FINS_MAX_WRITE_WORDS_DEVICENET		267			/* Max number of write words writing over DeviceNet	*/
									/*							*/
									/********************************************************/

									/********************************************************/
									/*							*/
#define FINS_COMM_TYPE_UNKNOWN			0x00			/* No communication protocol has been selected		*/
#define FINS_COMM_TYPE_TCP			0x01			/* The communication protocol is FINS/TCP		*/
#define FINS_COMM_TYPE_UDP			0x02			/* The communication protocol is FINS/UDP		*/
									/*							*/
									/********************************************************/

									/********************************************************/
									/*							*/
#define FINS_DATA_TYPE_NONE			0			/* No data type						*/
#define FINS_DATA_TYPE_INT16			1			/* 16 bit signed integer				*/
#define FINS_DATA_TYPE_INT32			2			/* 32 bit signed integer				*/
#define FINS_DATA_TYPE_UINT16			3			/* 16 bit unsigned integer				*/
#define FINS_DATA_TYPE_UINT32			4			/* 32 bit unsigned integer				*/
#define FINS_DATA_TYPE_BCD16			5			/* Unsigned 16 bit BCD in the range 0..9999		*/
#define FINS_DATA_TYPE_BCD32			6			/* Unsigned 32 bit BCD in the range 0..99999999		*/
#define FINS_DATA_TYPE_SBCD16_0			7			/* Signed 16 bit BCD in the range -999..999		*/
#define FINS_DATA_TYPE_SBCD16_1			8			/* Signed 16 bit BCD in the range -7999..7999		*/
#define FINS_DATA_TYPE_SBCD16_2			9			/* Signed 16 bit BCD in the range -999..9999		*/
#define FINS_DATA_TYPE_SBCD16_3			10			/* Signed 16 bit BCD in the range -1999..9999		*/
#define FINS_DATA_TYPE_SBCD32_0			11			/* Signed 32 bit BCD in the range -9999999..9999999	*/
#define FINS_DATA_TYPE_SBCD32_1			12			/* Signed 32 bit BCD in the range -79999999..79999999	*/
#define FINS_DATA_TYPE_SBCD32_2			13			/* Signed 32 bit BCD in the range -9999999..99999999	*/
#define FINS_DATA_TYPE_SBCD32_3			14			/* Signed 32 bit BCD in the range -19999999..99999999	*/
#define FINS_DATA_TYPE_FLOAT			15			/* 32 bit floating point value				*/
#define FINS_DATA_TYPE_DOUBLE			16			/* 64 bit floating point value				*/
#define FINS_DATA_TYPE_BIT			17			/* Single bit						*/
#define FINS_DATA_TYPE_BIT_FORCED		18			/* Single bit with forced status			*/
#define FINS_DATA_TYPE_WORD_FORCED		19			/* 16 bit word with for each bit the forced status	*/
#define FINS_DATA_TYPE_LAST			19			/* Last defined data type				*/
									/*							*/
									/********************************************************/

#define FINS_MEMORY_CARD_NONE			0
#define FINS_MEMORY_CARD_FLASH			4

#define FINS_MSG_0				0x01
#define FINS_MSG_1				0x02
#define FINS_MSG_2				0x04
#define FINS_MSG_3				0x08
#define FINS_MSG_4				0x10
#define FINS_MSG_5				0x20
#define FINS_MSG_6				0x40
#define FINS_MSG_7				0x80
#define FINS_MSG_ALL				0xFF

									/********************************************************/
									/*							*/
#define FINS_FORCE_RESET			0x0000			/* Force the bit and reset it				*/
#define FINS_FORCE_SET				0x0001			/* Force the bit and set it				*/
#define FINS_FORCE_RELEASE_TO_OFF		0x8000			/* Release the force and reset the bit			*/
#define FINS_FORCE_RELEASE_TO_ON		0x8001			/* Release the force and set the bit			*/
#define FINS_FORCE_RELEASE			0xFFFF			/* Release the force					*/
									/*							*/
									/********************************************************/

#define FINS_WRITE_MODE_NEW_NOT_OVERWRITE	0x0000
#define FINS_WRITE_MODE_NEW_OVERWRITE		0x0001
#define FINS_WRITE_MODE_ADD_DATA		0x0002
#define FINS_WRITE_MODE_OVERWRITE		0x0003

									/********************************************************/
									/*							*/
#define FINS_PARAM_AREA_ALL			0x8000			/* Pseudo value for all parameter areas			*/
#define FINS_PARAM_AREA_PLC_SETUP		0x8010			/* Parameter area for the PLC setup			*/
#define FINS_PARAM_AREA_IO_TABLE_REGISTRATION	0x8012			/* Parameter area for the I/O table registration	*/
#define FINS_PARAM_AREA_ROUTING_TABLE		0x8013			/* Parameter area for the routing table			*/
#define FINS_PARAM_AREA_CPU_BUS_UNIT_SETUP	0x8002			/* Parameter area for the CPU Bus unit setup		*/
									/*							*/
									/********************************************************/

									/********************************************************/
									/*							*/
#define FINS_DISK_MEMORY_CARD			0x8000			/* The disk is the flash memory card			*/
#define FINS_DISK_EM_FILE_MEMORY		0x8001			/* The disk is the file system in EM File memory	*/
									/*							*/
									/********************************************************/

									/********************************************************/
									/*							*/
#define FINS_RETVAL_ERRNO_BASE			0xC000			/* All higher error numbers are errno.h values		*/
									/*							*/
#define FINS_RETVAL_SUCCESS			0x0000			/* Execution was successful				*/
#define FINS_RETVAL_CANCELED			0x0001			/* End code 0x0001 The service was canceled		*/
									/*							*/
#define FINS_RETVAL_LOCAL_NODE_NOT_IN_NETWORK	0x0101			/* End code 0x0101 Local node is not in network		*/
#define FINS_RETVAL_LOCAL_TOKEN_TIMEOUT		0x0102			/* End code 0x0102 Local node token timeout		*/
#define FINS_RETVAL_LOCAL_RETRIES_FAILED	0x0103			/* End code 0x0103 Local node retries failed		*/
#define FINS_RETVAL_LOCAL_TOO_MANY_SEND_FRAMES	0x0104			/* End code 0x0104 Local node too many send frames	*/
#define FINS_RETVAL_LOCAL_ADDRESS_RANGE_ERROR	0x0105			/* End code 0x0105 Local node address range error	*/
#define FINS_RETVAL_LOCAL_ADDRESS_DUPLICATION	0x0106			/* End code 0x0106 Local node address duplication	*/
									/*							*/
#define FINS_RETVAL_DEST_NOT_IN_NETWORK		0x0201			/* End code 0x0201 Destination is not in network	*/
#define FINS_RETVAL_DEST_UNIT_MISSING		0x0202			/* End code 0x0202 Destination unit missing		*/
#define FINS_RETVAL_DEST_THIRD_NODE_MISSING	0x0203			/* End code 0x0203 Destination third node missing	*/
#define FINS_RETVAL_DEST_NODE_BUSY		0x0204			/* End code 0x0204 Destination node is busy		*/
#define FINS_RETVAL_DEST_TIMEOUT		0x0205			/* End code 0x0205 Destination response timed out	*/
									/*							*/
#define FINS_RETVAL_CONTR_COMM_ERROR		0x0301			/* End code 0x0301 Communications controller error	*/
#define FINS_RETVAL_CONTR_CPU_UNIT_ERROR	0x0302			/* End code 0x0302 CPU Unit error			*/
#define FINS_RETVAL_CONTR_BOARD_ERROR		0x0303			/* End code 0x0303 Controller board error		*/
#define FINS_RETVAL_CONTR_UNIT_NUMBER_ERROR	0x0304			/* End code 0x0304 Unit number error			*/
									/*							*/
#define FINS_RETVAL_UNSUPPORTED_COMMAND		0x0401			/* End code 0x0401 Undefined command			*/
#define FINS_RETVAL_UNSUPPORTED_VERSION		0x0402			/* End code 0x0402 Not supported by model/version	*/
									/*							*/
#define FINS_RETVAL_ROUTING_ADDRESS_ERROR	0x0501			/* End code 0x0501 Routing destination address error	*/
#define FINS_RETVAL_ROUTING_NO_TABLES		0x0502			/* End code 0x0502 No routing tables available		*/
#define FINS_RETVAL_ROUTING_TABLE_ERROR		0x0503			/* End code 0x0503 Routing table error			*/
#define FINS_RETVAL_ROUTING_TOO_MANY_RELAYS	0x0504			/* End code 0x0504 Too many relays			*/
									/*							*/
#define FINS_RETVAL_COMMAND_TOO_LONG		0x1001			/* End code 0x1001 Command too long			*/
#define FINS_RETVAL_COMMAND_TOO_SHORT		0x1002			/* End code 0x1002 Command too short			*/
#define FINS_RETVAL_COMMAND_ELEMENT_MISMATCH	0x1003			/* End code 0x1003 Elements/data don't match		*/
#define FINS_RETVAL_COMMAND_FORMAT_ERROR	0x1004			/* End code 0x1004 Command format error			*/
#define FINS_RETVAL_COMMAND_HEADER_ERROR	0x1005			/* End code 0x1005 Command header error			*/
									/*							*/
#define FINS_RETVAL_PARAM_AREA_MISSING		0x1101			/* End code 0x1101 Parameter area classification missing*/
#define FINS_RETVAL_PARAM_ACCESS_SIZE_ERROR	0x1102			/* End code 0x1102 Parameter access size wrong		*/
#define FINS_RETVAL_PARAM_START_ADDRESS_ERROR	0x1103			/* End code 0x1103 Start address out of range		*/
#define FINS_RETVAL_PARAM_END_ADDRESS_ERROR	0x1104			/* End code 0x1104 End address out of range		*/
#define FINS_RETVAL_PARAM_PROGRAM_MISSING	0x1106			/* End code 0x1106 Program number is missing		*/
#define FINS_RETVAL_PARAM_RELATIONAL_ERROR	0x1109			/* End code 0x1109 Parameter relational error		*/
#define FINS_RETVAL_PARAM_DUPLICATE_ACCESS	0x110A			/* End code 0x110A Duplicate data access		*/
#define FINS_RETVAL_PARAM_RESPONSE_TOO_LONG	0x110B			/* End code 0x110B Response too long			*/
#define FINS_RETVAL_PARAM_PARAMETER_ERROR	0x110C			/* End code 0x110C Parameter error			*/
									/*							*/
#define FINS_RETVAL_RD_ERR_PROTECTED		0x2002			/* End code 0x2002 The program area is protected	*/
#define FINS_RETVAL_RD_ERR_TABLE_MISSING	0x2003			/* End code 0x2003 The table is not existing		*/
#define FINS_RETVAL_RD_ERR_DATA_MISSING		0x2004			/* End code 0x2004 The search data does not exist	*/
#define FINS_RETVAL_RD_ERR_PROGRAM_MISSING	0x2005			/* End code 0x2005 The program does not exist		*/
#define FINS_RETVAL_RD_ERR_FILE_MISSING		0x2006			/* End code 0x2006 The file does not exist		*/
#define FINS_RETVAL_RD_ERR_DATA_MISMATCH	0x2007			/* End code 0x2007 Data comparison failed		*/
									/*							*/
#define FINS_RETVAL_WR_ERR_READ_ONLY		0x2101			/* End code 0x2101 The specified area is read-only	*/
#define FINS_RETVAL_WR_ERR_PROTECTED		0x2102			/* End code 0x2102 The program area is protected	*/
#define FINS_RETVAL_WR_ERR_CANNOT_REGISTER	0x2103			/* End code 0x2103 Cannot register file			*/
#define FINS_RETVAL_WR_ERR_PROGRAM_MISSING	0x2105			/* End code 0x2105 Program number is not valid		*/
#define FINS_RETVAL_WR_ERR_FILE_MISSING		0x2106			/* End code 0x2106 File does not exist			*/
#define FINS_RETVAL_WR_ERR_FILE_EXISTS		0x2107			/* End code 0x2107 The file already exists		*/
#define FINS_RETVAL_WR_ERR_CANNOT_CHANGE	0x2108			/* End code 0x2108 Cannot change the data		*/
									/*							*/
#define FINS_RETVAL_MODE_NOT_DURING_EXECUTION	0x2201			/* End code 0x2201 Not possible during execution	*/
#define FINS_RETVAL_MODE_NOT_DURING_RUN		0x2202			/* End code 0x2202 Not possible while running		*/
#define FINS_RETVAL_MODE_IS_PROGRAM		0x2203			/* End code 0x2203 Not possible in program mode		*/
#define FINS_RETVAL_MODE_IS_DEBUG		0x2204			/* End code 0x2204 Not possible in debug mode		*/
#define FINS_RETVAL_MODE_IS_MONITOR		0x2205			/* End code 0x2205 Not possible in monitor mode		*/
#define FINS_RETVAL_MODE_IS_RUN			0x2206			/* End code 0x2206 Not possible in run mode		*/
#define FINS_RETVAL_MODE_NODE_NOT_POLLING	0x2207			/* End code 0x2207 Specified node not in polling mode	*/
#define FINS_RETVAL_MODE_NO_STEP		0x2208			/* End code 0x2208 Step cannot be executed		*/
									/*							*/
#define FINS_RETVAL_DEVICE_FILE_MISSING		0x2301			/* End code 0x2301 File device missing			*/
#define FINS_RETVAL_DEVICE_MEMORY_MISSING	0x2302			/* End code 0x2302 There is no file memory		*/
#define FINS_RETVAL_DEVICE_CLOCK_MISSING	0x2303			/* End code 0x2303 There is no clock			*/
									/*							*/
#define FINS_RETVAL_DATALINK_TABLE_MISSING	0x2401			/* End code 0x2401 Data link table missing or corrupt	*/
									/*							*/
#define FINS_RETVAL_UNIT_MEMORY_CONTENT_ERROR	0x2502			/* End code 0x2502 Memory content error			*/
#define FINS_RETVAL_UNIT_IO_SETTING_ERROR	0x2503			/* End code 0x2503 I/O setting error			*/
#define FINS_RETVAL_UNIT_TOO_MANY_IO_POINTS	0x2504			/* End code 0x2504 Too many I/O points registered	*/
#define FINS_RETVAL_UNIT_CPU_BUS_ERROR		0x2505			/* End code 0x2505 CPU bus line error			*/
#define FINS_RETVAL_UNIT_IO_DUPLICATION		0x2506			/* End code 0x2506 Duplicate I/O address		*/
#define FINS_RETVAL_UNIT_IO_BUS_ERROR		0x2507			/* End code 0x2507 I/O bus error			*/
#define FINS_RETVAL_UNIT_SYSMAC_BUS2_ERROR	0x2509			/* End code 0x2509 Sysmac bus/2 error			*/
#define FINS_RETVAL_UNIT_CPU_BUS_UNIT_ERROR	0x250A			/* End code 0x250A CPU bus unit error			*/
#define FINS_RETVAL_UNIT_SYSMAC_BUS_DUPLICATION	0x250D			/* End code 0x250D Same word is used more than once	*/
#define FINS_RETVAL_UNIT_MEMORY_ERROR		0x250F			/* End code 0x250F Memory error in internal memory	*/
#define FINS_RETVAL_UNIT_SYSMAC_BUS_TERMINATOR	0x2510			/* End code 0x2510 Sysmac bus terminator missing	*/
									/*							*/
#define FINS_RETVAL_COMMAND_NO_PROTECTION	0x2601			/* End code 0x2601 The specified area is not protected	*/
#define FINS_RETVAL_COMMAND_WRONG_PASSWORD	0x2602			/* End code 0x2602 Wrong password specified		*/
#define FINS_RETVAL_COMMAND_PROTECTED		0x2604			/* End code 0x2604 The specified area is protected	*/
#define FINS_RETVAL_COMMAND_SERVICE_EXECUTING	0x2605			/* End code 0x2605 Service is already executing		*/
#define FINS_RETVAL_COMMAND_SERVICE_STOPPED	0x2606			/* End code 0x2606 The service is stopped		*/
#define FINS_RETVAL_COMMAND_NO_EXECUTION_RIGHT	0x2607			/* End code 0x2607 No execution right			*/
#define FINS_RETVAL_COMMAND_SETTINGS_INCOMPLETE	0x2608			/* End code 0x2608 The settings are not complete	*/
#define FINS_RETVAL_COMMAND_ITEMS_NOT_SET	0x2609			/* End code 0x2609 Necessary items not set		*/
#define FINS_RETVAL_COMMAND_ALREADY_DEFINED	0x260A			/* End code 0x260A Number already defined		*/
#define FINS_RETVAL_COMMAND_ERROR_WONT_CLEAR	0x260B			/* End code 0x260B Error will not clear			*/
									/*							*/
#define FINS_RETVAL_ACCESS_NO_RIGHTS		0x3001			/* End code 0x3001 No access right			*/
									/*							*/
#define FINS_RETVAL_ABORTED			0x4001			/* End code 0x4001 Service aborted			*/
									/*							*/
#define FINS_RETVAL_NOT_INITIALIZED		0x8001			/* The connection with the PLC was not initialized	*/
#define FINS_RETVAL_NOT_CONNECTED		0x8002			/* There is no connection with the remote PLC		*/
#define FINS_RETVAL_OUT_OF_MEMORY		0x8003			/* There was not enough free memory for the action	*/
#define FINS_RETVAL_SUCCESS_LAST_DATA		0x8004			/* Execution successful and last data delivered		*/
#define FINS_RETVAL_INVALID_IP_ADDRESS		0x8005			/* The IP address passed to inet_pton is invalid	*/
#define FINS_RETVAL_MAX_ERROR_COUNT		0x8006			/* The connection was closed after reaching max errors	*/
#define FINS_RETVAL_SYNC_ERROR			0x8007			/* Synchronization error. Some packets probably lost	*/
									/*							*/
#define FINS_RETVAL_NO_READ_ADDRESS		0x8101			/* No read address in the remote PLC was specified	*/
#define FINS_RETVAL_NO_WRITE_ADDRESS		0x8102			/* No write address in the remote PLC was specified	*/
#define FINS_RETVAL_NO_DATA_BLOCK		0x8103			/* No local data memory block was provided		*/
									/*							*/
#define FINS_RETVAL_INVALID_READ_ADDRESS	0x8201			/* An invalid read address string was specified		*/
#define FINS_RETVAL_INVALID_WRITE_ADDRESS	0x8202			/* An invalid write address string was specified	*/
									/*							*/
#define FINS_RETVAL_INVALID_READ_AREA		0x8301			/* No read area associated with the address		*/
#define FINS_RETVAL_INVALID_WRITE_AREA		0x8302			/* No write area associated with the address		*/
#define FINS_RETVAL_INVALID_FILL_AREA		0x8303			/* No fill area associated with the address		*/
									/*							*/
#define FINS_RETVAL_INVALID_PARAMETER_AREA	0x8401			/* The parameter area is invalid			*/
									/*							*/
#define FINS_RETVAL_INVALID_DATE		0x8501			/* The provided date is not valid			*/
									/*							*/
#define FINS_RETVAL_INVALID_DISK		0x8601			/* An invalid disk was specified			*/
#define FINS_RETVAL_INVALID_PATH		0x8602			/* An invalid path on a disk was specified		*/
#define FINS_RETVAL_INVALID_FILENAME		0x8603			/* An invalid filename was specified			*/
									/*							*/
#define FINS_RETVAL_NO_COMMAND			0x8701			/* No command specified when executing a function	*/
#define FINS_RETVAL_NO_COMMAND_LENGTH           0x8702			/* No command length specified when executing a function*/
#define FINS_RETVAL_BODY_TOO_SHORT		0x8703			/* Command body length too short			*/
#define FINS_RETVAL_BODY_TOO_LONG		0x8704			/* The FINS body is longer than allowed			*/
#define FINS_RETVAL_HEADER_SEND_ERROR		0x8705			/* Error sending complete header			*/
#define FINS_RETVAL_COMMAND_SEND_ERROR		0x8706			/* Error sending complete command			*/
#define FINS_RETVAL_RESPONSE_INCOMPLETE		0x8707			/* Response frame is shorter than expected		*/
#define FINS_RETVAL_ILLEGAL_FINS_COMMAND	0x870B			/* Illegal FINS command					*/
#define FINS_RETVAL_RESPONSE_HEADER_INCOMPLETE	0x870C			/* The received response header is incomplete		*/
#define FINS_RETVAL_INVALID_FORCE_COMMAND	0x870D			/* An invalid FORCE mode was specified			*/
									/*							*/
#define FINS_RETVAL_TRY_LATER			0x8801			/* Please try again later				*/
									/*							*/
#define FINS_RETVAL_CLOSED_BY_REMOTE		0x8900			/* TCP connection closed by remote node without error	*/
#define FINS_RETVAL_NO_FINS_HEADER		0x8901			/* First 4 characters of TCP header are not "FINS"	*/
#define FINS_RETVAL_DATA_LENGTH_TOO_LONG	0x8902			/* TCP connection data length too long			*/
#define FINS_RETVAL_COMMAND_NOT_SUPPORTED	0x8903			/* TCP connection command not supported			*/
#define FINS_RETVAL_ALL_CONNECTIONS_IN_USE	0x8904			/* All TCP connections are in use			*/
#define FINS_RETVAL_NODE_ALREADY_CONNECTED	0x8905			/* Node is already connected				*/
#define FINS_RETVAL_NODE_IP_PROTECTED		0x8906			/* IP address of client not in allowed IP adres list	*/
#define FINS_RETVAL_CLIENT_NODE_OUT_OF_RANGE	0x8907			/* TCP the client node address is out of range		*/
#define FINS_RETVAL_SAME_NODE_ADDRESS		0x8908			/* TCP client and server have the same node address	*/
#define FINS_RETVAL_NO_NODE_ADDRESS_AVAILABLE	0x8909			/* TCP connection no node address available		*/
									/*							*/
#define FINS_RETVAL_WSA_UNRECOGNIZED_ERROR	0x8A00			/* Windows WSA returned an unrecognized error code	*/
#define FINS_RETVAL_WSA_NOT_INITIALIZED		0x8A01			/* Windows WSA was not properly initialized		*/
#define FINS_RETVAL_WSA_E_NET_DOWN		0x8A02			/* Windows WSA the network subsystem or provided failed	*/
#define FINS_RETVAL_WSA_E_AF_NO_SUPPORT		0x8A03			/* Windows WSA the address familiy is not supported	*/
#define FINS_RETVAL_WSA_E_IN_PROGRESS		0x8A04			/* Windows WSA a blocking socket 1.1 call is in progres	*/
#define FINS_RETVAL_WSA_E_MFILE			0x8A05			/* Windows WSA no more socket descriptors available	*/
#define FINS_RETVAL_WSA_E_INVAL			0x8A06			/* Windows WSA Invalid argument supplied		*/
#define FINS_RETVAL_WSA_E_INVALID_PROVIDER	0x8A07			/* Windows WSA Server provider function invalid		*/
#define FINS_RETVAL_WSA_E_INVALID_PROCTABLE	0x8A08			/* Windows WSA Invalid procedure table			*/
#define FINS_RETVAL_WSA_E_NOBUFS		0x8A09			/* Windows WSA No buffer space available		*/
#define FINS_RETVAL_WSA_E_PROTO_NO_SUPPORT	0x8A0A			/* Windows WSA the protocol is not supported		*/
#define FINS_RETVAL_WSA_E_PROTO_TYPE		0x8A0B			/* Windows WSA Wrong protocol type for this socket	*/
#define FINS_RETVAL_WSA_E_PROVIDER_FAILED_INIT	0x8A0C			/* Windows WSA Provider failed initialization		*/
#define FINS_RETVAL_WSA_E_SOCKT_NO_SUPPORT	0x8A0D			/* Windows WSA The specified socket type not supported	*/
#define FINS_RETVAL_WSA_SYS_NOT_READY		0x8A0E			/* Windows WSA The network subsystem is not ready	*/
#define FINS_RETVAL_WSA_VER_NOT_SUPPORTED	0x8A0F			/* Windows WSA The socket version is not supported	*/
#define FINS_RETVAL_WSA_E_PROC_LIM		0x8A10			/* Windows WSA Process number limit reached		*/
#define FINS_RETVAL_WSA_E_FAULT			0x8A11			/* Windows WSA The parameter is not valid		*/
#define FINS_RETVAL_WSA_E_NET_RESET		0x8A12			/* Windows WSA Connection timeout during Keep Alive	*/
#define FINS_RETVAL_WSA_E_NO_PROTO_OPT		0x8A13			/* Windows WSA Unsupported option for socket		*/
#define FINS_RETVAL_WSA_E_NOT_CONN		0x8A14			/* Windows WSA Connection reset during Keep Alive	*/
#define FINS_RETVAL_WSA_E_NOT_SOCK		0x8A15			/* Windows WSA The descriptor is not a socket		*/
#define FINS_RETVAL_WSA_E_ACCES			0x8A16			/* Windows WSA Socket access violation			*/
#define FINS_RETVAL_WSA_E_ADDR_IN_USE		0x8A17			/* Windows WSA The address is already in use		*/
#define FINS_RETVAL_WSA_E_ADDR_NOT_AVAIL	0x8A18			/* Windows WSA The address is not available		*/
#define FINS_RETVAL_WSA_E_INTR			0x8A19			/* Windows WSA The blocking 1.1 call was cancelled	*/
#define FINS_RETVAL_WSA_E_ALREADY		0x8A1A			/* Windows WSA Non blocking call already in progress	*/
#define FINS_RETVAL_WSA_E_CONN_REFUSED		0x8A1B			/* Windows WSA The connection was refused		*/
#define FINS_RETVAL_WSA_E_IS_CONN		0x8A1C			/* Windows WSA Socket is already connected		*/
#define FINS_RETVAL_WSA_E_NET_UNREACH		0x8A1D			/* Windows WSA Network is unreacheable			*/
#define FINS_RETVAL_WSA_E_HOST_UNREACH		0x8A1E			/* Windows WSA Host is unreacheable			*/
#define FINS_RETVAL_WSA_E_TIMED_OUT		0x8A1F			/* Windows WSA The connection timed out			*/
#define FINS_RETVAL_WSA_E_WOULD_BLOCK		0x8A20			/* Windows WSA Non-blocking connection would block	*/
									/*							*/
									/********************************************************/
#pragma pack(push,1)
									/********************************************************/
struct fins_command_tp {						/*							*/
	unsigned char		header[FINS_HEADER_LEN];		/* FINS header with address information			*/
	unsigned char		body[FINS_BODY_LEN];			/* FINS body with command information			*/
};									/*							*/
									/********************************************************/
#pragma pack(pop)
									/********************************************************/
struct fins_area_tp {							/*							*/
	int		plc_mode;					/* CS/CJ or CV mode communication			*/
	const char *	name;						/* Text string with the area short code			*/
	int		bits;						/* Number of bits in the data				*/
	int		length;						/* Number of bytes per element				*/
	uint8_t		area;						/* Area code						*/
	uint32_t	low_id;						/* Lowest identificator					*/
	uint32_t	high_id;					/* Highest identificator				*/
	uint32_t	low_addr;					/* Lowest memory address				*/
	uint32_t	high_addr;					/* Highest memory address				*/
	int32_t		access;						/* Read or Read/Write access				*/
	bool		force;						/* Force status used 					*/
};									/*							*/
									/********************************************************/

									/********************************************************/
struct fins_mcap_tp {							/*							*/
	const char *	model;						/* CPU model						*/
	int		fins_mode;					/* FINS mode used to communicate			*/
	size_t		pa_size;					/* Program Area size in words				*/
	size_t		ex_banks;					/* Number of extended memory banks			*/
};									/*							*/
									/********************************************************/

struct fins_sys_tp {
	char		address[128];
	uint16_t	port;
	SOCKET		sockfd;
	time_t		timeout;
	int		error_count;
	int		error_max;
	int		last_error;
	bool		error_changed;
	uint8_t		local_net;
	uint8_t		local_node;
	uint8_t		local_unit;
	uint8_t		remote_net;
	uint8_t		remote_node;
	uint8_t		remote_unit;
	uint8_t		sid;
	uint8_t		comm_type;
	char		model[21];
	char		version[21];
	int		plc_mode;
};
									/********************************************************/
struct fins_datetime_tp {						/* 							*/
	int		year;						/* Year							*/
	int		month;						/* Month						*/
	int		day;						/* Day							*/
	int		hour;						/* Hour							*/
	int		min;						/* Minute						*/
	int		sec;						/* Second						*/
	int		dow;						/* Day of Week						*/
};									/*							*/
									/********************************************************/

struct fins_cycletime_tp {
	uint32_t	min;
	uint32_t	avg;
	uint32_t	max;
};

struct fins_cpustatus_tp {
	bool		message_exists[8];
	bool		running;
	bool		flash_writing;
	bool		battery_present;
	bool		standby;
	bool		fatal_memory_error;
	bool		fatal_io_bus_error;
	bool		fatal_duplication_error;
	bool		fatal_inner_board_error;
	bool		fatal_io_point_overflow;
	bool		fatal_io_setting_error;
	bool		fatal_program_error;
	bool		fatal_cycle_time_over;
	bool		fatal_fals_error;
	bool		fal_error;
	bool		duplex_error;
	bool		interrupt_task_error;
	bool		basic_io_unit_error;
	bool		plc_setup_error;
	bool		io_verification_error;
	bool		inner_board_error;
	bool		cpu_bus_unit_error;
	bool		special_io_unit_error;
	bool		sysmac_bus_error;
	bool		battery_error;
	bool		cs1_cpu_bus_unit_setting_error;
	bool		special_io_unit_setting_error;
	uint8_t		run_mode;
	uint16_t	error_code;
	char		error_message[17];
};

struct fins_cpudata_tp {
	char		model[21];
	char		version[21];
	unsigned char	system_block[40];
	int		largest_em_bank;
	int		program_area_size;
	int		iom_size;
	int		number_of_dm_words;
	int		timer_counter_size;
	int		em_non_file_memory_size;
	int		memory_card_size;
	int		num_sysmac_bus_masters;
	int		num_racks;
	uint16_t	bus_unit_id[16];
	bool		bus_unit_present[16];
	bool		dip_switch[8];
	uint8_t		memory_card_type;
};

									/********************************************************/
struct fins_unitdata_tp {						/*							*/
	char		model[21];					/* Name of the I/O unit					*/
	uint8_t		unit;						/* Unit number						*/
};									/*							*/
									/********************************************************/

struct fins_msgdata_tp {
	char		text[33];
	uint8_t		msg;
};

struct fins_nodedata_tp {
	uint8_t		network;
	uint8_t		node;
	uint8_t		unit;
};

struct fins_errordata_tp {
	uint16_t	error_code[2];
	int		year;
	int		month;
	int		day;
	int		hour;
	int		min;
	int		sec;
};

struct fins_accessdata_tp {
	uint8_t		network;
	uint8_t		node;
	uint8_t		unit;
	uint16_t	command_code;
	int		year;
	int		month;
	int		day;
	int		hour;
	int		min;
	int		sec;
};

struct fins_diskinfo_tp {
	char		volume_label[13];
	uint32_t	total_capacity;
	uint32_t	free_capacity;
	uint16_t	total_files;
	int		year;
	int		month;
	int		day;
	int		hour;
	int		min;
	int		sec;
};

									/********************************************************/
struct fins_fileinfo_tp {						/*							*/
	char		filename[13];					/* File name in MS-DOS format				*/
	uint32_t	size;						/* File size in bytes					*/
	int		year;						/* Year							*/
	int		month;						/* Month						*/
	int		day;						/* Day							*/
	int		hour;						/* Hour							*/
	int		min;						/* Minute						*/
	int		sec;						/* Second						*/
	bool		read_only;					/* Read/only attribute					*/
	bool		hidden;						/* Hidden attribute					*/
	bool		system;						/* System attribute					*/
	bool		volume_label;					/* Attribute to indicate that entry is a volume label	*/
	bool		directory;					/* Attribute to indicate that entry is a directory	*/
	bool		archive;					/* File has been modified since last backup		*/
};									/*							*/
									/********************************************************/

struct fins_address_tp {
	char		name[4];
	uint32_t	main_address;
	uint32_t	sub_address;
};

struct fins_forcebit_tp {
	char		address[12];
	uint16_t	force_command;
};

struct fins_multidata_tp {
    char		address[12];
    int			type;
    union {
	int16_t		int16;
	int32_t		int32;
	uint16_t	uint16;
	uint32_t	uint32;
	float		sfloat;
	double		dfloat;
	struct {
	    bool	bit;
	    bool	b_force;
	};
	struct {
	    uint16_t	word;
	    uint16_t	w_force;
	};
    };
};




int				finslib_access_log_read( struct fins_sys_tp *sys, struct fins_accessdata_tp *accessdata, uint16_t start_record, size_t *num_records, size_t *stored_records );
int				finslib_access_right_acquire( struct fins_sys_tp *sys, struct fins_nodedata_tp *nodedata );
int				finslib_access_right_forced_acquire( struct fins_sys_tp* sys );
int				finslib_access_right_release( struct fins_sys_tp *sys );
int				finslib_area_file_compare( struct fins_sys_tp *sys, const char *start, uint16_t disk, const char *path, const char *file, size_t *num_records );
int				finslib_area_to_file_transfer( struct fins_sys_tp *sys, const char *start, uint16_t disk, const char *path, const char *file, size_t *num_records );
int32_t				finslib_bcd_to_int( uint32_t value, int type );
int				finslib_clock_read( struct fins_sys_tp* sys, struct fins_datetime_tp *datetime );
int				finslib_clock_write( struct fins_sys_tp *sys, const struct fins_datetime_tp *datetime, bool do_sec, bool do_day_of_week );
int				finslib_connection_data_read( struct fins_sys_tp *sys, struct fins_unitdata_tp *unitdata, uint8_t start_unit, size_t *num_units );
int				finslib_cpu_unit_data_read( struct fins_sys_tp *sys, struct fins_cpudata_tp *cpudata );
int				finslib_cpu_unit_status_read( struct fins_sys_tp *sys, struct fins_cpustatus_tp *status );
int				finslib_cycle_time_init( struct fins_sys_tp *sys );
int				finslib_cycle_time_read( struct fins_sys_tp *sys, struct fins_cycletime_tp *ctime );
void				finslib_disconnect( struct fins_sys_tp* sys );
const char *			finslib_errmsg( int error_code, char *buffer, size_t buffer_len );
int				finslib_error_clear( struct fins_sys_tp *sys, uint16_t error_code );
int				finslib_error_clear_all( struct fins_sys_tp *sys );
int				finslib_error_clear_current( struct fins_sys_tp *sys );
int				finslib_error_clear_fal( struct fins_sys_tp *sys, uint16_t fal_number );
int				finslib_error_clear_fals( struct fins_sys_tp *sys, uint16_t fals_number );
int				finslib_error_log_clear( struct fins_sys_tp *sys );
int				finslib_error_log_read( struct fins_sys_tp *sys, struct fins_errordata_tp *errordata, uint16_t start_record, size_t *num_records, size_t *stored_records );
int				finslib_filename_to_83( const char *infile, char *outfile );
int				finslib_file_memory_format( struct fins_sys_tp *sys, uint16_t disk );
int				finslib_file_name_read( struct fins_sys_tp *sys, struct fins_diskinfo_tp *diskinfo, struct fins_fileinfo_tp *fileinfo, uint16_t disk, const char *path, uint16_t start_file, size_t *num_files );
int				finslib_file_read( struct fins_sys_tp *sys, uint16_t disk, const char *path, const char *filename, unsigned char *data, size_t file_position, size_t *num_bytes );
int				finslib_file_to_area_transfer( struct fins_sys_tp *sys, const char *start, uint16_t disk, const char *path, const char *file, size_t *num_records );
int				finslib_file_write( struct fins_sys_tp *sys, uint16_t disk, const char *path, const char *filename, const unsigned char *data, size_t file_position, size_t num_bytes, uint16_t open_mode );
int				finslib_forced_set_reset_cancel( struct fins_sys_tp *sys );
uint32_t			finslib_int_to_bcd( int32_t value, int type );
int				finslib_link_unit_reset( struct fins_sys_tp *sys );
int				finslib_memory_area_fill( struct fins_sys_tp *sys, const char *start, uint16_t fill_data, size_t num_word );
int				finslib_memory_area_read_bcd16( struct fins_sys_tp *sys, const char *start, uint16_t *data, size_t num_bcd16 );
int				finslib_memory_area_read_bcd32( struct fins_sys_tp *sys, const char *start, uint32_t *data, size_t num_bcd32 );
int				finslib_memory_area_read_bit( struct fins_sys_tp *sys, const char *start, bool *data, size_t num_bits );
int				finslib_memory_area_read_int16( struct fins_sys_tp *sys, const char *start, int16_t *data, size_t num_int16 );
int				finslib_memory_area_read_int32( struct fins_sys_tp *sys, const char *start, int32_t *data, size_t num_int32 );
int				finslib_memory_area_read_sbcd16( struct fins_sys_tp *sys, const char *start, int16_t *data, size_t num_sbcd16, int type );
int				finslib_memory_area_read_sbcd32( struct fins_sys_tp *sys, const char *start, int32_t *data, size_t num_sbcd32, int type );
int				finslib_memory_area_read_uint16( struct fins_sys_tp *sys, const char *start, uint16_t *data, size_t num_uint16 );
int				finslib_memory_area_read_uint32( struct fins_sys_tp *sys, const char *start, uint32_t *data, size_t num_uint32 );
int				finslib_memory_area_read_word( struct fins_sys_tp *sys, const char *start, unsigned char *data, size_t num_word );
int				finslib_memory_area_transfer( struct fins_sys_tp *sys, const char *source, const char *dest, size_t num_words );
int				finslib_memory_area_write_bcd16( struct fins_sys_tp *sys, const char *start, const uint16_t *data, size_t num_bcd16 );
int				finslib_memory_area_write_bcd32( struct fins_sys_tp *sys, const char *start, const uint32_t *data, size_t num_bcd32 );
int				finslib_memory_area_write_bit( struct fins_sys_tp *sys, const char *start, const bool *data, size_t num_bit );
int				finslib_memory_area_write_int16( struct fins_sys_tp *sys, const char *start, const int16_t *data, size_t num_int16 );
int				finslib_memory_area_write_int32( struct fins_sys_tp *sys, const char *start, const int32_t *data, size_t num_int32 );
int				finslib_memory_area_write_sbcd16( struct fins_sys_tp *sys, const char *start, const int16_t *data, size_t num_sbcd16, int type );
int				finslib_memory_area_write_sbcd32( struct fins_sys_tp *sys, const char *start, const int32_t *data, size_t num_sbcd32, int type );
int				finslib_memory_area_write_uint16( struct fins_sys_tp *sys, const char *start, const uint16_t *data, size_t num_uint16 );
int				finslib_memory_area_write_uint32( struct fins_sys_tp *sys, const char *start, const uint32_t *data, size_t num_uint32 );
int				finslib_memory_area_write_word( struct fins_sys_tp *sys, const char *start, const unsigned char *data, size_t num_word );
int				finslib_message_clear( struct fins_sys_tp *sys, uint8_t msg_mask );
int				finslib_message_read( struct fins_sys_tp *sys, struct fins_msgdata_tp *msgdata, uint8_t msg_mask );
int				finslib_message_fal_fals_read( struct fins_sys_tp *sys, char *faldata, uint16_t fal_number );
void				finslib_milli_second_sleep( int msec );
time_t				finslib_monotonic_sec_timer( void );
int				finslib_multiple_memory_area_read( struct fins_sys_tp *sys, struct fins_multidata_tp *item, size_t num_item );
int				finslib_name_delete( struct fins_sys_tp *sys );
int				finslib_name_read( struct fins_sys_tp *sys, char *name_buffer, size_t name_buffer_len );
int				finslib_name_set( struct fins_sys_tp *sys, const char *name );
int				finslib_parameter_area_clear( struct fins_sys_tp *sys, uint16_t area_code, size_t num_words );
int				finslib_parameter_area_read( struct fins_sys_tp *sys, uint16_t area_code, uint16_t *data, uint16_t start_word, size_t num_words );
int				finslib_parameter_area_write( struct fins_sys_tp *sys, uint16_t area_code, const uint16_t *data, uint16_t start_word, size_t num_words );
int				finslib_program_area_clear( struct fins_sys_tp *sys, bool do_interrupt_tasks );
int				finslib_program_area_read( struct fins_sys_tp *sys, unsigned char *data, uint32_t start_word, size_t *num_bytes );
int				finslib_program_area_write( struct fins_sys_tp *sys, const unsigned char *data, uint32_t start_word, size_t num_bytes );
int				finslib_raw( struct fins_sys_tp *sys, uint16_t command, unsigned char *buffer, size_t send_len, size_t *recv_len );
int				finslib_set_cpu_run( struct fins_sys_tp *sys, bool do_monitor );
int				finslib_set_cpu_stop( struct fins_sys_tp *sys );
int				finslib_set_plc_name( struct fins_sys_tp *sys, const char *name );
struct fins_sys_tp *		finslib_tcp_connect( struct fins_sys_tp *sys, const char *address, uint16_t port, uint8_t local_net, uint8_t local_node, uint8_t local_unit, uint8_t remote_net, uint8_t remote_node, uint8_t remote_unit, int *error_val, int error_max );
struct fins_sys_tp *		finslib_udp_connect( struct fins_sys_tp *sys, const char *address, uint16_t port, uint8_t local_net, uint8_t local_node, uint8_t local_unit, uint8_t remote_net, uint8_t remote_node, uint8_t remote_unit, int *error_val, int error_max );
bool				finslib_valid_directory( const char *path );
bool				finslib_valid_filename( const char *filename );
int				finslib_write_access_log_clear( struct fins_sys_tp *sys );
int				XX_finslib_communicate( struct fins_sys_tp *sys, struct fins_command_tp *command, size_t *bodylen, bool wait_response );
bool				XX_finslib_decode_address( const char *str, struct fins_address_tp *address );
void				XX_finslib_init_command( struct fins_sys_tp *sys, struct fins_command_tp *command, uint8_t mrc, uint8_t src );
const struct fins_area_tp *	XX_finslib_search_area( struct fins_sys_tp *sys, const struct fins_address_tp *address, int bits, uint32_t access, bool force );
int				XX_finslib_wsa_errorcode_to_fins_retval( int errorcode );



extern struct fins_mcap_tp	fins_model[];

#endif
