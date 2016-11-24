# Finslib API Reference

## Functions

### `finslib_errmsg( error_code, buffer, buffer_len );`

#### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`error_code`**|`int`|The return value of a FINS function which must be translated to a human readable error message|
|**`buffer`**|`char *`|The buffer where the return string must be stored|
|**`buffer_len`**|`size_t`|The amount of characters including the zero termination character of the string which can be stored in the buffer|

#### Returns

| Type | Description |
| :--- | :--- |
|`const char *`|A pointer to the string message when successful, or NULL when an unrecoverable error occured|

#### Description

All functions in the library which have the option of failing return an integer return code. This code can either be an operating system error,
an error returned from the remote peer over the FINS protocol, or an error which occured in the libfins library itself. The function
`finslib_errmsg()` can be used to translate such an error number to a human readable string.

If the function fails to provide an error message due to an internal problem like an unexisting string buffer, the function will return NULL.
In all other cases the caller provided buffer will be filled with the appropriate error message and the function returns a pointer to the
beginning of that buffer. In case the error code is unknown but the function doesn't fail due to an internal error, the text "Unknown error"
is returned.
