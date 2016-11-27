# Libfins API Reference

### `finslib_access_right_release( sys );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`finslib_access_log_read();`](finslib_access_log_read.md)
* [`finslib_access_right_acquire();`](finslib_access_right_acquire.md)
* [`finslib_access_right_forced_acquire();`](finslib_access_right_forced_acquire.md)
* [`finslib_write_access_log_clear();`](finslib_write_access_log_clear.md)
