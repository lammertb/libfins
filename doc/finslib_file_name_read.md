# Libfins API Reference

### `finslib_file_name_read( sys, diskinfo, fileinfo, disk, path, start_file, num_files );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`diskinfo`**|`struct fins_diskinfo_tp *`|Pointer to the location where the disk information should be stored|
|**`fileinfo`**|`struct fins_fileinfo_tp *`|Pointer to the location where the information of the files must be stored|
|**`disk`**|`uint16_t`|The disk from which the information must be read|
|**`path`**|`const char *path`|The subdirectory in which the files are located|
|**`start_file`**|`size *`|The maximum number of files to read|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](fins_retval.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](fins_retval.md) &ndash; Libfins function return code list
* [`finslib_filename_to_83();`](finslib_filename_to_83)
* [`finslib_file_read();`](finslib_file_read.md)
* [`finslib_file_write();`](finslib_file_write.md)
* [`finslib_valid_directory();`](finslib_valid_directory.md)
* [`finslib_valid_filename();`](finslib_valid_filename.md)
