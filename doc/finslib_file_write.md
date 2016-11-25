# Libfins API Reference

### `finslib_file_write( sys, disk, path, filename, data, file_position, num_bytes, open_mode );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`disk`**|`uint16_t`|The disk where the file is located|
|**`path`**|`const char *`|The subdirectory where the file is located|
|**`filename`**|`const char *`|The name of the file|
|**`data`**|`const unsigned char *`|Pointer to the buffer with the data which must be written to the file|
|**`file_position`**|`size_t`|The offset from the beginning of the file where data writing should start|
|**`num_bytes`**|`size_t`|The number of bytes which must be written|
|**`open_mode`**|`uint16_t`|The mode in which the file should be opened|

### Returns

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`finslib_filename_to_83();`](finslib_filename_to_83.md)
* [`finslib_file_name_read();`](finslib_file_name_read.md)
* [`finslib_file_read();`](finslib_file_read.md)
