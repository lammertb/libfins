#
# Library: libfins
# File:    Makefile
# Author:  Lammert Bies
#
# This file is licensed under the MIT License as stated below
#
# Copyright (c) 2016 Lammert Bies
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Description
# -----------
# This Makefile is used to build the libfins library. The only action you should
# normally have to do is to run the make program from the command line,
# independent on the Operating System you are working on.
#
# This Makefile is manually maintained. No autoconf or automake involved. This
# was a deliberate decision due to the absense of these tools on some systems,
# in particular in Windows environments.
#
# Dependencies
# ------------
# This Makefile is known to be functional with GNU Make. Other make utilities
# may work or may have problems. GNU Make is available both in source code
# and precompiled binaries for a large number of environments and therefore
# you are recommended to install a version of GNU Make if you encounter
# problems with the default make utility in your development chain.
#
# Aside from GNU Make and the standard C compiler headers and libraries which
# should have been installed already together with your compiler there are no
# other known dependencies.
#
# Library Type
# ------------
# The generated library is a library useable for static linking in this
# source directory structure. The decision for a static linkable library
# was deliberate because of the relatively small size of the library and the
# routines and to avoid version and dependency issues when distributing the
# end application to different environments.
#

ifneq ($(OS),Windows_NT)
OS:=$(shell uname -s)
endif

INCDIR = include/
LIBDIR = lib/
OBJDIR = obj/
SRCDIR = src/
CC     = cc
RM     = /bin/rm -f
OBJEXT = o
LIBEXT = a
OFLAG  = -o
AR     = ar
ARQC   = qc 
ARQ    = q
RANLIB = ranlib

CFLAGS=	-Wall \
	-Wextra \
	-Wstrict-prototypes \
	-Wshadow \
	-Wpointer-arith \
	-Wcast-qual \
	-Wcast-align \
	-Wwrite-strings \
	-Wnested-externs \
	-Wredundant-decls \
	-Werror \
	-O3 \
	-funsigned-char \
	-I${INCDIR}

ifeq ($(OS),Windows_NT)
INCDIR = include\\
LIBDIR = lib\\
OBJDIR = obj\\
SRCDIR = src\\
CC     = cl
RM     = del /q
OBJEXT = obj
LIBEXT = lib
OFLAG  = -Fo
AR     = lib
ARQC   = /NOLOGO /OUT:
ARQ    = /NOLOGO
RANLIB = dir

CFLAGS = -Ox -Ot -MT -GT -volatile:iso -I${INCDIR} -nologo -J -sdl -Wall -WX -wd4464 -wd4710 -wd4711 -wd4201 -wd4820
endif

${OBJDIR}%.${OBJEXT} : ${SRCDIR}%.c
	${CC} -c ${CPPFLAGS} ${CFLAGS} ${OFLAG}$@ $<

all: ${LIBDIR}libfins.${LIBEXT}

clean:
	${RM} ${OBJDIR}*.${OBJEXT}
	${RM} ${LIBDIR}libfins.${LIBEXT}

