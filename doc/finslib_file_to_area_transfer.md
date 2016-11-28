# Libfins API Reference

### `finslib_file_to_area_transfer( sys, start, disk, path, file, num_records );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`start`**|`const char *`|ASCII representation of the starting memory address in the PLC where the file data should be stored|
|**`disk`**|`uint16_t`|The disk where the source file is located|
|**`path`**|`const char *`|The subdirectory where the source file is located|
|**`file`**|`const char *`|The name of the source file|
|**`num_records`**|`size_t *`|The number of records to transfer|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](fins_retval.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](fins_retval.md) &ndash; Libfins function return code list
* [`finslib_area_file_compare();`](finslib_area_file_compare.md)
* [`finslib_area_to_file_transfer();`](finslib_area_to_file_transfer.md)
* [`finslib_valid_directory();`](finslib_valid_directory.md)
* [`finslib_valid_filename();`](finslib_valid_filename.md)
