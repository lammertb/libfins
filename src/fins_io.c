/*
 * Library: libfins
 * File:    src/fins_io.c
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
 * The source file src/fins_io.c contains the low level routines to communicate
 * with devices over the Omron FINS protocol.
 */

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#if defined(_WIN32)
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#if defined(__FreeBSD__)
#include <sys/timespec.h>
#endif

#include <signal.h>
#include "fins.h"

#define BUFLEN		1024
#define SEND_TIMEOUT	10
#define RECV_TIMEOUT	10

#if defined(_WIN32)
typedef const char	send_tp;
typedef const char	sendto_tp;
typedef const char	setsockopt_tp;
#else
typedef void		send_tp;
typedef void		sendto_tp;
typedef void		setsockopt_tp;
#endif

static int			check_error_count( struct fins_sys_tp *sys, int error_code );
static void			init_system( struct fins_sys_tp *sys, int error_max );
static struct fins_sys_tp *	fins_close_socket( struct fins_sys_tp *sys );
static struct fins_sys_tp *	fins_close_socket_with_error( struct fins_sys_tp *sys, int *error_val );
static int			fins_recv_tcp_command( struct fins_sys_tp *sys, int total_len, struct fins_command_tp *command );
static int			fins_recv_tcp_header( struct fins_sys_tp *sys, int *error_val );
static int			fins_send_tcp_command( struct fins_sys_tp *sys, size_t bodylen, struct fins_command_tp *command );
static int			fins_send_tcp_header( struct fins_sys_tp *sys, size_t bodylen );
static int			fins_tcp_recv( struct fins_sys_tp *sys, unsigned char *buf, int len );
static int			tcp_errorcode_to_fins_retval( uint32_t errorcode );

/*
 * static void init_system( fins_sys_tp *sysm int error_max );
 *
 * The function init_system() clears a PLC structure to make it useable
 * for a new connection.
 */

static void init_system( struct fins_sys_tp *sys, int error_max ) {

	time_t timeout_val;

	timeout_val = finslib_monotonic_sec_timer() - 2*FINS_TIMEOUT;
	if ( finslib_monotonic_sec_timer() > timeout_val ) timeout_val = 0;

	sys->address[0]    = 0;
	sys->port          = FINS_DEFAULT_PORT;
	sys->sockfd        = INVALID_SOCKET;
	sys->timeout       = timeout_val;
	sys->plc_mode      = FINS_MODE_UNKNOWN;
	sys->model[0]      = 0;
	sys->version[0]    = 0;
	sys->sid           = 0;
	sys->comm_type     = FINS_COMM_TYPE_UNKNOWN;
	sys->local_net     = 0;
	sys->local_node    = 0;
	sys->local_unit    = 0;
	sys->remote_net    = 0;
	sys->remote_node   = 0;
	sys->remote_unit   = 0;
	sys->error_count   = 0;
	sys->error_max     = error_max;
	sys->last_error    = FINS_RETVAL_SUCCESS;
	sys->error_changed = false;

}  /* init_system */

/*
 * int finsliv_tcp_connect( const char *address, int port );
 *
 * The function finslib_tcp_connect() tries to connect over FINS with the
 * remote FINS TCP server. If the connection succeeds, a pointer to a system
 * structure containing the data of the connection. Otherwise the return value
 * is NULL.
 *
 * The function gets a system structure pointer as parameter. If this parameter
 * is not NULL, the contents of that structure will be reused, instead of
 * allocating a new one. If an error occurs, the error value is returned in a
 * variable who's address is passed as a pointer.
 */

struct fins_sys_tp *finslib_tcp_connect( struct fins_sys_tp *sys, const char *address, uint16_t port, uint8_t local_net, uint8_t local_node, uint8_t local_unit, uint8_t remote_net, uint8_t remote_node, uint8_t remote_unit, int *error_val, int error_max ) {

	int sendlen;
	int recvlen;
	int retval;
	int keep_alive;
	int new_error;
	uint32_t command;
	uint32_t errorcode;
	struct sockaddr_in ws_addr;
	struct sockaddr_in cs_addr;
	struct timeval tv;
	unsigned char fins_tcp_header[FINS_MAX_TCP_HEADER];