${LIBDIR}libfins.${LIBEXT}:				\
		${OBJDIR}fins_01_01.${OBJEXT}		\
		${OBJDIR}fins_01_01_bcd16.${OBJEXT}	\
		${OBJDIR}fins_01_01_bcd32.${OBJEXT}	\
		${OBJDIR}fins_01_01_bit.${OBJEXT}	\
		${OBJDIR}fins_01_01_int16.${OBJEXT}	\
		${OBJDIR}fins_01_01_int32.${OBJEXT}	\
		${OBJDIR}fins_01_02.${OBJEXT}		\
		${OBJDIR}fins_01_02_bcd16.${OBJEXT}	\
		${OBJDIR}fins_01_02_bcd32.${OBJEXT}	\
		${OBJDIR}fins_01_02_bit.${OBJEXT}	\
		${OBJDIR}fins_01_02_int16.${OBJEXT}	\
		${OBJDIR}fins_01_02_int32.${OBJEXT}	\
		${OBJDIR}fins_01_03.${OBJEXT}		\
		${OBJDIR}fins_01_04.${OBJEXT}		\
		${OBJDIR}fins_01_05.${OBJEXT}		\
		${OBJDIR}fins_02_01.${OBJEXT}		\
		${OBJDIR}fins_02_02.${OBJEXT}		\
		${OBJDIR}fins_02_03.${OBJEXT}		\
		${OBJDIR}fins_03_06.${OBJEXT}		\
		${OBJDIR}fins_03_07.${OBJEXT}		\
		${OBJDIR}fins_03_08.${OBJEXT}		\
		${OBJDIR}fins_04_01.${OBJEXT}		\
		${OBJDIR}fins_04_02.${OBJEXT}		\
		${OBJDIR}fins_05_01.${OBJEXT}		\
		${OBJDIR}fins_05_02.${OBJEXT}		\
		${OBJDIR}fins_06_01.${OBJEXT}		\
		${OBJDIR}fins_06_20_init.${OBJEXT}	\
		${OBJDIR}fins_06_20_read.${OBJEXT}	\
		${OBJDIR}fins_07_01.${OBJEXT}		\
		${OBJDIR}fins_07_02.${OBJEXT}		\
		${OBJDIR}fins_09_20_clear.${OBJEXT}	\
		${OBJDIR}fins_09_20_fal.${OBJEXT}	\
		${OBJDIR}fins_09_20_read.${OBJEXT}	\
		${OBJDIR}fins_0c_01.${OBJEXT}		\
		${OBJDIR}fins_0c_02.${OBJEXT}		\
		${OBJDIR}fins_0c_03.${OBJEXT}		\
		${OBJDIR}fins_21_01.${OBJEXT}		\
		${OBJDIR}fins_21_02.${OBJEXT}		\
		${OBJDIR}fins_21_03.${OBJEXT}		\
		${OBJDIR}fins_21_40.${OBJEXT}		\
		${OBJDIR}fins_21_41.${OBJEXT}		\
		${OBJDIR}fins_22_01.${OBJEXT}		\
		${OBJDIR}fins_22_02.${OBJEXT}		\
		${OBJDIR}fins_22_03.${OBJEXT}		\
		${OBJDIR}fins_22_04.${OBJEXT}		\
		${OBJDIR}fins_22_05.${OBJEXT}		\
		${OBJDIR}fins_22_07.${OBJEXT}		\
		${OBJDIR}fins_22_08.${OBJEXT}		\
		${OBJDIR}fins_22_0a.${OBJEXT}		\
		${OBJDIR}fins_22_0b.${OBJEXT}		\
		${OBJDIR}fins_22_0c.${OBJEXT}		\
		${OBJDIR}fins_22_15.${OBJEXT}		\
		${OBJDIR}fins_23_01.${OBJEXT}		\
		${OBJDIR}fins_23_02.${OBJEXT}		\
		${OBJDIR}fins_decode.${OBJEXT}		\
		${OBJDIR}fins_error.${OBJEXT}		\
		${OBJDIR}fins_init.${OBJEXT}		\
		${OBJDIR}fins_io.${OBJEXT}		\
		${OBJDIR}fins_raw.${OBJEXT}		\
		${OBJDIR}fins_search.${OBJEXT}		\
		${OBJDIR}fins_utils.${OBJEXT}		\
		Makefile
	${RM}	${LIBDIR}libfins.${LIBEXT}
	${AR}	${ARQC}${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_01_01.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_01_01_bcd16.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_01_01_bcd32.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_01_01_bit.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_01_01_int16.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_01_01_int32.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_01_02.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_01_02_bcd16.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_01_02_bcd32.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_01_02_bit.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_01_02_int16.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_01_02_int32.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_01_03.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_01_04.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_01_05.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_02_01.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_02_02.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_02_03.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_03_06.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_03_07.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_03_08.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_04_01.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_04_02.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_05_01.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_05_02.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_06_01.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_06_20_init.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_06_20_read.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_07_01.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_07_02.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_09_20_clear.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_09_20_fal.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_09_20_read.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_0c_01.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_0c_02.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_0c_03.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_21_01.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_21_02.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_21_03.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_21_40.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_21_41.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_22_01.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_22_02.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_22_03.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_22_04.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_22_05.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_22_07.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_22_08.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_22_0a.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_22_0b.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_22_0c.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_22_15.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_23_01.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_23_02.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_decode.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_error.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_init.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_io.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_raw.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_search.${OBJEXT}
	${AR}	${ARQ}	${LIBDIR}libfins.${LIBEXT}	${OBJDIR}fins_utils.${OBJEXT}
	${RANLIB}	${LIBDIR}libfins.${LIBEXT}

${OBJDIR}fins_01_01.${OBJEXT} :		${SRCDIR}fins_01_01.c ${INCDIR}fins.h

${OBJDIR}fins_01_01_bcd16.${OBJEXT} :	${SRCDIR}fins_01_01_bcd16.c ${INCDIR}fins.h

${OBJDIR}fins_01_01_bcd32.${OBJEXT} :	${SRCDIR}fins_01_01_bcd32.c ${INCDIR}fins.h

${OBJDIR}fins_01_01_bit.${OBJEXT} :	${SRCDIR}fins_01_01_bit.c ${INCDIR}fins.h

${OBJDIR}fins_01_01_int16.${OBJEXT} :	${SRCDIR}fins_01_01_int16.c ${INCDIR}fins.h

${OBJDIR}fins_01_01_int32.${OBJEXT} :	${SRCDIR}fins_01_01_int32.c ${INCDIR}fins.h

${OBJDIR}fins_01_02.${OBJEXT} :		${SRCDIR}fins_01_02.c ${INCDIR}fins.h

${OBJDIR}fins_01_02_bcd16.${OBJEXT} :	${SRCDIR}fins_01_02_bcd16.c ${INCDIR}fins.h

${OBJDIR}fins_01_02_bcd32.${OBJEXT} :	${SRCDIR}fins_01_02_bcd32.c ${INCDIR}fins.h

${OBJDIR}fins_01_02_bit.${OBJEXT} :	${SRCDIR}fins_01_02_bit.c ${INCDIR}fins.h

${OBJDIR}fins_01_02_int16.${OBJEXT} :	${SRCDIR}fins_01_02_int16.c ${INCDIR}fins.h

${OBJDIR}fins_01_02_int32.${OBJEXT} :	${SRCDIR}fins_01_02_int32.c ${INCDIR}fins.h

${OBJDIR}fins_01_03.${OBJEXT} :		${SRCDIR}fins_01_03.c ${INCDIR}fins.h

${OBJDIR}fins_01_04.${OBJEXT} :		${SRCDIR}fins_01_04.c ${INCDIR}fins.h

${OBJDIR}fins_01_05.${OBJEXT} :		${SRCDIR}fins_01_05.c ${INCDIR}fins.h

${OBJDIR}fins_02_01.${OBJEXT} :		${SRCDIR}fins_02_01.c ${INCDIR}fins.h

${OBJDIR}fins_02_02.${OBJEXT} :		${SRCDIR}fins_02_02.c ${INCDIR}fins.h

${OBJDIR}fins_02_03.${OBJEXT} :		${SRCDIR}fins_02_03.c ${INCDIR}fins.h

${OBJDIR}fins_03_06.${OBJEXT} :		${SRCDIR}fins_03_06.c ${INCDIR}fins.h

${OBJDIR}fins_03_07.${OBJEXT} :		${SRCDIR}fins_03_07.c ${INCDIR}fins.h

${OBJDIR}fins_03_08.${OBJEXT} :		${SRCDIR}fins_03_08.c ${INCDIR}fins.h

${OBJDIR}fins_04_01.${OBJEXT} :		${SRCDIR}fins_04_01.c ${INCDIR}fins.h

${OBJDIR}fins_04_02.${OBJEXT} :		${SRCDIR}fins_04_02.c ${INCDIR}fins.h

${OBJDIR}fins_05_01.${OBJEXT} :		${SRCDIR}fins_05_01.c ${INCDIR}fins.h

${OBJDIR}fins_05_02.${OBJEXT} :		${SRCDIR}fins_05_02.c ${INCDIR}fins.h

${OBJDIR}fins_06_01.${OBJEXT} :		${SRCDIR}fins_06_01.c ${INCDIR}fins.h

${OBJDIR}fins_06_20_init.${OBJEXT} :	${SRCDIR}fins_06_20_init.c ${INCDIR}fins.h

${OBJDIR}fins_06_20_read.${OBJEXT} :	${SRCDIR}fins_06_20_read.c ${INCDIR}fins.h

${OBJDIR}fins_07_01.${OBJEXT} :		${SRCDIR}fins_07_01.c ${INCDIR}fins.h

${OBJDIR}fins_07_02.${OBJEXT} :		${SRCDIR}fins_07_02.c ${INCDIR}fins.h

${OBJDIR}fins_09_20_clear.${OBJEXT} :	${SRCDIR}fins_09_20_clear.c ${INCDIR}fins.h

${OBJDIR}fins_09_20_fal.${OBJEXT} :	${SRCDIR}fins_09_20_fal.c ${INCDIR}fins.h

${OBJDIR}fins_09_20_read.${OBJEXT} :	${SRCDIR}fins_09_20_read.c ${INCDIR}fins.h

${OBJDIR}fins_0c_01.${OBJEXT} :		${SRCDIR}fins_0c_01.c ${INCDIR}fins.h

${OBJDIR}fins_0c_02.${OBJEXT} :		${SRCDIR}fins_0c_02.c ${INCDIR}fins.h

${OBJDIR}fins_0c_03.${OBJEXT} :		${SRCDIR}fins_0c_03.c ${INCDIR}fins.h

${OBJDIR}fins_21_01.${OBJEXT} :		${SRCDIR}fins_21_01.c ${INCDIR}fins.h

${OBJDIR}fins_21_02.${OBJEXT} :		${SRCDIR}fins_21_02.c ${INCDIR}fins.h

${OBJDIR}fins_21_03.${OBJEXT} :		${SRCDIR}fins_21_03.c ${INCDIR}fins.h

${OBJDIR}fins_21_40.${OBJEXT} :		${SRCDIR}fins_21_40.c ${INCDIR}fins.h

${OBJDIR}fins_21_41.${OBJEXT} :		${SRCDIR}fins_21_41.c ${INCDIR}fins.h

${OBJDIR}fins_22_01.${OBJEXT} :		${SRCDIR}fins_22_01.c ${INCDIR}fins.h

${OBJDIR}fins_22_02.${OBJEXT} :		${SRCDIR}fins_22_02.c ${INCDIR}fins.h

${OBJDIR}fins_22_03.${OBJEXT} :		${SRCDIR}fins_22_03.c ${INCDIR}fins.h

${OBJDIR}fins_22_04.${OBJEXT} :		${SRCDIR}fins_22_04.c ${INCDIR}fins.h

${OBJDIR}fins_22_05.${OBJEXT} :		${SRCDIR}fins_22_05.c ${INCDIR}fins.h

${OBJDIR}fins_22_07.${OBJEXT} :		${SRCDIR}fins_22_07.c ${INCDIR}fins.h

${OBJDIR}fins_22_08.${OBJEXT} :		${SRCDIR}fins_22_08.c ${INCDIR}fins.h

${OBJDIR}fins_22_0a.${OBJEXT} :		${SRCDIR}fins_22_0a.c ${INCDIR}fins.h

${OBJDIR}fins_22_0b.${OBJEXT} :		${SRCDIR}fins_22_0b.c ${INCDIR}fins.h

${OBJDIR}fins_22_0c.${OBJEXT} :		${SRCDIR}fins_22_0c.c ${INCDIR}fins.h

${OBJDIR}fins_22_15.${OBJEXT} :		${SRCDIR}fins_22_15.c ${INCDIR}fins.h

${OBJDIR}fins_23_01.${OBJEXT} :		${SRCDIR}fins_23_01.c ${INCDIR}fins.h

${OBJDIR}fins_23_02.${OBJEXT} :		${SRCDIR}fins_23_02.c ${INCDIR}fins.h

${OBJDIR}fins_decode.${OBJEXT} :	${SRCDIR}fins_decode.c ${INCDIR}fins.h

${OBJDIR}fins_error.${OBJEXT} :		${SRCDIR}fins_error.c ${INCDIR}fins.h

${OBJDIR}fins_init.${OBJEXT} :		${SRCDIR}fins_init.c ${INCDIR}fins.h

${OBJDIR}fins_io.${OBJEXT} :		${SRCDIR}fins_io.c ${INCDIR}fins.h

${OBJDIR}fins_raw.${OBJEXT} :		${SRCDIR}fins_raw.c ${INCDIR}fins.h

${OBJDIR}fins_search.${OBJEXT} :	${SRCDIR}fins_search.c ${INCDIR}fins.h

${OBJDIR}fins_utils.${OBJEXT} :		${SRCDIR}fins_utils.c ${INCDIR}fins.h
