# Libfins API Reference

### `finslib_bcd_to_int( value, type );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`value`**|`uint32_t`|The BCD value which must be converted to binary|
|**`type`**|`int`|The type of conversion to perform|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|The value of the BCD parameter in binary, or `INT16_MAX` or `INT32_MAX` if the parameter was not a valid BCD value|

### Description

The function `finslib_bcd_to_int()` can be used to convert a BCD value to its binary equivalent.  The function can be used to convert 16 and 32 bit input values which may be either signed or unsigned.  Signed BCD values can be encoded in four different ways. To guide the function in selecting the proper conversion protocol, a second parameter is provided which tells according to which conversion algorithm the conversion to a binary value should take place.

If a parameter is provided which does not represent a valid BCD value, the function returns `INT16_MAX` when a 16 bit conversion was requested and `INT32_MAX` otherwise. The following conversion types are supported by the function.

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

* [`FINS_RETVAL...`](FINS_RETVAL.md) &ndash; Libfins function return code list
* [`FINS_DATA_TYPE...`](FINS_DATA_TYPE.md) &ndash; Libfins data types
* [`finslib_int_to_bcd();`](finslib_int_to_bcd.md)
* [`finslib_memory_area_read_bcd16();`](finslib_memory_area_read_bcd16.md)
* [`finslib_memory_area_read_bcd32();`](finslib_memory_area_read_bcd32.md)
* [`finslib_memory_area_read_sbcd16();`](finslib_memory_area_read_sbcd16.md)
* [`finslib_memory_area_read_sbcd32();`](finslib_memory_area_read_sbcd32.md)
* [`finslib_memory_area_write_bcd16();`](finslib_memory_area_write_bcd16.md)
* [`finslib_memory_area_write_bcd32();`](finslib_memory_area_write_bcd32.md)
* [`finslib_memory_area_write_sbcd16();`](finslib_memory_area_write_sbcd16.md)
* [`finslib_memory_area_write_sbcd32();`](finslib_memory_area_write_sbcd32.md)
