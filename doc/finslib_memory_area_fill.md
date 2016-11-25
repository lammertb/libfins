# Finslib API Reference

### `finslib_memory_area_fill( sys, start, fill_data, num_word );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`start`**|`const char *`|An ASCII string describing the first memory element to fill|
|**`fill_data`**|`uint16_t`|A 16 bit word containing the data to be written to all the affected words in the remote PLC memory area|
|**`num_word`**|`size_t`|The number of words to write|

### Returns

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the command|

### Description

The function `finslib_memory_area_fill()` can be used to fill a block of 16 bit words in a memory
area in a remote PLC with a fill word. The connection with the PLC should already be present before this function is called.

The start of the memory area is provided as an ASCII string which represents the starting address in human
readable format. Example formats are **`CIO20`** and **`W100`**.

The return value is either **`FINS_RETVAL_SUCCESS`** when the function succeeded, or one of the other
**`FINS_RETVAL_`** values if an eror occurs. In the latter case depending on the error message it is not sure if none, some or all of the data has
been written to the PLC and additional processing and communication with the PLC may be necessary to know or set
the correct state of the memory contents of the PLC.

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`finslib_memory_area_transfer();`](finslib_memory_area_transfer.md)
* [`finslib_memory_area_write_word();`](finslib_memory_area_write_word.md)
* [`finslib_parameter_area_clear();`](finslib_parameter_area_clear.md)
* [`finslib_program_area_clear();`](finslib_program_area_clear.md)