	if ( sys != NULL  &&  finslib_monotonic_sec_timer() < sys->timeout + FINS_TIMEOUT  &&  sys->timeout > 0 ) {

		if ( error_val != NULL ) *error_val = FINS_RETVAL_TRY_LATER;

		return sys;
	}

	if ( sys == NULL ) {

		if ( port < FINS_PORT_RESERVED  ||  port >= FINS_PORT_MAX ) port = FINS_DEFAULT_PORT;

		if ( address == NULL  ||  address[0] == 0 ) {

			if ( error_val != NULL ) *error_val = FINS_RETVAL_NO_READ_ADDRESS;
			return NULL;
		}

		sys = malloc( sizeof(struct fins_sys_tp) );

		if ( sys == NULL ) {

			if ( error_val != NULL ) *error_val = FINS_RETVAL_OUT_OF_MEMORY;
			return NULL;
		}

		init_system( sys, error_max );

		sys->comm_type   = FINS_COMM_TYPE_TCP;
		sys->port        = port;
		sys->local_net   = local_net;
		sys->local_node  = local_node;
		sys->local_unit  = local_unit;
		sys->remote_net  = remote_net;
		sys->remote_node = remote_node;
		sys->remote_unit = remote_unit;

		snprintf( sys->address, 128, "%s", address );
	}

	sys->sockfd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	if ( sys->sockfd == INVALID_SOCKET ) return fins_close_socket_with_error( sys, error_val );



	keep_alive = true;

	if ( setsockopt( sys->sockfd, SOL_SOCKET, SO_KEEPALIVE, (setsockopt_tp *) & keep_alive, sizeof(keep_alive) ) < 0 ) return fins_close_socket_with_error( sys, error_val );



	tv.tv_sec  = SEND_TIMEOUT;
	tv.tv_usec = 0;

	if ( setsockopt( sys->sockfd, SOL_SOCKET, SO_SNDTIMEO, (setsockopt_tp *) & tv, sizeof(tv) ) < 0 ) return fins_close_socket_with_error( sys, error_val );



	tv.tv_sec  = RECV_TIMEOUT;
	tv.tv_usec = 0;

	if ( setsockopt( sys->sockfd, SOL_SOCKET, SO_RCVTIMEO, (setsockopt_tp *) & tv, sizeof(tv) ) < 0 ) return fins_close_socket_with_error( sys, error_val );



	memset( & ws_addr, 0, sizeof(ws_addr) );

	ws_addr.sin_family      = AF_INET;
	ws_addr.sin_addr.s_addr = htonl( INADDR_ANY );
	ws_addr.sin_port        = htons( 0 );

	if ( bind( sys->sockfd, (struct sockaddr *) &ws_addr, sizeof(ws_addr) ) < 0 ) return fins_close_socket_with_error( sys, error_val );


	memset( & cs_addr, 0, sizeof(cs_addr) );

	cs_addr.sin_family      = AF_INET;
	cs_addr.sin_port        = htons( port );

	retval = inet_pton( AF_INET, address, & cs_addr.sin_addr.s_addr );

	if      ( retval <  0 ) return fins_close_socket_with_error( sys, error_val );
	else if ( retval == 0 ) {

		sys->error_changed = ( FINS_RETVAL_INVALID_IP_ADDRESS != sys->last_error );
		sys->last_error    =   FINS_RETVAL_INVALID_IP_ADDRESS;

		if ( error_val != NULL ) *error_val = sys->last_error;

		return fins_close_socket( sys );
	}

	if ( connect( sys->sockfd, (struct sockaddr *) &cs_addr, sizeof(cs_addr) ) < 0 ) return fins_close_socket_with_error( sys, error_val );

