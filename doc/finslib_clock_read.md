# Libfins API Reference

### `finslib_clock_read( sys, datetime );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`datetime`**|`struct fins_datetime_tp *`|Pointer to the memory location where the read date and time must be stored|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](fins_retval.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](fins_retval.md) &ndash; Libfins function return code list
* [`finslib_clock_write();`](finslib_clock_write.md)
