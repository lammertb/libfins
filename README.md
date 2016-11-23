# Libfins - Multi platform MIT licensed CRC library in C
Libfins is a multi platform library with routines to communicate with Omron PLCs
over the FINS protocol. FINS is a inter-PLC protocol which allows hardware layer independent exchange of
data between PLCs. Omron PLCs support the exchange of data over serial lines, TCP, UDP and bus systems.
This library currently implements the FINS/TCP communication.

FINS is a client server protocol where a client sends requests to a remote server and processes their
answer. The libfins library contains almost all of the publically documented functionality. The only
functionality currently missing is using a PLC as gateway to execute Compoway/F and Modbus commands
over SCI serial unit. These features will be implemented and tested in the near future though.

## License

The libfins library is licensed with the MIT license. This should give maximum flexibility when
using the library in both open en closed source applications.

## Server functionality, UDP and other hardware layers

The libfins library currently supports client mode operation and FINS/TCP as hardware layer only.
Both server functionality and support for other hardware layers are available, but not as part of this
open source software package. If you would like more information, please [contact me directly](https://www.lammertbies.nl/whoami/address.html).

## Installation of the library

The installation procedure of the library is covered in a separate [installation document](INSTALL).

## Usage of the library

The [examples](examples) sub directory contains examples explaining how the routines in the library can be
integrated in an application. Furthermore there is an [API reference](APIReference.md) as work in progress.

## Multi platform

The Libfins library is developed to be used on multiple platforms. It currently supports Linux, Windows, OS-X
and FreeBSD in 32 and 64 bit and on i86 and AMD architectures. New versions of the code are
regularly compiled and checked on the systems mentioned in the following lists.

### 32 bit development environments
|Operating System|Compiler|
| :--- | :--- |
|Centos 6.8|gcc 4.4.7|
|Debian 8.6|gcc 4.9.2|
|FreeBSD 10.3|clang 3.4.1|
|Raspbian|gcc 4.8|
|Windows 7|Visual Studio 2015|

### 64 bit development environments
|Operating system|Compiler|
| :--- | :--- |
|Centos 6.8|gcc 4.4.7|
|Centos 7.2.1511|gcc 4.8.5|
|Debian 8.6|gcc 4.9.2|
|FreeBSD 10.3|clang 3.4.1|
|OS X El Capitan 10.11.6|Apple LLVM 8.0.0|
|Windows 7|Visual Studio 2015|
