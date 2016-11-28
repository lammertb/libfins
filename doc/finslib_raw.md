# Libfins API Reference

### `finslib_raw( sys, command, buffer, send_len, recv_len );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`command`**|`uint16_t`|The command to execute over FINS on the remote PLC|
|**`buffer`**|`unsigned char *`|Buffer which contains both the command body when invoking the function, and the return body when the function is finished|
|**`send_len`**|`size_t`|The number of relevant bytes in the buffer when the function is invoked|
|**`recv_len`**|`size_t *`|The size of the buffer when the function is invoked, and the size of the return body when the function is finished|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](fins_retval.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](fins_retval.md) &ndash; Libfins function return code list
* [`finslib_disconnect();`](finslib_disconnect.md)
* [`finslib_tcp_connect();`](finslib_tcp_connect.md)
