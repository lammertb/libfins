# Finslib API Reference

### `finslib_memory_area_write_uint32( sys, start, data, num_uint32 );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`start`**|`const char *`|An ASCII string describing the first memory element to write|
|**`data`**|`const uint32_t *`|Pointer to the buffer where the data to be written is located|
|**`num_uint32`**|`size_t`|The number of unsigned 32 bit integer values to write|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the command|

### Description

The function `finslib_memory_area_write_uint32()` can be used to write a block of unsigned 32 bit integer values to a memory area in a remote PLC. The connection with the PLC should already be present before this function is called.

Data is transferred unmodified from the caller supplied buffer to the FINS data stream.  Existence and filling of the dataspace is the responsibility of the calling function, but `finslib_memory_area_write_uint32()` will return an error if a NULL pointer is provided for data storage.

The start of the memory area is provided as an ASCII string which represents the starting address in human readable format. Example formats are **`CIO20`** and **`W100`**.

The number of unsigned integer values to be written in one function call is not limited by the amount of data a PLC can send in one FINS packet because `finslib_memory_area_write_uint32()` will automatically use multiple request at the FINS layer if the dataset is too large.

The return value is either **`FINS_RETVAL_SUCCESS`** when the function succeeded, or one of the other **`FINS_RETVAL_`** values if an eror occurs. In the latter case depending on the error message it is not sure if none, some or all of the data has been written to the PLC and additional processing and communication with the PLC may be necessary to know or set the correct state of the memory contents of the PLC.

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`FINS_DATA_TYPE...`](FINS_DATA_TYPE.md) &ndash; Libfins data types
* [`finslib_memory_area_read_int16();`](finslib_memory_area_read_int16.md)
* [`finslib_memory_area_read_int32();`](finslib_memory_area_read_int32.md)
* [`finslib_memory_area_read_uint16();`](finslib_memory_area_read_uint16.md)
* [`finslib_memory_area_read_uint32();`](finslib_memory_area_read_uint32.md)
* [`finslib_memory_area_write_int16();`](finslib_memory_area_write_int16.md)
* [`finslib_memory_area_write_int32();`](finslib_memory_area_write_int32.md)
* [`finslib_memory_area_write_uint16();`](finslib_memory_area_write_uint16.md)
* [`finslib_mutliple_memory_area_read();`](finslib_multiple_memory_area_read.md)
