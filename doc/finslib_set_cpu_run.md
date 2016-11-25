# Libfins API Reference

### `finslib_set_cpu_run( sys, do_monitor );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`do_monitor`**|`bool`|**`true`** if the CPU should run in monitor mode|

### Returns

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`finslib_set_cpu_stop();`](finslib_set_cpu_stop.md)
