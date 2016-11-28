# Libfins API Reference

### `finslib_forced_set_reset_cancel( sys );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](fins_retval.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](fins_retval.md) &ndash; Libfins function return code list
* [`FINS_FORCE...`](fins_force.md) &ndash; Bit force mode list
* [`finslib_memory_area_read_bit();`](finslib_memory_area_read_bit.md)
* [`finslib_memory_area_read_word();`](finslib_memory_area_read_word.md)
* [`finslib_memory_area_write_bit();`](finslib_memory_area_write_bit.md)
* [`finslib_memory_area_write_word();`](finslib_memory_area_write_word.md)
* [`finslib_multiple_memory_area_read();`](finslib_multiple_memory_area_read.md)