						/****************************************/
						/*					*/
	fins_tcp_header[0]  = 'F';		/* Header				*/
	fins_tcp_header[1]  = 'I';		/*					*/
	fins_tcp_header[2]  = 'N';		/*					*/
	fins_tcp_header[3]  = 'S';		/*					*/
						/*					*/
	fins_tcp_header[4]  = 0x00;		/* Length				*/
	fins_tcp_header[5]  = 0x00;		/*					*/
	fins_tcp_header[6]  = 0x00;		/*					*/
	fins_tcp_header[7]  = 8 + 4;		/*					*/
						/*					*/
	fins_tcp_header[8]  = 0x00;		/* Command				*/
	fins_tcp_header[9]  = 0x00;		/*					*/
	fins_tcp_header[10] = 0x00;		/*					*/
	fins_tcp_header[11] = 0x00;		/*					*/
						/*					*/
	fins_tcp_header[12] = 0x00;		/* Error Code				*/
	fins_tcp_header[13] = 0x00;		/*					*/
	fins_tcp_header[14] = 0x00;		/*					*/
	fins_tcp_header[15] = 0x00;		/*					*/
						/*					*/
	fins_tcp_header[16] = 0x00;		/* Client node add			*/
	fins_tcp_header[17] = 0x00;		/*					*/
	fins_tcp_header[18] = 0x00;		/*					*/
	fins_tcp_header[19] = 0x00;		/* Get node number automatically	*/
						/*					*/
						/****************************************/
	sendlen = 20;

	if ( send( sys->sockfd, fins_tcp_header, sendlen, 0 ) != sendlen ) {

		sys->error_changed = ( FINS_RETVAL_HEADER_SEND_ERROR != sys->last_error );
		sys->last_error    =   FINS_RETVAL_HEADER_SEND_ERROR;

		if ( error_val != NULL ) *error_val = sys->last_error;

		return fins_close_socket( sys );
	}

	sys->timeout = finslib_monotonic_sec_timer();

	recvlen = 24;

	if ( fins_tcp_recv( sys, fins_tcp_header, recvlen ) < recvlen ) {

		sys->error_changed = ( FINS_RETVAL_RESPONSE_HEADER_INCOMPLETE != sys->last_error );
		sys->last_error    =   FINS_RETVAL_RESPONSE_HEADER_INCOMPLETE;

		if ( error_val != NULL ) *error_val = sys->last_error;

		return fins_close_socket( sys );
	}

	command     = fins_tcp_header[8];
	command   <<= 8;
	command    += fins_tcp_header[9];
	command   <<= 8;
	command    += fins_tcp_header[10];
	command   <<= 8;
	command    += fins_tcp_header[11];

	errorcode   = fins_tcp_header[12];
	errorcode <<= 8;
	errorcode  += fins_tcp_header[13];
	errorcode <<= 8;
	errorcode  += fins_tcp_header[14];
	errorcode <<= 8;
	errorcode  += fins_tcp_header[15];

	if ( command != 0x00000001 ) {

		new_error          = tcp_errorcode_to_fins_retval( errorcode );
		sys->error_changed = ( new_error != sys->last_error );
		sys->last_error    = new_error;

		if ( error_val != NULL ) *error_val = sys->last_error;

		return fins_close_socket( sys );
	}

	sys->local_node    = fins_tcp_header[19];
	sys->remote_node   = fins_tcp_header[23];

	sys->error_changed = ( FINS_RETVAL_SUCCESS != sys->last_error );
	sys->last_error    =   FINS_RETVAL_SUCCESS;

	if ( error_val != NULL ) *error_val = sys->last_error;

	return sys;

}  /* finslib_tcp_connect */

/*
 * void finslib_disconnect( fins_sys_tp *sys );
 *
 * The function finslib_disconnect() disconnects a FINS client connection
 * and frees the memory associated with it. The function will only return if
 * the action succeeded. Otherwise it may hang indefinitely.
 */

void finslib_disconnect( struct fins_sys_tp *sys ) {

	if ( sys == NULL ) return;

	fins_close_socket( sys );
	free( sys );

}  /* finslib_disconnect */

/*
 * static fins_sys_tp *fins_close_socket_with_error( fins_sys_tp *sys, int *error_val );
 *
 * The function fins_close_socket_with_error() closes the FINS socket and sets
 * an error value based on the error returned by the previous operation.
 */

static struct fins_sys_tp *fins_close_socket_with_error( struct fins_sys_tp *sys, int *error_val ) {

	int error_code;

#if defined(_WIN32)
	error_code = XX_finslib_wsa_errorcode_to_fins_retval( WSAGetLastError() );
#else
	error_code = FINS_RETVAL_ERRNO_BASE + errno;
#endif

	sys->error_changed = ( error_code != sys->last_error );
	sys->last_error    =   error_code;

	if ( error_val != NULL ) *error_val = error_code;

