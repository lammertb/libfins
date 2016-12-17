# Libfins API Reference

### `struct fins_cycletime_tp;`

### Fields

| Field | Type | Description |
| :--- | :--- | :--- |
|**`avg`**|`uint32_t`|The average cycle time over the last eight cycles of the PLC. The time is expressed in units of 0.1 msec.|
|**`max`**|`uint32_t`|The maximum cycle time of the PLC since the last cycle time reset. The time is expressed in units of 0.1 msec.|
|**`min`**|`uint32_t`|The minimum cycle time of the PLC since the last cycle time reset. The time is expressed in units of 0.1 msec.|

### Description

The structure `fins_cycletime_tp` is used by the function `finslib_cycle_time_read()` to store the minimum, average and maximum cycle times of the PLC. The average cycle time is calculated over the last eight cycles. Note that the values are in 0.1 millisecond increments.

### See Also

* [`finslib_cpu_unit_status_read();`](finslib_cpu_unit_status_read.md)
* [`finslib_cycle_time_init();`](finslib_cycle_time_init.md)
* [`finslib_cycle_time_read();`](finslib_cycle_time_read.md)
* [`finslib_set_cpu_run();`](finslib_set_cpu_run.md)
* [`finslib_set_cpu_stop();`](finslib_set_cpu_stop.md)
