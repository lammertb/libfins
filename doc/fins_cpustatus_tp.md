# Finslib API Reference

## Structures

### `struct fins_cpustatus_tp;`

#### Fields

| Field | Type | Description |
| :--- | :--- | :--- |
|**`message_exists`**|`bool[8]`|An array of bits indicating if a user generated message is available|
|**`running`**|`bool`|**`true`** if the CPU is currently running|
|**`flash_writing`**|`bool`|**`true`** if the PLC is currently busy writing to flash memory|
|**`battery_present`**|`bool`|**`true`** if a memory backup battery is present in the PLC|
|**`standby`**|`bool`|**`true`** if the CPU is currently in standby mode|
|**`fatal_memory_error`**|`bool`|**`true`** if a fatal memory error in the PLC occured|
|**`fata_io_bus_error`**|`bool`|**`true`** if a fatal I/O bus error occured|
|**`fata_duplication_error`**|`bool`|**`true`** if a fatal duplication error occured|
|**`fatal_inner_board_error`**|`bool`|**`true`** if a fatal inner board error occured|
|**`fatal_io_point_overflow`**|`bool`|**`true`** if a fatal I/O point overflow occured|
|**`fatal_io_setting_error`**|`bool`|**`true`** if a fatal I/O setting error occured|
|**`fatal_program_error`**|`bool`|**`true`** if a fatal program error occured|
|**`fatal_cycle_time_error`**|`bool`|**`true`** if the PLC stopped due to a cycle time overflow|
|**`fatal_fals_error`**|`bool`|**`true`** if the PLC stopped due to a user program generated FALS error|
|**`fal_error`**|`bool`|**`true`** if the user program generated a non-fatal FAL error|
|**`duplex_error`**|`bool`|**`true`** if a non-fatal duplex error occured|
|**`interrupt_task_error`**|`bool`|**`true`** if a non-fatal interrupt task error occured|
|**`basic_io_unit_error`**|`bool`|**`true`** if a non-fatal basic I/O unit error occured|
|**`plc_setup_error`**|`bool`|**`true`** if a non-fatal PLC setup error occured|
|**`io_verification_error`**|`bool`|**`true`** if a non-fatal I/O verification error occured|
|**`inner_board_error`**|`bool`|**`true`** if a non-fatal inner board error occured|
|**`cpu_bus_unit_error`**|`bool`|**`true`** if a non-fatal CPU bus unit error occured|
|**`special_io_unit_error`**|`bool`|**`true`** if a non-fatal special I/O unit error occured|
|**`sysmac_bus_error`**|`bool`|**`true`** if a non-fatal sysmac bus error occured|
|**`battery_error`**|`bool`|**`true`** if a non-fatal battery error occured|
|**`cs1_cpu_bus_unit_setting_error`**|`bool`|**`true`** if a non-fatal error occured in the settings of a CS1 CPU bus unit|
|**`special_io_unit_setting_error`**|`bool`|**`true`** if q non-fatal error occured in the settings of a special I/O unit|
|**`run_mode`**|`uint8_t`|The current operating mode of the CPU|
|**`error_code`**|`uint16_t`|The active error code with the highest priority|
|**`error_message`**|`char[17]`|The current active error message in ASCII text|

#### Description

The structure `fins_cpustatus_tp` is used to store the actual status of the CPU of a PLC when the function
`finslib_cpu_unit_status_read()` is called.