	return fins_close_socket( sys );

}  /* fins_close_socket_with_error */

/*
 * static fins_sys_tp *fins_close_socket( fins_sys_tp *sys );
 *
 * The function fins_close_socket() closes the fins socket for a client TCP
 * conection. It first sets the timeouts for reading and sending to zero and
 * stops lingering, because otherwise stopping the socket may take an
 * indefinite amount of time. It also resets the error counter and the global
 * timeout value for retries. The pointer returns a pointer to the system
 * structure, or NULL when an error occured.
 */

static struct fins_sys_tp *fins_close_socket( struct fins_sys_tp *sys ) {

	struct timeval tv;
	struct linger li;

	if ( sys == NULL ) return NULL;

	if ( sys->sockfd != INVALID_SOCKET ) {

		if ( sys->comm_type == FINS_COMM_TYPE_TCP ) {

			li.l_onoff  = false;
			li.l_linger = 0;

			setsockopt( sys->sockfd, SOL_SOCKET, SO_LINGER, (setsockopt_tp *) & li, sizeof(li) );

			tv.tv_sec  = 0;
			tv.tv_usec = 0;

			setsockopt( sys->sockfd, SOL_SOCKET, SO_SNDTIMEO, (setsockopt_tp *) & tv, sizeof(tv) );

			tv.tv_sec  = 0;
			tv.tv_usec = 0;

			setsockopt( sys->sockfd, SOL_SOCKET, SO_RCVTIMEO, (setsockopt_tp *) & tv, sizeof(tv) );
		}

		closesocket( sys->sockfd );
	}

	sys->error_count = 0;
	sys->sockfd      = INVALID_SOCKET;
	sys->timeout     = finslib_monotonic_sec_timer();

	return sys;

}  /* fins_close_socket */

/*
 * static int fins_tcp_recv( int index, uint8_t *buf, size_t len );
 *
 * The function tcp_recv() receives information from the remotely connected
 * PLC which is sent over the network with the FINS protocol. As long as new
 * information is coming in, the data is appended to a buffer and receiving
 * data continues.
 */

static int fins_tcp_recv( struct fins_sys_tp *sys, unsigned char *buf, int len ) {

	int total_len;
	int recv_len;

	if ( len <= 0 ) return 0;

	total_len = 0;

	for (;;) {

		recv_len = recv( sys->sockfd, buf, len, 0 );

		if ( recv_len > 0 ) {

			len       -= recv_len;
			buf       += recv_len;
			total_len += recv_len;

			if ( len <= 0 ) break;
		}

		else if ( recv_len < 0 ) {

			if ( errno == EAGAIN ) {

				finslib_milli_second_sleep( 10 );
				continue;
			}
		}

		else return total_len;
	}

	return total_len;

}  /* fins_tcp_recv */

/*
 * static int fins_send_tcp_header( fins_sys_tp *sys, size_t bodylen );
 *
 * The function fins_send_tcp_header() sends a standard FINS TCP header and
 * waits for completement.
 */

static int fins_send_tcp_header( struct fins_sys_tp *sys, size_t bodylen ) {

	int sendlen;
	unsigned char fins_tcp_header[FINS_MAX_TCP_HEADER];

	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;
	if ( bodylen     >  FINS_BODY_LEN  ) return FINS_RETVAL_BODY_TOO_LONG;

	bodylen += 8 + FINS_HEADER_LEN;

	fins_tcp_header[0]  = 'F';
	fins_tcp_header[1]  = 'I';
	fins_tcp_header[2]  = 'N';
	fins_tcp_header[3]  = 'S';

	fins_tcp_header[4]  = (bodylen >> 24) & 0xff;
	fins_tcp_header[5]  = (bodylen >> 16) & 0xff;
	fins_tcp_header[6]  = (bodylen >>  8) & 0xff;
	fins_tcp_header[7]  = (bodylen      ) & 0xff;

	fins_tcp_header[8]  = 0x00;
	fins_tcp_header[9]  = 0x00;
	fins_tcp_header[10] = 0x00;
	fins_tcp_header[11] = 0x02;

	fins_tcp_header[12] = 0x00;
	fins_tcp_header[13] = 0x00;
	fins_tcp_header[14] = 0x00;
	fins_tcp_header[15] = 0x00;

	sendlen = 16;

	if ( send( sys->sockfd, fins_tcp_header, sendlen, 0 ) != sendlen ) return FINS_RETVAL_HEADER_SEND_ERROR;

	return FINS_RETVAL_SUCCESS;

}  /* fins_send_tcp_header */

