[![MSBuild](https://github.com/lammertb/libfins/actions/workflows/msbuild.yml/badge.svg)](https://github.com/lammertb/libfins/actions/workflows/msbuild.yml)
[![Windows](https://github.com/lammertb/libfins/actions/workflows/cmake-windows.yml/badge.svg)](https://github.com/lammertb/libfins/actions/workflows/cmake-windows.yml)
[![Linux](https://github.com/lammertb/libfins/actions/workflows/cmake.yml/badge.svg)](https://github.com/lammertb/libfins/actions/workflows/cmake.yml)
[![MacOS](https://github.com/lammertb/libfins/actions/workflows/cmake-macos.yml/badge.svg)](https://github.com/lammertb/libfins/actions/workflows/cmake-macos.yml)

# Libfins - Multi platform MIT licensed FINS library in C
Libfins is a multi platform library with routines to communicate with Omron PLCs
over the FINS protocol. FINS is an inter-PLC protocol which allows hardware layer independent exchange of
data between PLCs. Omron PLCs support the exchange of data over serial lines, TCP, UDP and bus systems.
This library currently implements the FINS/TCP communication.

FINS is a client server protocol where a client sends requests to a remote server and processes their
answer. The libfins library contains almost all of the publically documented functionality. The only
functionality currently missing is using a PLC as gateway to execute Compoway/F and Modbus commands
over SCU serial units. These features will be implemented and tested in the near future though.

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
and FreeBSD in 32 and 64 bit and on x86 and AMD architectures. New versions of the code are
regularly compiled and checked on the systems mentioned in the following lists.

### 32 bit target environments
|Operating System|Compiler|Build System|
| :--- | :--- | :--- |
|Centos 6.8|gcc 4.4.7|GNU make|
|Debian 8.6|gcc 4.9.2|GNU make|
|FreeBSD 10.3|clang 3.4.1|GNU make|
|Raspbian|gcc 4.8|GNU make|
|Windows 11 10.0.22621|MSVC 19.33.31630|MSBuild|

### 64 bit target environments
|Operating system|Compiler|Build System|
| :--- | :--- | :--- |
|Centos 6.8|gcc 4.4.7|GNU make|
|Centos 7.2.1511|gcc 4.8.5|GNU make|
|Debian 8.6|gcc 4.9.2|GNU make|
|FreeBSD 10.3|clang 3.4.1|GNU make|
|Ubuntu 20.04.6 LTS|gcc 9.4.0|CMake|
|Ubuntu 22.04.2 LTS|gcc 11.3.0|CMake|
|MacOS 11.7.8|AppleClang 13.0.0.13000029|CMake|
|MacOS 12.6.7|AppleClang 14.0.0.14000029|CMake|
|MacOS 13.4|AppleClang 14.0.0.14000029|CMake|
|Windows Server 2019 10.0.17763|MSVC 19.29.30151.0|CMake|
|Windows Server 2022 10.0.20348|MSVC 19.35.32217.1|CMake|
|Windows 11 10.0.22621|MSVC 19.33.31630|MSBuild|
