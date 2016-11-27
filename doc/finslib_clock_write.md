# Libfins API Reference

### `finslib_clock_write( sys, date_time, do_sec, do_dow );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`datetime`**|`struct fins_datetime_tp *`|A pointer to a structure with the data and time which must be transferred to the PLC|
|**`do_sec`**|`bool`|**`true`** when the seconds on the remote PLC must also be set.
|**`do_dow`**|`bool`|**`true`** when the day of the wak on the remote PLC must also be set. This parameter is only functional when `do_sec` is also set to **`true`**|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`finslib_clock_read();`](finslib_clock_read.md)
