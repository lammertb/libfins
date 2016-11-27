# Finslib API Reference

### `finslib_memory_area_read_uint16( sys, start, data, num_uint16 );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`start`**|`const char *`|An ASCII string describing the first memory element to retrieve|
|**`data`**|`uint16_t *`|Pointer to the buffer where the result must be stored|
|**`num_uint16`**|`size_t`|The number of 16 bit unsigned integer values to return|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the query|

### Description

The function `finslib_memory_area_read_uint16()` can be used to retrieve a block of 16 bit unsigned integer values from a memory
area in a remote PLC. The connection with the PLC should already be present before this function is called.

Data is transferred unmodified from the FINS data stream to the caller supplied buffer.
Enough dataspace is the responsibility of the calling function, but `finslib_memory_area_read_uint16()`
will return an error if a NULL pointer is provided for data storage.

The start of the memory area is provided as an ASCII string which represents the starting address in human
readable format. Example formats are **`CIO20`** and **`W100`**.

The requested number of unsigned integer values is not limited by the amount of data a PLC can send in one FINS packet because
`finslib_memory_area_read_uint16()` will automatically use multiple request at the FINS layer if the dataset will
be too large.

The return value is either **`FINS_RETVAL_SUCCESS`** when the function succeeded, or one of the other
**`FINS_RETVAL_`** values if an eror occurs. In the latter case the data in the return buffer is unreliable and
should not be used.

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`finslib_memory_area_read_int16();`](finslib_memory_area_read_int16.md)
* [`finslib_memory_area_read_int32();`](finslib_memory_area_read_int32.md)
* [`finslib_memory_area_read_uint32();`](finslib_memory_area_read_uint32.md)
* [`finslib_memory_area_write_int16();`](finslib_memory_area_write_int16.md)
* [`finslib_memory_area_write_int32();`](finslib_memory_area_write_int32.md)
* [`finslib_memory_area_write_uint16();`](finslib_memory_area_write_uint16.md)
* [`finslib_memory_area_write_uint32();`](finslib_memory_area_write_uint32.md)
* [`finslib_multiple_memory_area_read();`](finslib_multiple_memory_area_read.md)
