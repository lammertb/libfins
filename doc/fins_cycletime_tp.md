# Libfins API Reference

## Structures

### `struct fins_cycletime_tp;`

#### Fields

| Field | Type | Description |
| :--- | :--- | :--- |
|**`min`**|`uint32_t`|The minimum cycle time of the PLC since the last cycle time reset. The time is expressed in units of 0.1 msec.|
|**`avg`**|`uint32_t`|The average cycle time over the last eight cycles of the PLC. The time is expressed in units of 0.1 msec.|
|**`max`**|`uint32_t`|The maximum cycle time of the PLC since the last cycle time reset. The time is expressed in uints of 0.1 msec.|

#### Description

The structure `fins_cycletime_tp` is used by the function `finslib_cycle_time_read()` to store the minimum, average and maximum
cycle times of the PLC.