/*
 * static int check_error_count( struct fins_sys_tp *sys, int error_code );
 *
 * The function check_error_count() checks an errorcode and the current error
 * count on a connection. If the errorcode indicates success, the counter is
 * reset to 0. Otherwise if the counter reached the maximum error counts, the
 * counter is reset and the connection is closed. In that case the function
 * returns the maximum error count error. Otherwise the error indicated as the
 * parameter.
 */

static int check_error_count( struct fins_sys_tp *sys, int error_code ) {

	if ( sys == NULL ) return FINS_RETVAL_NOT_INITIALIZED;

	if ( sys->sockfd    == INVALID_SOCKET                ||
	     sys->error_max <  0                             ||
	     error_code     == FINS_RETVAL_SUCCESS           ||
	     error_code     == FINS_RETVAL_SUCCESS_LAST_DATA      ) {

		sys->error_count   = 0;
		sys->error_changed = ( error_code != sys->last_error );
		sys->last_error    =   error_code;

		return error_code;
	}

	sys->error_count++;

	if ( sys->error_count > sys->error_max ) error_code = FINS_RETVAL_MAX_ERROR_COUNT;

	switch ( error_code ) {

		case FINS_RETVAL_MAX_ERROR_COUNT            :
		case FINS_RETVAL_CLOSED_BY_REMOTE           :
		case FINS_RETVAL_WSA_UNRECOGNIZED_ERROR     :
		case FINS_RETVAL_WSA_NOT_INITIALIZED        :
		case FINS_RETVAL_WSA_SYS_NOT_READY          :
		case FINS_RETVAL_WSA_VER_NOT_SUPPORTED      :
		case FINS_RETVAL_WSA_E_ACCES                :
		case FINS_RETVAL_WSA_E_ADDR_IN_USE          :
		case FINS_RETVAL_WSA_E_ADDR_NOT_AVAIL       :
		case FINS_RETVAL_WSA_E_AF_NO_SUPPORT        :
		case FINS_RETVAL_WSA_E_CONN_REFUSED         :
		case FINS_RETVAL_WSA_E_HOST_UNREACH         :
		case FINS_RETVAL_WSA_E_MFILE                :
		case FINS_RETVAL_WSA_E_NET_DOWN             :
		case FINS_RETVAL_WSA_E_NET_RESET            :
		case FINS_RETVAL_WSA_E_NET_UNREACH          :
		case FINS_RETVAL_WSA_E_NO_PROTO_OPT         :
		case FINS_RETVAL_WSA_E_NOT_CONN             :
		case FINS_RETVAL_WSA_E_NOT_SOCK             :
		case FINS_RETVAL_WSA_E_PROC_LIM             :
		case FINS_RETVAL_WSA_E_PROTO_NO_SUPPORT     :
		case FINS_RETVAL_WSA_E_PROTO_TYPE           :
		case FINS_RETVAL_WSA_E_PROVIDER_FAILED_INIT :
		case FINS_RETVAL_WSA_E_SOCKT_NO_SUPPORT     :

			sys->error_count   = 0;
			sys->error_changed = ( error_code != sys->last_error );
			sys->last_error    =   error_code;

			fins_close_socket( sys );

			break;
	}

	return error_code;

}  /* check_error_count */

/*
 * static int fins_send_tcp_command( fins_sys_tp *sys, size_t bodylen, fins_command_tp *command );
 *
 * The fins_send_tcp_command() function sends the command header and body to
 * the remote PLC over a TCP connection. The TCP header should already have
 * been send before.
 */

static int fins_send_tcp_command( struct fins_sys_tp *sys, size_t bodylen, struct fins_command_tp *command ) {

	int sendlen;
	int retval;

	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( command     == NULL           ) return FINS_RETVAL_NO_COMMAND;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;
	if ( bodylen     >  FINS_BODY_LEN  ) return FINS_RETVAL_BODY_TOO_LONG;

	sendlen = FINS_HEADER_LEN + (int) bodylen;
	retval  = send( sys->sockfd, (send_tp *) command, sendlen, 0 );

	if ( retval <  0       ) return FINS_RETVAL_ERRNO_BASE + errno;
	if ( retval != sendlen ) return FINS_RETVAL_COMMAND_SEND_ERROR;

	return FINS_RETVAL_SUCCESS;

}  /* fins_send_tcp_command */

