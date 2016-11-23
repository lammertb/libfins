/*
 * Library: libfins
 * File:    src/fins_search.c
 * Author:  Lammert Bies
 *
 * This file is licensed under the MIT License as stated below
 *
 * Copyright (c) 2016 Lammert Bies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Description
 * -----------
 * The source file src/fins_search.c contains routines to convert a text format
 * PLC address to the data associated with that memory area.
 */

#include <stdio.h>
#include <string.h>
#include "fins.h"

#define BUFLEN		1024

static const struct fins_area_tp fins_area[] = {
	/* plc_mode     name,  bits, length, area, low_id, high_id, low_addr, high_addr, access,                                                      force */
	{ FINS_MODE_CS, "CIO", 1,    1,      0x30,      0,    6143, 0x000000, 0x17FF0F,  FI_RD | FI_WR |           FI_MRD                   | FI_FRC, false },
	{ FINS_MODE_CS, "CIO", 1,    1,      0x70,      0,    6143, 0x000000, 0x17FF0F,                            FI_MRD,                            true  },
	{ FINS_MODE_CS, "CIO", 16,   2,      0xB0,      0,    6143, 0x000000, 0x17FF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "CIO", 16,   4,      0xF0,      0,    6143, 0x000000, 0x17FF00,                            FI_MRD,                            true  },
	{ FINS_MODE_CS, "W",   1,    1,      0x31,      0,     511, 0x000000, 0x01FF0F,  FI_RD | FI_WR           | FI_MRD                   | FI_FRC, false },
	{ FINS_MODE_CS, "W",   1,    1,      0x71,      0,     511, 0x000000, 0x01FF0F,                            FI_MRD,                            true  },
	{ FINS_MODE_CS, "W",   16,   2,      0xB1,      0,     511, 0x000000, 0x01FF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "W",   16,   4,      0xF1,      0,     511, 0x000000, 0x01FF00,                            FI_MRD,                            true  },
	{ FINS_MODE_CS, "H",   1,    1,      0x32,      0,     511, 0x000000, 0x01FF0F,  FI_RD | FI_WR           | FI_MRD                   | FI_FRC, false },
	{ FINS_MODE_CS, "H",   1,    1,      0x72,      0,     511, 0x000000, 0x01FF0F,                            FI_MRD,                            true  },
	{ FINS_MODE_CS, "H",   16,   2,      0xB2,      0,     511, 0x000000, 0x01FF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "H",   16,   4,      0xF2,      0,     511, 0x000000, 0x01FF00,                            FI_MRD,                            true  },
	{ FINS_MODE_CS, "A",   1,    1,      0x33,      0,     959, 0x000000, 0x03BF0F,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "A",   1,    1,      0x33,    448,     959, 0x01C000, 0x03BF0F,          FI_WR,                                               false },
	{ FINS_MODE_CS, "A",   16,   2,      0xB3,      0,     959, 0x000000, 0x03BF00,  FI_RD                   | FI_MRD | FI_TRS,                   false },
	{ FINS_MODE_CS, "A",   16,   2,      0xB3,    448,     959, 0x01C000, 0x03BF00,          FI_WR | FI_FILL                   | FI_TRD,          false },
	{ FINS_MODE_CS, "TIM", 1,    1,      0x09,      0,    4095, 0x000000, 0x0FFF00,  FI_RD                   | FI_MRD                   | FI_FRC, false },
	{ FINS_MODE_CS, "TIM", 1,    1,      0x49,      0,    4095, 0x000000, 0x0FFF00,                            FI_MRD,                            true  },
	{ FINS_MODE_CS, "TIM", 16,   2,      0x89,      0,    4095, 0x000000, 0x0FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "CNT", 1,    1,      0x09,      0,    4095, 0x800000, 0x8FFF00,  FI_RD                   | FI_MRD                   | FI_FRC, false },
	{ FINS_MODE_CS, "CNT", 1,    1,      0x49,      0,    4095, 0x800000, 0x8FFF00,                            FI_MRD,                            true  },
	{ FINS_MODE_CS, "CNT", 16,   2,      0x89,      0,    4095, 0x800000, 0x8FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "DM",  1,    1,      0x02,      0,   32767, 0x000000, 0x7FFF0F,  FI_RD | FI_WR           | FI_MRD,                            false },
	{ FINS_MODE_CS, "DM",  16,   2,      0x82,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "E0_", 1,    1,      0x20,      0,   32767, 0x000000, 0x7FFF0F,  FI_RD | FI_WR           | FI_MRD,                            false },
	{ FINS_MODE_CS, "E1_", 1,    1,      0x21,      0,   32767, 0x000000, 0x7FFF0F,  FI_RD | FI_WR           | FI_MRD,                            false },
	{ FINS_MODE_CS, "E2_", 1,    1,      0x22,      0,   32767, 0x000000, 0x7FFF0F,  FI_RD | FI_WR           | FI_MRD,                            false },
	{ FINS_MODE_CS, "E3_", 1,    1,      0x23,      0,   32767, 0x000000, 0x7FFF0F,  FI_RD | FI_WR           | FI_MRD,                            false },
	{ FINS_MODE_CS, "E4_", 1,    1,      0x24,      0,   32767, 0x000000, 0x7FFF0F,  FI_RD | FI_WR           | FI_MRD,                            false },
	{ FINS_MODE_CS, "E5_", 1,    1,      0x25,      0,   32767, 0x000000, 0x7FFF0F,  FI_RD | FI_WR           | FI_MRD,                            false },
	{ FINS_MODE_CS, "E6_", 1,    1,      0x26,      0,   32767, 0x000000, 0x7FFF0F,  FI_RD | FI_WR           | FI_MRD,                            false },
	{ FINS_MODE_CS, "E7_", 1,    1,      0x27,      0,   32767, 0x000000, 0x7FFF0F,  FI_RD | FI_WR           | FI_MRD,                            false },
	{ FINS_MODE_CS, "E8_", 1,    1,      0x28,      0,   32767, 0x000000, 0x7FFF0F,  FI_RD | FI_WR           | FI_MRD,                            false },
	{ FINS_MODE_CS, "E9_", 1,    1,      0x29,      0,   32767, 0x000000, 0x7FFF0F,  FI_RD | FI_WR           | FI_MRD,                            false },
	{ FINS_MODE_CS, "EA_", 1,    1,      0x2A,      0,   32767, 0x000000, 0x7FFF0F,  FI_RD | FI_WR           | FI_MRD,                            false },
	{ FINS_MODE_CS, "EB_", 1,    1,      0x2B,      0,   32767, 0x000000, 0x7FFF0F,  FI_RD | FI_WR           | FI_MRD,                            false },
	{ FINS_MODE_CS, "EC_", 1,    1,      0x2C,      0,   32767, 0x000000, 0x7FFF0F,  FI_RD | FI_WR           | FI_MRD,                            false },
	{ FINS_MODE_CS, "E0_", 16,   2,      0xA0,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "E1_", 16,   2,      0xA1,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "E2_", 16,   2,      0xA2,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "E3_", 16,   2,      0xA3,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "E4_", 16,   2,      0xA4,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "E5_", 16,   2,      0xA5,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "E6_", 16,   2,      0xA6,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "E7_", 16,   2,      0xA7,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "E8_", 16,   2,      0xA8,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "E9_", 16,   2,      0xA9,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "EA_", 16,   2,      0xAA,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "EB_", 16,   2,      0xAB,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "EC_", 16,   2,      0xAC,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "E",   16,   2,      0x98,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CS, "EM",  16,   2,      0xBC,      0,       0, 0x0F0000, 0x0F0000,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "TK",  1,    1,      0x06,      0,      31, 0x000000, 0x001F00,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "TKS", 1,    1,      0x46,      0,      31, 0x000000, 0x001F00,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "IR",  16,   4,      0xDC,      0,      15, 0x010000, 0x010F00,  FI_RD | FI_WR           | FI_MRD,                            false },
	{ FINS_MODE_CS, "DR",  16,   2,      0xBC,      0,      15, 0x020000, 0x020F00,  FI_RD | FI_WR           | FI_MRD,                            false },
	{ FINS_MODE_CS, "C1M", 1,    1,      0x07,      0,       0, 0x000000, 0x000000,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "C1S", 1,    1,      0x07,      0,       0, 0x000100, 0x000100,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "C02", 1,    1,      0x07,      0,       0, 0x000200, 0x000200,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "C01", 1,    1,      0x07,      0,       0, 0x000300, 0x000300,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "C22", 1,    1,      0x07,      0,       0, 0x000400, 0x000400,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "CER", 1,    1,      0x07,      0,       0, 0x100000, 0x100000,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "CCY", 1,    1,      0x07,      0,       0, 0x100100, 0x100100,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "CGT", 1,    1,      0x07,      0,       0, 0x100200, 0x100200,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "CEQ", 1,    1,      0x07,      0,       0, 0x100300, 0x100300,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "CLT", 1,    1,      0x07,      0,       0, 0x100400, 0x100400,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "NEG", 1,    1,      0x07,      0,       0, 0x100500, 0x100500,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "COF", 1,    1,      0x07,      0,       0, 0x100600, 0x100600,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "CUF", 1,    1,      0x07,      0,       0, 0x100700, 0x100700,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "CGE", 1,    1,      0x07,      0,       0, 0x100800, 0x100800,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "CNE", 1,    1,      0x07,      0,       0, 0x100900, 0x100900,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "CLE", 1,    1,      0x07,      0,       0, 0x100A00, 0x100A00,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "OFF", 1,    1,      0x07,      0,       0, 0x100E00, 0x100E00,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CS, "ON",  1,    1,      0x07,      0,       0, 0x100F00, 0x100F00,  FI_RD                   | FI_MRD,                            false },

	{ FINS_MODE_CV, "CIO", 1,    1,      0x00,      0,    2555, 0x000000, 0x09FB0F,  FI_RD                   | FI_MRD                   | FI_FRC, false },
	{ FINS_MODE_CV, "CIO", 1,    1,      0x40,      0,    2555, 0x000000, 0x09FB0F,                            FI_MRD,                            true  },
	{ FINS_MODE_CV, "CIO", 16,   2,      0x80,      0,    2555, 0x000000, 0x09FB00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CV, "CIO", 16,   2,      0xC0,      0,    2555, 0x000000, 0x09FB00,                            FI_MRD,                            true  },
	{ FINS_MODE_CV, "A",   1,    1,      0x00,      0,     959, 0x0B0000, 0x0EBF0F,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CV, "A",   1,    1,      0x00,    448,     959, 0x0CC000, 0x0EBF0F,  0,                                                           false },
	{ FINS_MODE_CV, "A",   16,   2,      0x80,      0,     959, 0x0B0000, 0x0EBF00,  FI_RD                   | FI_MRD | FI_TRS,                   false },
	{ FINS_MODE_CV, "A",   16,   2,      0x80,    448,     959, 0x0CC000, 0x0EBF00,          FI_WR | FI_FILL                   | FI_TRD,          false },
	{ FINS_MODE_CV, "TIM", 1,    1,      0x01,      0,    2047, 0x000000, 0x07FF00,  FI_RD                   | FI_MRD                   | FI_FRC, false },
	{ FINS_MODE_CV, "TIM", 1,    1,      0x41,      0,    2047, 0x000000, 0x07FF00,                            FI_MRD,                            true  },
	{ FINS_MODE_CV, "TIM", 16,   2,      0x81,      0,    2047, 0x000000, 0x07FF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CV, "CNT", 1,    1,      0x01,      0,    2047, 0x080000, 0x0FFF00,  FI_RD                   | FI_MRD                   | FI_FRC, false },
	{ FINS_MODE_CV, "CNT", 1,    1,      0x41,      0,    2047, 0x080000, 0x0FFF00,                            FI_MRD,                            true  },
	{ FINS_MODE_CV, "CNT", 16,   2,      0x81,      0,    2047, 0x080000, 0x0FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CV, "DM",  16,   2,      0x82,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CV, "E0_", 16,   2,      0x90,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CV, "E1_", 16,   2,      0x91,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CV, "E2_", 16,   2,      0x92,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CV, "E3_", 16,   2,      0x93,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CV, "E4_", 16,   2,      0x94,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CV, "E5_", 16,   2,      0x95,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CV, "E6_", 16,   2,      0x96,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CV, "E7_", 16,   2,      0x97,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CV, "E",   16,   2,      0x98,      0,   32767, 0x000000, 0x7FFF00,  FI_RD | FI_WR | FI_FILL | FI_MRD | FI_TRS | FI_TRD,          false },
	{ FINS_MODE_CV, "EM",  16,   2,      0x9C,      0,       0, 0x000600, 0x000600,  FI_RD                   | FI_MRD,                            false },
	{ FINS_MODE_CV, "DR",  16,   2,      0x9C,      0,       2, 0x000300, 0x000500,  FI_RD | FI_WR           | FI_MRD,                            false },

	{ FINS_MODE_UNKNOWN, NULL, 0, 0,     0x00,      0,       0, 0x000000, 0x000000,  0,                                                           false }
};

