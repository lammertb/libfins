# Libfins API Reference

### `finslib_cycle_time_read( sys, ctime );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`ctime`**|`struct fins_cycletime_tp *`|A pointer to a structure where the cycle times must be stored|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](fins_retval.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](fins_retval.md) &ndash; Libfins function return code list
* [`finslib_cycle_time_init();`](finslib_cycle_time_init.md)
* [`finslib_cpu_unit_status_read();`](finslib_cpu_unit_status_read.md)