/*
 * static int tcp_errorcode_to_fins_retval( uint32_t errorcode );
 *
 * The function tcp_errorcode_to_fins_retval() converts a FINS/TCP header
 * errorcode to a FINS_RETVAL which is recognized by calling routines.
 */

static int tcp_errorcode_to_fins_retval( uint32_t errorcode ) {

	switch ( errorcode ) {

		case 0x00000000 : return FINS_RETVAL_CLOSED_BY_REMOTE;
		case 0x00000001 : return FINS_RETVAL_NO_FINS_HEADER;
		case 0x00000002 : return FINS_RETVAL_DATA_LENGTH_TOO_LONG;
		case 0x00000003 : return FINS_RETVAL_COMMAND_NOT_SUPPORTED;
		case 0x00000020 : return FINS_RETVAL_ALL_CONNECTIONS_IN_USE;
		case 0x00000021 : return FINS_RETVAL_NODE_ALREADY_CONNECTED;
		case 0x00000022 : return FINS_RETVAL_NODE_IP_PROTECTED;
		case 0x00000023 : return FINS_RETVAL_CLIENT_NODE_OUT_OF_RANGE;
		case 0x00000024 : return FINS_RETVAL_SAME_NODE_ADDRESS;
		case 0x00000025 : return FINS_RETVAL_NO_NODE_ADDRESS_AVAILABLE;
	}

	return FINS_RETVAL_ILLEGAL_FINS_COMMAND;

}  /* tcp_errorcode_to_fins_retval */

/*
 * int fins_recv_tcp_header( fins_sys_tp *sys, int *error_val );
 *
 * The function fins_recv_tcp_header() receives a TCP response header from a
 * remote PLC. The function checks the integrity of the returned header. If
 * an error occurs, -1 is returned. Otherwise the return value is the size
 * of the following payload. Note that in some situations the payload length
 * can be 0, which is not an error.
 */

static int fins_recv_tcp_header( struct fins_sys_tp *sys, int *error_val ) {

	int recvlen;
	int retval;
	uint32_t command;
	uint32_t errorcode;
	unsigned char fins_tcp_header[FINS_MAX_TCP_HEADER];

	if ( sys == NULL  ||  sys->sockfd == INVALID_SOCKET ) return -1;

	recvlen = 16;
	retval  = fins_tcp_recv( sys, fins_tcp_header, recvlen );

	if ( retval < recvlen ) {

		if ( error_val != NULL ) *error_val = FINS_RETVAL_RESPONSE_HEADER_INCOMPLETE;
		return -1;
	}

	command     = fins_tcp_header[8];
	command   <<= 8;
	command    += fins_tcp_header[9];
	command   <<= 8;
	command    += fins_tcp_header[10];
	command   <<= 8;
	command    += fins_tcp_header[11];

	errorcode   = fins_tcp_header[12];
	errorcode <<= 8;
	errorcode  += fins_tcp_header[13];
	errorcode <<= 8;
	errorcode  += fins_tcp_header[14];
	errorcode <<= 8;
	errorcode  += fins_tcp_header[15];

	if ( command != 0x00000002 ) {

		if ( error_val != NULL ) *error_val = tcp_errorcode_to_fins_retval( errorcode );
		return -1;
	}

	recvlen   = fins_tcp_header[6];
	recvlen <<= 8;
	recvlen  += fins_tcp_header[7];
	recvlen  -= 8;

	if ( recvlen > FINS_HEADER_LEN + FINS_BODY_LEN ) {

		if ( error_val != NULL ) *error_val = FINS_RETVAL_BODY_TOO_LONG;
		return -1;
	}

	if ( error_val != NULL ) *error_val = FINS_RETVAL_SUCCESS;

	return recvlen;

}  /* fins_recv_tcp_header */

