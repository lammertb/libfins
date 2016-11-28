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

The function `finslib_memory_area_transfer()` can be used to transfer a block of data between two memory areas. The source and destination memory areas may be different which makes it possible to copy blocks of data from for example the `WR` area to the `DM` area. If a number of words of 0 is used, the function will return with a success code but there will be no data transfered.

Writing to the or counter areas causes the completion flags of the affected timers and counters to be turned off. Note that this function can be called when the CPU is in running mode and that the system may be negatively effected by the memory transfer. The calling party should make sure that the memory transfer will not interfere with a running process or that the CPU is in stop mode, before `finslib_memory_area_transfer()` is called.

### See Also

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`FINS_DATA_TYPE...`](FINS_DATA_TYPE.md) &ndash; Libfins data types
* [`finslib_area_to_file_transfer();`](finslib_area_to_file_transfer.md)
* [`finslib_cpu_unit_status_read();`](finslib_cpu_unit_status_read.md)
* [`finslib_file_to_area_transfer();`](finslib_file_to_area_transfer.md)
* [`finslib_memory_area_fill();`](finslib_memory_area_fill.md)
* [`finslib_memory_area_read_word();`](finslib_memory_area_read_word.md)
* [`finslib_memory_area_write_word();`](finslib_memory_area_write_word.md)
* [`finslib_set_cpu_stop();`](finslib_set_cpu_stop.md)
