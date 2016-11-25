# Libfins API Reference

### `finslib_file_read( sys, disk, path, filename, data, file_position, num_bytes );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`disk`**|`uint16_t`|The disk on which the file is located|
|**`path`**|`const char *`|The subdirectory in which the file is located|
|**`filename`**|`const char *`|The name of the file|
|**`data`**|`unsigned char *`|The buffer where the file contents must be stored|
|**`file_position`**|`size_t`|The position in the file where reading should start|
|**`num_bytes`**|`size_t *`|The amount of bytes requested and at function return the actual amount of bytes read|

### Returns

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`finslib_filename_to_83();`](finslib_filename_to_83.md)
* [`finslib_file_name_read();`](finslib_file_name_read.md)
* [`finslib_file_write();`](finslib_file_write.md)
