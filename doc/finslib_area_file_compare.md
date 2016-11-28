# Libfins API Reference

### `finslib_area_file_compare( sys, start, disk, path, file, num_records );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`start`**|`const char *`|ASCII representation of the first memory word to compare|
|**`disk`**|`uint16_t`|Disk where the file is located|
|**`path`**|`const char *`|Subdirectory path of the file|
|**`file`**|`const char *`|Name of the file|
|**`num_records`**|`size_t *`|The number of records to compare|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](fins_retval.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](fins_retval.md) &ndash; Libfins function return code list
* [`finslib_area_to_file_transfer();`](finslib_area_to_file_transfer.md)
* [`finslib_file_to_area_transfer();`](finslib_file_to_area_transfer.md)
* [`finslib_valid_directory();`](finslib_valid_directory.md)
* [`finslib_valid_filename();`](finslib_valid_filename.md)
