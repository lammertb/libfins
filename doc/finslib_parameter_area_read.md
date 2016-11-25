# Libfins API Reference

### `finslib_parameter_area_read( sys, area_code, data, start_word, num_words );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`area_code`**|`uint16_t`|The parameter area from which data must be read|
|**`data`**|`uint16_t *`|Pointer to the buffer where the retrieved data must be stored|
|**`start_word`**|`uint16_t`|Offset of the first wordt in the parameter area which must be read|
|**`num_words`**|`size_t`|The number of words to be read from the parameter area|

### Returns

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`finslib_memory_area_read_word();`](finslib_memory_area_read_word.md)
* [`finslib_parameter_area_clear();`](finslib_parameter_area_clear.md)
* [`finslib_parameter_area_write();`](finslib_parameter_area_write.md)
* [`finslib_program_area_read();`](finslib_program_area_read.md)
