# Libfins API Reference

### `finslib_message_read( sys, msgdata, msg_mask );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`msgdata`**|`struct fins_msgdata_tp *`|Pointer to the buffer where the message information should be stored|
|**`msg_mask`**|`uint8_t`|Bit mask identifying the messages to be retrieved|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`finslib_message_clear();`](finslib_message_read.md)
* [`finslib_message_fal_fals_read();`](finslib_message_fal_fals_read.md)
