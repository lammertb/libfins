# Finslib API Reference

### `struct fins_unitdata_tp;`

### Fields

| Field | Type | Description |
| :--- | :--- | :--- |
|**`model`**|`char[21]`|The model name of the unit|
|**`unit`**|`uint8_t`|The unit number of the unit|

### Description

The structure `fins_unitdata_tp` is used to store the information of one special I/O unit in a PLC system when unit data
is requested with a call to the `finslib_connection_data_read()` function.

### See Also
