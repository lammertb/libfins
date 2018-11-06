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

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](fins_retval.md) indicating the result of the query|

### Description

The function `finslib_parameter_area_read()` can be used to read data from a parameter area.  The area to read must be selected with a [`FINS_PARAM_AREA...`](fins_param_area.md) parameter Also the start offset and number of words must be passed to the function as parameters.  It is the responsibility of the calling function to ensure that the buffer which must contain the returned block of words is large enough.

The return value is either **`FINS_RETVAL_SUCCESS`** when the function succeeded, or one of the other **`FINS_RETVAL_`** values if an error occurs. In the latter case the data in the return buffer is unreliable and should not be used.

### See Also

* [`FINS_RETVAL...`](fins_retval.md) &ndash; Libfins function return code list
* [`FINS_PARAM_AREA...`](fins_param_area.md) &ndash; CPU parameter area list
* [`finslib_memory_area_read_word();`](finslib_memory_area_read_word.md)
* [`finslib_parameter_area_clear();`](finslib_parameter_area_clear.md)
* [`finslib_parameter_area_write();`](finslib_parameter_area_write.md)
* [`finslib_program_area_read();`](finslib_program_area_read.md)
