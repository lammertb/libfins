# Libfins API Reference

### PLC Parameter Areas

| Name | Description |
| :--- | :--- |
|**`FINS_PARAM_AREA_ALL`**|Function affects all parameter areas|
|**`FINS_PARAM_AREA_CPU_BUS_UNIT_SETUP`**|Area with setup information for CPU bus units|
|**`FINS_PARAM_AREA_IO_TABLE_REGISTRATION`**|Area with I/O registration information|
|**`FINS_PARAM_AREA_PLC_SETUP`**|Area with PLC setup information|
|**`FINS_PARAM_AREA_ROUTING_TABLE`**|Area with the network routing table|

### Description

An Omron PLC contains a number of parameter areas which store settings used for the
functioning of the system and its environment, rather than for the functioning of the
user programs themselves. These parameters store for example information about the units attached to
the CPU and routing tables for networking.

The parameter areas can be read and written over the FINS protocol. For each function call
to a function which affects a parameter area, it is necessary to specify on which parameter
area the activity should be performed. The constants `FINS_PARAM_AREA...` can be used
to specify these areas.

### See Also

* [`finslib_parameter_area_clear();`](finslib_parameter_area_clear.md)
* [`finslib_parameter_area_read();`](finslib_parameter_area_read.md)
* [`finslib_parameter_area_write();`](finslib_parameter_area_write.md)
