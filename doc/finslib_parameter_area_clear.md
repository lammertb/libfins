# Libfins API Reference

### `finslib_parameter_area_clear( sys, area_code, num_words );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`area_code`**|`uint16_t`|The area code in which data must be cleared|
|**`num_words`**|`size_t`|The number of words that must be cleared in the parameter area|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](fins_retval.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](fins_retval.md) &ndash; Libfins function return code list
* [`FINS_PARAM_AREA...`](fins_param_area.md) &ndash; CPU parameter area list
* [`finslib_memory_area_fill();`](finslib_memory_area_fill.md)
* [`finslib_parameter_area_read();`](finslib_parameter_area_read.md)
* [`finslib_parameter_area_write();`](finslib_parameter_area_write.md)
* [`finslib_program_area_clear();`](finslib_program_area.clear.md)
