# Libfins API Reference

### `finslib_message_clear( sys, msg_mask );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`msg_mask`**|`uint8_t`|Bit mask representing the messages to clear|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`finslib_error_clear();`](finslib_error_clear.md)
* [`finslib_error_clear_fal();`](finslib_error_clear_fal.md)
* [`finslib_error_clear_fals();`](finslib_error_clear_fals.md)
* [`finslib_error_log_clear();`](finslib_error_log_clear.md)
* [`finslib_error_log_read();`](finslib_error_log_read.md)
* [`finslib_message_read();`](finslib_message_read.md)
* [`finslib_message_fal_fals_read();`](finslib_message_fal_fals_read.md)
