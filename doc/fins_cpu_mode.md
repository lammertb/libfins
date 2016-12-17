# Libfins API Reference

### PLC CPU modes

| Name | Description |
| :--- | :--- |
|**`FINS_CPU_MODE_MONITOR`**|Status code indicating that the PLC CPU is in monitor mode|
|**`FINS_CPU_MODE_PROGRAM`**|Status code indicating that the PLC CPU is in program mode|
|**`FINS_CPU_MODE_RUN`**|Status code indicating that the PLC CPU is in run mode|

### Description

The CPU of a PLC can be in different modes. It can be stopped, running or running with monitoring switched on. The functions [`finslib_set_cpu_run()`](finslib_set_cpu_run.md) and [`finslib_set_cpu_stop()`](finslib_set_cpu_stop.md) are used to change the current mode of the PLC. The current mode can be read with a call to [`finslib_cpu_unit_status_read()`](finsliv_cpu_unit_status_read.md). Where applicable, a constant from the range `FINS_CPU_MODE...` is used to set or read the CPU mode.

### See Also

* [`finslib_cpu_unit_status_read();`](finslib_cpu_unit_status_read.md)
* [`finslib_cycle_time_read();`](finslib_cpu_cycle_time_read.md)
* [`finslib_set_cpu_run();`](finslib_set_cpu_run.md)
* [`finslib_set_cpu_stop();`](finslib_set_cpu_stop.md)
