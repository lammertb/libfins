# Libfins API Reference

### `finslib_error_log_read( sys, errordata, start_record, num_records, stored_records );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`errordata`**|`struct fins_errordata_tp *`|A pointer to a buffer where the error log data must be stored|
|**`start_record`**|`uint16_t`|The first error log record to be retrieved|
|**`num_records`**|`size_t *`|The number of records to retrieve|
|**`stored_records`**|`size_t *`|The total number of stored records in the error log|

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
* [`finslib_error_clear_fals();`](finslib_error_clear_fals.md)
* [`finslib_error_log_clear();`](finslib_error_log_clear.md)
