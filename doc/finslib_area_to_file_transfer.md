# Libfins API Reference

### `finslib_area_to_file_transfer( sys, start, disk, path, file, num_records );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`start`**|`const char *`|ASCII representation of the start memory address|
|**`disk`**|`uint16_t`|Disk where the file is located|
|**`path`**|`const char *`|Sub directory path where the file is located|
|**`file`**|`const char *`|Name of the file where the data must be written to|
|**`num_records`**|`size_t *`|Number of records written to the file|

### Returns

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`finslib_area_file_compare();`](finslib_area_file_compare.md)
* [`finslib_file_to_area_transfer();`](finslib_file_to_area_transfer.md)
