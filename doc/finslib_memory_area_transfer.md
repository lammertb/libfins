# Libfins API Reference

### `finslib_memory_area_transfer( sys, source, dest, num_words );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`source`**|`const char *source`|ASCII representation of the source starting memory area address|
|**`dest`**|`const char *dest`|ASCII representation of the destination starting memory area address|
|**`num_words`**|`size_t`|The number of words to transfer

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](FINS_RETVAL.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`finslib_area_to_file_transfer();`](finslib_area_to_file_transfer.md)
* [`finslib_file_to_area_transfer();`](finslib_file_to_area_transfer.md)
* [`finslib_memory_area_fill();`](finslib_memory_area_fill.md)
* [`finslib_memory_area_read_word();`](finslib_memory_area_read_word.md)
* [`finslib_memory_area_write_word();`](finslib_memory_area_write_word.md)
