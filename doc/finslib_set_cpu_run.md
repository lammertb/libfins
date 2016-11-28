# Libfins API Reference

### `finslib_set_cpu_run( sys, do_monitor );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`do_monitor`**|`bool`|**`true`** if the CPU should run in monitor mode|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](fins_retval.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](fins_retval.md) &ndash; Libfins function return code list
* [`FINS_CPU_MODE_...`](doc/fins_cpu_mode.md) &ndash; PLC CPU modes
* [`finslib_set_cpu_stop();`](finslib_set_cpu_stop.md)
* [`finslib_cpu_unit_status_read();`](finslib_cpu_unit_status_read.md)
* [`finslib_cycle_time_read();`](finslib_cycle_time_read.md)
