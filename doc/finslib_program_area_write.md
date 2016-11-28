# Libfins API Reference

### `finslib_program_area_write( sys, data, start_word, num_bytes );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`data`**|`const unsigned char *`|Pointer to the buffer with data which must be written to the program area|
|**`start_word`**|`uint32_t`|Offset in the program area where writing data must begin|
|**`num_bytes`**|`size_t`|Number of bytes to write|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](fins_retval.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](fins_retval.md) &ndash; Libfins function return code list
* [`finslib_memory_area_write_word();`](finslib_memory_area_write_word.md)
* [`finslib_parameter_area_write();`](finslib_parameter_area_write.md)
* [`finslib_program_area_clear();`](finslib_program_area_clear.md)
* [`finslib_program_area_read();`](finslib_program_area_read.md)
