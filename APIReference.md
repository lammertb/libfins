# Libfins API Reference

Libfins is a library to communicate over the FINS/TCP protocol over ethernet with Omron PLCs. The library is written
in C and can be compiled with any modern C compiler. The API to the library is described in this document.

*This document is currently work in progress.*

## Constants

* [PLC CPU modes](doc/FINS_CPU_MODE.md)
* [Default communication settings](doc/FINS_DEFAULT.md)
* [Bit force modes](doc/FINS_FORCE.md)
* [FINS user message masks](doc/FINS_MSG.md)
* [Multiple read parameter types](doc/FINS_MULTI_TYPE.md)
* [Function return values](doc/FINS_RETVAL.md)

## Structures

* [`struct fins_cpustatus_tp;`](doc/fins_cpustatus_tp.md)
* [`struct fins_cycletime_tp;`](doc/fins_cycletime_tp.md)
* [`struct fins_unitdata_tp;`](doc/fins_unitdata_tp.md)

## Functions

* [`finslib_errmsg( error_code, buffer, buffer_len );`](doc/finslib_errmsg.md)
* [`finslib_memory_area_read_bcd16( sys, start, data, num_bcd16 );`](doc/finslib_memory_area_read_bcd16.md)
* [`finslib_memory_area_read_bcd32( sys, start, data, num_bcd32 );`](doc/finslib_memory_area_read_bcd32.md)
* [`finslib_memory_area_read_bit( sys, start, data, num_bit );`](doc/finslib_memory_area_read_bit.md)
* [`finslib_memory_area_read_word( sys, start, data, num_words );`](doc/finslib_memory_area_read_word.md)
* [`finslib_milli_second_sleep( int msec );`](doc/finslib_milli_second_sleep.md)
* [`finslib_monotonic_sec_timer( void );`](doc/finslib_monotonic_sec_timer.md)