/*
 * static int fins_recv_tcp_command( fins_sys_tp *sys, int total_len, fins_command_tp *command );
 *
 * The function fins_recv_tcp_command() receives a command structure from the
 * remote PLC. The total number of bytes is provided as a parameter. This total
 * number includes the complete header and some of the body.
 */

static int fins_recv_tcp_command( struct fins_sys_tp *sys, int total_len, struct fins_command_tp *command ) {

	if ( fins_tcp_recv( sys, (unsigned char *)command, total_len ) != total_len ) return FINS_RETVAL_RESPONSE_INCOMPLETE;

	return FINS_RETVAL_SUCCESS;

}  /* fins_recv_tcp_command */

/*
 * int XX_finslib_communicate( fins_sys_tp *sys, fins_command_tp *command, size_t *bodylen );
 *
 * The function XX_finslib_communicate() is the function used by outside
 * routines to perform the actual communication with a FINS server. The
 * function both sends the command and receives the response and hides all the
 * details of the low level communication for the calling routine.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int XX_finslib_communicate( struct fins_sys_tp *sys, struct fins_command_tp *command, size_t *bodylen ) {

	int a;
	int recvlen;
	int retval;
	int error_val;
	uint16_t endcode;
	unsigned char sent_header[FINS_HEADER_LEN];
	unsigned char waste_buffer[BUFLEN];

	if ( sys         == NULL           ) return check_error_count( sys, FINS_RETVAL_NOT_INITIALIZED   );
	if ( command     == NULL           ) return check_error_count( sys, FINS_RETVAL_NO_COMMAND        );
	if ( bodylen     == NULL           ) return check_error_count( sys, FINS_RETVAL_NO_COMMAND_LENGTH );
	if ( sys->sockfd == INVALID_SOCKET ) return check_error_count( sys, FINS_RETVAL_NOT_CONNECTED     );

	if ( sys->comm_type == FINS_COMM_TYPE_TCP ) {

		error_val = FINS_RETVAL_SUCCESS;

		for (a=0; a<FINS_HEADER_LEN; a++) sent_header[a] = command->header[a];

		if ( ( retval = fins_send_tcp_header(  sys, *bodylen          ) ) != FINS_RETVAL_SUCCESS ) return check_error_count( sys, retval );
		if ( ( retval = fins_send_tcp_command( sys, *bodylen, command ) ) != FINS_RETVAL_SUCCESS ) return check_error_count( sys, retval );

		recvlen = fins_recv_tcp_header( sys, & error_val );

		if ( recvlen <  0 ) return check_error_count( sys, error_val                  );
		if ( recvlen == 0 ) return check_error_count( sys, FINS_RETVAL_BODY_TOO_SHORT );

		if ( ( retval = fins_recv_tcp_command( sys, recvlen, command ) ) != FINS_RETVAL_SUCCESS ) return check_error_count( sys, retval );

		if ( command->header[FINS_ICF]  !=  (sent_header[FINS_ICF] | 0x40)  ||
		     command->header[FINS_RSV]  !=                           0x00   ||
		     command->header[FINS_DNA]  !=   sent_header[FINS_SNA]          ||
		     command->header[FINS_DA1]  !=   sent_header[FINS_SA1]          ||
		     command->header[FINS_DA2]  !=   sent_header[FINS_SA2]          ||
		     command->header[FINS_SNA]  !=   sent_header[FINS_DNA]          ||
		     command->header[FINS_SA1]  !=   sent_header[FINS_DA1]          ||
		     command->header[FINS_SA2]  !=   sent_header[FINS_DA2]          ||
		     command->header[FINS_SID]  !=   sent_header[FINS_SID]          ||
		     command->header[FINS_MRC]  !=   sent_header[FINS_MRC]          ||
		     command->header[FINS_SRC]  !=   sent_header[FINS_SRC]              ) {

			while ( fins_tcp_recv( sys, waste_buffer, BUFLEN ) > 0 ) {};

			return check_error_count( sys, FINS_RETVAL_SYNC_ERROR );
		}

		recvlen -= FINS_HEADER_LEN;
		*bodylen = recvlen;

		if ( recvlen < 2 ) return check_error_count( sys, FINS_RETVAL_BODY_TOO_SHORT );

		endcode   = command->body[0] & 0x7f;
		endcode <<= 8;
		endcode  += command->body[1] & 0x3f;

		return check_error_count( sys, endcode );
	}

	return check_error_count( sys, FINS_RETVAL_NOT_INITIALIZED );

}  /* XX_finslib_communicate */

