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

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`finslib_memory_area_write_word();`](finslib_memory_area_write_word.md)
* [`finslib_parameter_area_clear();`](finslib_parameter_area_clear.md)
* [`finslib_parameter_area_read();`](finslib_parameter_area_read.md)
* [`finslib_program_area_write();`](finslib_program_area_write.md)
