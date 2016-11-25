# Libfins API Reference

### `finslib_error_clear_fals( sys, fals_number );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`fals_number`**|`uint16_t`|The number of the FALS error to be cleared|

### Returns

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`finslib_error_clear();`](finslib_error_clear.md)
* [`finslib_error_clear_all();`](finslib_error_clear_all.md)
* [`finslib_error_clear_current();`](finslib_error_clear_current.md)
* [`finslib_error_clear_fal();`](finslib_error_clear_fal.md)
* [`finslib_error_log_clear();`](finslib_error_log_clear.md)
* [`finslib_error_log_read();`](finslib_error_log_read.md)
