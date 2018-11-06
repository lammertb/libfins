# Finslib API Reference

### `finslib_multiple_memory_area_read( sys, item, num_item );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`item`**|`struct fins_multidata_tp *`|Pointer to an array of structures where each element contains the information and storage place for one item to be retrieved|
|**`num_item`**|`size_t`|The number of items to return|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](fins_retval.md) indicating the result of the query|

### Description

The function `finslib_multiple_memory_area_read()` is the most versatile of all memory read functions. It takes an array of elements as a parameter where each element contains information about one memory item to be read.  In one function call information from multiple memory areas can be read and also value types like BCD and signed integer and value size like bit, 16 or 32 bit words may be mixed. The connection with the PLC should already be present before this function is called.

Ensuring that the array length matches the number of elements parameter in the function call is the responsibility of the calling function but `finslib_multiple_memory_area_read()` will return an error if a NULL pointer is provided for data storage.

For each element in the `item` array, the fields `address` and `type` must be filled by the calling function.  The function `finslib_muliple_memory_area_read()` will then fill one or two of the other fields depending on the type of the requested element.

The `address` of each element is provided as an ASCII string which represents the starting address in human readable format. Example formats are **`CIO20`** and **`W100.5`**.

The `type` of each element is one of the [`FINS_DATA_TYPE...`](fins_data_type.md) constants.

The requested number of elements is not limited by the amount of data a PLC can send in one FINS packet because `finslib_multiple_memory_area_read()` will automatically use multiple requests at the FINS layer if the dataset will be too large.

The return value is either **`FINS_RETVAL_SUCCESS`** when the function succeeded, or one of the other **`FINS_RETVAL_`** values if an error occurs. In the latter case the data in the return buffer is unreliable and should not be used.

### See Also

* [`FINS_RETVAL...`](fins_retval.md) &ndash; Libfins function return code list
* [`FINS_DATA_TYPE...`](fins_data_type.md) &ndash; Libfins data types
* [`FINS_FORCE...`](fins_force.md) &ndash; Bit force mode list
* [`finslib_memory_area_read_bcd16();`](finslib_memory_area_read_bcd16.md)
* [`finslib_memory_area_read_bcd32();`](finslib_memory_area_read_bcd32.md)
* [`finslib_memory_area_read_bit();`](finslib_memory_area_read_bit.md)
* [`finslib_memory_area_read_double();`](finslib_memory_area_read_double.md)
* [`finslib_memory_area_read_float();`](finslib_memory_area_read_float.md)
* [`finslib_memory_area_read_int16();`](finslib_memory_area_read_int16.md)
* [`finslib_memory_area_read_int32();`](finslib_memory_area_read_int32.md)
* [`finslib_memory_area_read_sbcd16();`](finslib_memory_area_read_sbcd16.md)
* [`finslib_memory_area_read_sbcd32();`](finslib_memory_area_read_sbcd32.md)
* [`finslib_memory_area_read_uint16();`](finslib_memory_area_read_uint16.md)
* [`finslib_memory_area_read_uint32();`](finslib_memory_area_read_uint32.md)
* [`finslib_memory_area_read_word();`](finslib_memory_area_read_word.md)
