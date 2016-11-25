# Finslib API Reference

### `finslib_memory_area_write_word( sys, start, data, num_word );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`start`**|`const char *`|An ASCII string describing the first memory element to write|
|**`data`**|`const unsigned char *`|Pointer to the buffer where the data to be written is located|
|**`num_word`**|`size_t`|The number of words to write|

### Returns

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the command|

### Description

The function `finslib_memory_area_write_word()` can be used to write a block of 16 bit words to a memory
area in a remote PLC. The connection with the PLC should already be present before this function is called.

Data is transferred unmodified from the caller supplied buffer to the FINS data stream.
Existence and filling of the dataspace is the responsibility of the calling function, but `finslib_memory_area_write_word()`
will return an error if a NULL pointer is provided for data storage.

The start of the memory area is provided as an ASCII string which represents the starting address in human
readable format. Example formats are **`CIO20`** and **`W100`**.

The number of words to be written in one function call is not limited by the amount of data a PLC can send in one FINS packet because
`finslib_memory_area_write_word()` will automatically use multiple request at the FINS layer if the dataset is 
too large.

The return value is either **`FINS_RETVAL_SUCCESS`** when the function succeeded, or one of the other
**`FINS_RETVAL_`** values if an eror occurs. In the latter case depending on the error message it is not sure if none, some or all of the data has
been written to the PLC and additional processing and communication with the PLC may be necessary to know or set
the correct state of the memory contents of the PLC.

### See Also
