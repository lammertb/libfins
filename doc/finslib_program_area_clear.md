# Libfins API Reference

### `finslib_program_area_clear( sys, do_interrupt_tasks );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`do_interrupt_tasks`**|`bool`|**`true`** if the interrupt tasks must be also cleared|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](fins_retval.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](fins_retval.md) &ndash; Libfins function return code list
* [`finslib_memory_area_fill();`](finslib_memory_area_fill.md)
* [`finslib_parameter_area_clear();`](finslib_parameter_area_clear.md)
* [`finslib_program_area_read();`](finslib_program_area_read.md)
* [`finslib_program_area_write();`](finslib_program_area_write.md)
