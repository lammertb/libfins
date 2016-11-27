# Libfins API Reference

### `finslib_cpu_unit_data_read( sys, cpudata );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`cpudata`**|`struct fins_cpudata_tp *`|A pointer to a structure where the CPU information must be stored|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`finslib_connection_data_read();`](finslib_connection_data_read.md)
* [`finslib_cpu_unit_status_read();`](finslib_cpu_unit_status_read.md)
