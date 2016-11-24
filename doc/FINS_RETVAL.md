# Finslib API Reference

## Constants

### Function return values

|Name|Description|
|:---|:---|
|**`FINS_RETVAL_SUCCESS`**|The function executed successfully|
|**`FINS_RETVAL_SUCCESS_LAST_DATA`**|The function executed successfully and the last available data is returned|
|**`FINS_RETVAL_CANCELED`**|The execution of the function was cancelled|
|**`FINS_RETVAL_ABORTED`**|The execution of the function was aborted|
|**`FINS_RETVAL_MAX_ERROR_COUNT`**|The maximum allowed error count was reached and the connection is closed|
|**`FINS_RETVAL_SYNC_ERROR`**|A synchronization error occured|
|**`FINS_RETVAL_CLOSED_BY_REMOTE`**|The connection was closed by the remote peer|
|**`FINS_RETVAL_NO_FINS_HEADER`**|The request or response packet had an invalid FINS header|
|**`FINS_RETVAL_DATA_LENGTH_TOO_LONG`**|The length of the packet is too long|
|**`FINS_RETVAL_COMMAND_NOT_SUPPORTED`**|The issued FINS command is not supported by the remote CPU|
|**`FINS_RETVAL_ALL_CONNECTIONS_IN_USE`**|All the connections are in use|
|**`FINS_RETVAL_NODE_ALREADY_CONNECTED`**|The node is already connected|
|**`FINS_RETVAL_NODE_IP_PROTECTED`**|The IP address of the node is connected|
|**`FINS_RETVAL_CLIENT_NODE_OUT_OF_RANGE`**|The client node number is out of range|
|**`FINS_RETVAL_SAME_NODE_ADDRESS`**|The client and server use the same node address|
|**`FINS_RETVAL_NO_NODE_ADDRESS_AVAILABLE`**|All free node address slots are in use|
|**`FINS_RETVAL_TRY_LATER`**|The system is busy, please try again later|
|**`FINS_RETVAL_NOT_INITIALIZED`**|The FINS contect is not initialized|
|**`FINS_RETVAL_NOT_CONNECTED`**|There is no active connection with the remote peer|
|**`FINS_RETVAL_OUT_OF_MEMORY`**|An out of memory error occured|
|**`FINS_RETVAL_INVALID_IP_ADDRESS`**|An invalid IP address was specified|
|**`FINS_RETVAL_NO_READ_ADDRESS`**|No remote read address specified in function call|
|**`FINS_RETVAL_NO_WRITE_ADDRESS`**|No remote write address specified in function call|
|**`FINS_RETVAL_NO_DATA_BLOCK`**|There was no data block specified in the function call to store received or to send data|
|**`FINS_RETVAL_INVALID_READ_ADDRESS`**|No valid memory address for reading data from the PLC was provided|
|**`FINS_RETVAL_INVALID_WRITE_ADDRESS`**|No valid memory address for writing data to the PLCwas provided|
|**`FINS_RETVAL_INVALID_READ_AREA`**|The specified memory area is not available for reading data|
|**`FINS_RETVAL_INVALID_WRITE_AREA`**|The specified memory area is not available for writing data|
|**`FINS_RETVAL_INVALID_FILL_AREA`**|The specified memory area is not available for filling with data|
|**`FINS_RETVAL_INVALID_PARAMETER_AREA`**|The specified parameter area is invalid|
|**`FINS_RETVAL_INVALID_DISK`**|The specified disk is invalid|
|**`FINS_RETVAL_INVALID_PATH`**|The specified directory path is invalid|
|**`FINS_RETVAL_INVALID_FILENAME`**|The specified filename is invalid|
|**`FINS_RETVAL_INVALID_DATA`**|The specified date is invalid|
|**`FINS_RETVAL_NO_COMMAND`**|There was no FINS command specified|
|**`FINS_RETVAL_NO_COMMAND_LENGTH`**|There was no FINS command length specified|
|**`FINS_RETVAL_BODY_TOO_SHORT`**|The body of the FINS command is too short|
|**`FINS_RETVAL_BODY_TOO_LONG`**|The body of the FINS command is too long|
|**`FINS_RETVAL_HEADER_SEND_ERROR`**|An error occured while sending the FINS command header|
|**`FINS_RETVAL_COMMAND_SEND_ERROR`**|An error occured while sending the FINS command body|
|**`FINS_RETVAL_RESPONSE_INCOMPLETE`**|The response received from the other peer was incomplete|
|**`FINS_RETVAL_ILLEGAL_FINS_COMMAND`**|The FINS command specified is illegal|
|**`FINS_RETVAL_RESPONSE_HEADER_INCOMPLETE`**|The header of the response is shorter than expected|
|**`FINS_RETVAL_INVALID_FORCE_COMMAND`**|The specified command to force a bit is invalid|
|**`FINS_RETVAL_LOCAL_NODE_NOT_IN_NETWORK`**|The local node is currently not connected a a network|
|**`FINS_RETVAL_LOCAL_TOKEN_TIMEOUT`**|Waiting for a token timed out|
|**`FINS_RETVAL_LOCAL_RETRIES_FAILED`**|The local node failed after the specified amount of retries|
|**`FINS_RETVAL_LOCAL_TOO_MANY_SEND_FRAMES`**|Too many send frames are currently processed|
|**`FINS_RETVAL_LOCAL_ADDRESS_RANGE_ERROR`**|The local node has an address setting error|
|**`FINS_RETVAL_LOCAL_ADDRESS_DUPLICATION`**|The address of the local node is a duplicate of the address of another node|
