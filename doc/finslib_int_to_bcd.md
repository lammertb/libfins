# Libfins API Reference

### `finslib_int_to_bcd( value, type );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`value`**|`int32_t`|The value to convert from binary to BCD|
|**`type`**|`int`|The type of binary to BCD conversion to perform|

### Return Value

| Type | Description |
| :--- | :--- |
|`uint32_t`|The binary value converted to BCD or `INT16_MAX` or `INT32_MAX` if an error occured|

### Description

The function `finslib_int_to_bcd()` can be used to convert a binary value to its BCD BCD equivalent.  The function can be used to convert 16 and 32 bit input values which may be either signed or unsigned.  Signed BCD values can be encoded in four different ways. To guide the function in selecting the proper conversion protocol, a second parameter is provided which tells according to which conversion algorithm the conversion to a BCD value should take place.

If the input parameter is out of range, the function returns `INT16_MAX` when a 16 bit conversion was requested and `INT32_MAX` otherwise. The following conversion types are supported by the function.

|Type|Low|High|On error|
|:---|---:|---:|
|`FINS_DATA_TYPE_BCD16`|0|9999|`INT16_MAX`|
|`FINS_DATA_TYPE_SBCD16_0`|-999|999|`INT16_MAX`|
|`FINS_DATA_TYPE_SBCD16_1`|-7999|7999|`INT16_MAX`|
|`FINS_DATA_TYPE_SBCD16_2`|-999|9999|`INT16_MAX`|
|`FINS_DATA_RTPE_SBCD16_3`|-1999|9999|`INT16_MAX`|
|`FINS_DATA_TYPE_BCD32`|0|99999999|`INT32_MAX`|
|`FINS_DATA_TYPE_SBCD32_0`|-9999999|9999999|`INT32_MAX`|
|`FINS_DATA_TYPE_SBCD32_1`|-79999999|79999999|`INT32_MAX`|
|`FINS_DATA_TYPE_SBCD32_2`|-9999999|99999999|`INT32_MAX`|
|`FINS_DATA_TYPE_SBCD32_3`|-19999999|99999999|`INT32_MAX`|
|*all other*|0|-1|`INT32_MAX`|

The types **0**, **1**, **2** and **3** are the encoding types as used in Omron PLCs by the functions **`BINS`** and **`BISL`**.  More information can be found in the Omron instruction set manuals.

### See Also

* [`FINS_RETVAL...`](fins_retval.md) &ndash; Libfins function return code list
* [`FINS_DATA_TYPE...`](fins_data_type.md) &ndash; Libfins data types
* [`finslib_bcd_to_int();`](finslib_bcd_to_int.md)
* [`finslib_memory_area_read_bcd16();`](finslib_memory_area_read_bcd16.md)
* [`finslib_memory_area_read_bcd32();`](finslib_memory_area_read_bcd32.md)
* [`finslib_memory_area_read_sbcd16();`](finslib_memory_area_read_sbcd16.md)
* [`finslib_memory_area_read_sbcd32();`](finslib_memory_area_read_sbcd32.md)
* [`finslib_memory_area_write_bcd16();`](finslib_memory_area_write_bcd16.md)
* [`finslib_memory_area_write_bcd32();`](finslib_memory_area_write_bcd32.md)
* [`finslib_memory_area_write_sbcd16();`](finslib_memory_area_write_sbcd16.md)
* [`finslib_memory_area_write_sbcd32();`](finslib_memory_area_write_sbcd32.md)
