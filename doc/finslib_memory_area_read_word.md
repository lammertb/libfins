# Finslib API Reference

## Functions

### `finslib_memory_area_read_word( sys, start, data, num_words );`

#### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`start`**|`const char *`|An ASCII string describing the first memory element to retrieve|
|**`data`**|`unsigned char *`|Pointer to the buffer where the result must be stored|
|**`num_words`**|`size_t`|The number of words to return|

#### Returns

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list `FINS_RETVAL_...` indicating the result of the query|

#### Description

The function `finslib_memory_area_read_word()` can be used to retrieve a block of 16 bit words from a memory
are in a remote PLC. The connection with the PLC should already be present before this function is called.

Data is transferred unmodified from the FINS data stream to the caller supplied buffer. As this function
returns a number of words, the size of the data buffer in bytes should be at least twice the number of words
requested. Enough dataspace is the responsibility of the calling function, but `finslib_memory_area_read_word()`
will return an error if a NULL pointer is provided for data storage.

The start of the memory area is provided as an ASCII string which represents the starting address in human
readable format. Example formats are **`CIO20`** and **`W100`**.

The requested number of words is not limited by the amount of data a PLC can send in one FINS packet because
`finslib_memory_area_read_word()` will automatically use multiple request at the FINS layer if the dataset will
be too large.

The return value is either **`FINS_RETVAL_SUCCESS`** when the function succeeded, or one of the other
**`FINS_RETVAL_`** values if an eror occurs. In the latter case the data in the return buffer is unreliable and
should not be used.
