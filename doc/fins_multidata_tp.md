## Structures

### `struct fins_multidata_tp;`

#### Fields

| Field | Type | Description |
| :--- | :--- | :--- |
|**`address`**|`char[12]`|An ASCII representation of the starting address of the element. For example CIO20.1 or W100|
|**`type`**|`int`|The type of the element to be retrieved. This is one of the [`FINS_MULTI_TYPE...`](FINS_MULTI_TYPE.md) values|
|**`int16`**|`int16_t`|The returned element value if it is a signed 16 bit integer or signed 16 bit BCD|
|**`int32`**|`int32_t`|The returned element value if it is a signed 32 bit integer or signed 32 bit BCD|
|**`uint16`**|`uint16_t`|The returned element value if it is an unsigned 16 bit integer or unsigned 16 bit BCD|
|**`uint32`**|`uint32_t`|The returned element value if it is an unsigned 32 bit integer or unsigned 32 bit BCD|
|**`sfloat`**|`float`|The returned element value if it is a single precision 32 bit floating point|
|**`dfloat`**|`double`|The returned element value if it is a double precision 64 bit floating point|
|**`bit`**|`bool`|The returned element value if it is a bit|
|**`b_force`**|`bool`|The forced status of a bit value if a bit with force status was requested|
|**`word`**|`uint16_t`|The returned element value if an unmodified word was requested|
|**`w_force`**|`uint16_t`|The forced status of the bits of an unmodified word if the force status was requested|

#### Description

The structure `fins_multidata_tp` is used by the function [`finslib_mutliple_memory_area_read()`](finslib_multiple_memory_area_read.md)
to retrieve one or more elements from one or more memory areas in a remote PLC. More information of the usage of
this structure can be found in the documentation of that function.