/*
 * const struct fins_area_tp *fins_search_area( struct fins_sys_tp *sys, const char *start, int bits, uint32_t accs, bool force );
 *
 * The function fins_search_area() returns a pointer to an area which matches
 * the parameters, or NULL if no such area could be found.
 */

const struct fins_area_tp *fins_search_area( struct fins_sys_tp *sys, const struct fins_address_tp *address, int bits, uint32_t accs, bool force ) {

	int a;

	a = 0;

	while ( fins_area[a].plc_mode != FINS_MODE_UNKNOWN ) {

		if (   fins_area[a].plc_mode           != sys->plc_mode         ) { a++; continue; }
		if (   fins_area[a].bits               != bits                  ) { a++; continue; }
		if ( ( fins_area[a].access | accs )    == 0x00000000            ) { a++; continue; }
		if (   fins_area[a].force              != force                 ) { a++; continue; }
		if (   fins_area[a].low_id             >  address->main_address ) { a++; continue; }
		if (   fins_area[a].high_id            <  address->main_address ) { a++; continue; }
		if (   strcmp( fins_area[a].name, address->name )               ) { a++; continue; }

		break;
	}

	if ( fins_area[a].plc_mode == FINS_MODE_UNKNOWN ) return NULL;

	return & fins_area[a];

}  /* fins_search_area */
