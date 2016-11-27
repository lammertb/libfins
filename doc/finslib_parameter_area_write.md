# Libfins API Reference

### `finslib_parameter_area_write( sys, area_code, data, start_word, num_words );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`area_code`**|`uint16_t`|The parameter area to which the data must be written|
|**`data`**|`const uint16_t *`|Pointer to the buffer containing the data to be written to the parameter area|
|**`start_word`**|`uint16_t`|Offset from the beginning of the parameter area where writing of this data must start|
|**`num_words`**|`size_t`|Number of words to be written to the parameter area|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the query|

### Description

The function `finslib_parameter_area_write()` writes a block of data to a parameter
area of a remote PLC over FINS protocol. The area which must be written is provided
as a parameter. Also the starting offset an number of words to write are parameters
to the `finslib_parameter_area_write()` function.

Note that all areas except for the routing table must be written in its entirety.

The I/O registration table can only be written if the CPU is in program mode. When
that area is written while the CPU is running or in monitor mode, `finslib_parameter_area_write()`
will still return **`FINS_RETVAL_SUCCESS`** but nothing will be written to the area. It
is the responsibility of the calling party to first ensure that the CPU is stopped
before the I/O registration table is written.

The return value is either **`FINS_RETVAL_SUCCESS`** when the function succeeded, or one of the other
**`FINS_RETVAL_`** values if an eror occurs. In the latter case depending on the error message it is
not sure if none, some or all of the data has
been written to the PLC and additional processing and communication with the PLC may be necessary to know or set
the correct state of the parameter area memory contents of the PLC.

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`FINS_PARAM_AREA...`](FINS_PARAM_AREA.md) &ndash; CPU parameter area list
* [`finslib_memory_area_write_word();`](finslib_memory_area_write_word.md)
* [`finslib_parameter_area_clear();`](finslib_parameter_area_clear.md)
* [`finslib_parameter_area_read();`](finslib_parameter_area_read.md)
* [`finslib_program_area_write();`](finslib_program_area_write.md)