/*
 * int XX_finslib_wsa_errorcode_to_fins_retval( int errorcode );
 *
 * The function XX_finslib_wsa_errorcode_to_fins_retval() converts a Windowss
 * WSA error code to a FINS_RETVAL_ value.
 */

int XX_finslib_wsa_errorcode_to_fins_retval( int errorcode ) {

	switch ( errorcode ) {

#if defined(_WIN32)
		case WSASYSNOTREADY         : return FINS_RETVAL_WSA_SYS_NOT_READY;
		case WSAVERNOTSUPPORTED     : return FINS_RETVAL_WSA_VER_NOT_SUPPORTED;
		case WSANOTINITIALISED      : return FINS_RETVAL_WSA_NOT_INITIALIZED;

		case WSAEACCES              : return FINS_RETVAL_WSA_E_ACCES;
		case WSAEADDRINUSE          : return FINS_RETVAL_WSA_E_ADDR_IN_USE;
		case WSAEADDRNOTAVAIL       : return FINS_RETVAL_WSA_E_ADDR_NOT_AVAIL;
		case WSAEAFNOSUPPORT        : return FINS_RETVAL_WSA_E_AF_NO_SUPPORT;
		case WSAEALREADY            : return FINS_RETVAL_WSA_E_ALREADY;
		case WSAECONNREFUSED        : return FINS_RETVAL_WSA_E_CONN_REFUSED;
		case WSAEFAULT              : return FINS_RETVAL_WSA_E_FAULT;
		case WSAEHOSTUNREACH        : return FINS_RETVAL_WSA_E_HOST_UNREACH;
		case WSAEINPROGRESS         : return FINS_RETVAL_WSA_E_IN_PROGRESS;
		case WSAEINTR               : return FINS_RETVAL_WSA_E_INTR;
		case WSAEINVAL              : return FINS_RETVAL_WSA_E_INVAL;
		case WSAEINVALIDPROCTABLE   : return FINS_RETVAL_WSA_E_INVALID_PROCTABLE;
		case WSAEINVALIDPROVIDER    : return FINS_RETVAL_WSA_E_INVALID_PROVIDER;
		case WSAEISCONN             : return FINS_RETVAL_WSA_E_IS_CONN;
		case WSAEMFILE              : return FINS_RETVAL_WSA_E_MFILE;
		case WSAENETDOWN            : return FINS_RETVAL_WSA_E_NET_DOWN;
		case WSAENETRESET           : return FINS_RETVAL_WSA_E_NET_RESET;
		case WSAENETUNREACH         : return FINS_RETVAL_WSA_E_NET_UNREACH;
		case WSAENOBUFS             : return FINS_RETVAL_WSA_E_NOBUFS;
		case WSAENOPROTOOPT         : return FINS_RETVAL_WSA_E_NO_PROTO_OPT;
		case WSAENOTCONN            : return FINS_RETVAL_WSA_E_NOT_CONN;
		case WSAENOTSOCK            : return FINS_RETVAL_WSA_E_NOT_SOCK;
		case WSAEPROCLIM            : return FINS_RETVAL_WSA_E_PROC_LIM;
		case WSAEPROTONOSUPPORT     : return FINS_RETVAL_WSA_E_PROTO_NO_SUPPORT;
		case WSAEPROTOTYPE          : return FINS_RETVAL_WSA_E_PROTO_TYPE;
		case WSAEPROVIDERFAILEDINIT : return FINS_RETVAL_WSA_E_PROVIDER_FAILED_INIT;
		case WSAESOCKTNOSUPPORT     : return FINS_RETVAL_WSA_E_SOCKT_NO_SUPPORT;
		case WSAETIMEDOUT           : return FINS_RETVAL_WSA_E_TIMED_OUT;
		case WSAEWOULDBLOCK         : return FINS_RETVAL_WSA_E_WOULD_BLOCK;
#endif
		default                     : return FINS_RETVAL_WSA_UNRECOGNIZED_ERROR;
	}

}  /* XX_finslib_wsa_errorcode_to_fins_retval */
