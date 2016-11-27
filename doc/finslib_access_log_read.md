# Libfins API Reference

### `finslib_access_log_read( sys, accessdata, start_record, num_records, stored_records );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`accessdata`**|`struct fins_accessdata_tp *`|Pointer to the buffer where the access data must be stored|
|**`start_record`**|`uint16_t`|The first access record to retrieve|
|**`num_records`**|`size_t`|The number of records to retrieve and at successful function return the actual number of records read|
|**`stored_records`**|`size_t`|The total number of stored records|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`finslib_write_access_log_clear();`](finslib_write_access_log_clear.md)
