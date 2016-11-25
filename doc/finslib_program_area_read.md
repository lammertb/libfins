# Libfins API Reference

### `finslib_program_area_read( sys, data, start_word, num_bytes );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`data`**|`unsigned char *`|Pointer to the buffer where the retrieved data must be stored|
|**`start_word`**|`uint32_t`|Offset in the program area where reading data must begin|
|**`num_bytes`**|`size_t *`|The amount of bytes to read and if the function returns successfully the actual amount of bytes read|

### Returns

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`finslib_memory_area_read_word();`](finslib_memory_area_read_word.md)
* [`finslib_parameter_area_read_word();`](finslib_parameter_area_read_word.md)
* [`finslib_program_area_clear();`](finslib_program_area_clear.md)
* [`finslib_program_area_write();`](finslib_program_area_write.md)
