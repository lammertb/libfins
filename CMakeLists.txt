cmake_minimum_required(VERSION 3.5)
#INCLUDE(CheckLibraryExists)
#INCLUDE(CheckFunctionExists)

PROJECT(fins VERSION 1.0.0 LANGUAGES C)

ADD_DEFINITIONS(-Wall
	-Wextra
	-Wstrict-prototypes
	-Wshadow
	-Wpointer-arith
	-Wcast-qual
	-Wcast-align
	-Wwrite-strings
	-Wnested-externs
	-Wredundant-decls
	-Werror
	-O3
	-funsigned-char)

INCLUDE(GNUInstallDirs)

SET(SOURCES src/fins_01_01_bcd16.c src/fins_01_01_bcd32.c src/fins_01_01_bit.c src/fins_01_01.c src/fins_01_01_int16.c src/fins_01_01_int32.c src/fins_01_02_bcd16.c src/fins_01_02_bcd32.c src/fins_01_02_bit.c src/fins_01_02.c src/fins_01_02_int16.c src/fins_01_02_int32.c src/fins_01_03.c src/fins_01_04.c src/fins_01_05.c src/fins_02_01.c src/fins_02_02.c src/fins_02_03.c src/fins_03_06.c src/fins_03_07.c src/fins_03_08.c src/fins_04_01.c src/fins_04_02.c src/fins_04_03.c src/fins_05_01.c src/fins_05_02.c src/fins_06_01.c src/fins_06_20_init.c src/fins_06_20_read.c src/fins_07_01.c src/fins_07_02.c src/fins_09_20_clear.c src/fins_09_20_fal.c src/fins_09_20_read.c src/fins_0c_01.c src/fins_0c_02.c src/fins_0c_03.c src/fins_21_01.c src/fins_21_02.c src/fins_21_03.c src/fins_21_40.c src/fins_21_41.c src/fins_22_01.c src/fins_22_02.c src/fins_22_03.c src/fins_22_04.c src/fins_22_05.c src/fins_22_07.c src/fins_22_08.c src/fins_22_0a.c src/fins_22_0b.c src/fins_22_0c.c src/fins_22_15.c src/fins_23_01.c src/fins_23_02.c src/fins_26_01.c src/fins_26_02.c src/fins_26_03.c src/fins_decode.c src/fins_error.c src/fins_init.c src/fins_io.c src/fins_model_list.c src/fins_raw.c src/fins_search.c src/fins_utils.c )

ADD_LIBRARY(${PROJECT_NAME} SHARED ${SOURCES})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION 1)

ADD_LIBRARY(${PROJECT_NAME}-static STATIC ${SOURCES})
SET_TARGET_PROPERTIES(${PROJECT_NAME}-static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

SET(LIBS)

FILE(GLOB HEADERS include/*.h)
INSTALL(FILES ${HEADERS}
	DESTINATION include/
)
INSTALL(TARGETS fins fins-static
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)

export(TARGETS ${PROJECT_NAME} FILE LibFinsConfig.cmake)