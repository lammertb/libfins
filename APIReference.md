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
* [`finslib_memory_area_fill( sys, start, fill_data, num_word );`](doc/finslib_memory_area_fill.md)
* [`finslib_memory_area_read_bcd16( sys, start, data, num_bcd16 );`](doc/finslib_memory_area_read_bcd16.md)
* [`finslib_memory_area_read_bcd32( sys, start, data, num_bcd32 );`](doc/finslib_memory_area_read_bcd32.md)
* [`finslib_memory_area_read_bit( sys, start, data, num_bit );`](doc/finslib_memory_area_read_bit.md)
* [`finslib_memory_area_read_int16( sys, start, data, num_int16 );`](doc/finslib_memory_area_read_int16.md)
* [`finslib_memory_area_read_int32( sys, start, data, num_int32 );`](doc/finslib_memory_area_read_int32.md)
* [`finslib_memory_area_read_uint16( sys, start, data, num_uint16 );`](doc/finslib_memory_area_read_uint16.md)
* [`finslib_memory_area_read_uint32( sys, start, data, num_uint32 );`](doc/finslib_memory_area_read_uint32.md)
* [`finslib_memory_area_read_word( sys, start, data, num_word );`](doc/finslib_memory_area_read_word.md)
* [`finslib_memory_area_write_bcd16( sys, start, data, num_bcd16 );`](doc/finslib_memory_area_write_bcd16.md)
* [`finslib_memory_area_write_bcd32( sys, start, data, num_bcd32 );`](doc/finslib_memory_area_write_bcd32.md)
* [`finslib_memory_area_write_bit( sys, start, data, num_bit );`](doc/finslib_memory_area_write_bit.md)
* [`finslib_memory_area_write_int16( sys, start, data, num_int16 );`](doc/finslib_memory_area_write_int16.md)
* [`finslib_memory_area_write_int32( sys, start, data, num_int32 );`](doc/finslib_memory_area_write_int32.md)
* [`finslib_memory_area_write_uint16( sys, start, data, num_uint16 );`](doc/finslib_memory_area_write_uint16.md)
* [`finslib_memory_area_write_uint32( sys, start, data, num_uint32 );`](doc/finslib_memory_area_write_uint32.md)
* [`finslib_memory_area_write_word( sys, start, data, num_word );`](doc/finslib_memory_area_write_word.md)
* [`finslib_multiple_memory_area_read( sys, item, num_item );`](doc/finslib_multiple_memory_area_read.md)
* [`finslib_milli_second_sleep( int msec );`](doc/finslib_milli_second_sleep.md)
* [`finslib_monotonic_sec_timer( void );`](doc/finslib_monotonic_sec_timer.md)
