/*
 * Library: libfins
 * File:    src/fins_model_list.c
 * Author:  Lammert Bies
 *
 * This file is licensed under the MIT License as stated below
 *
 * Copyright (c) 2019 Lammert Bies
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
 * The source file src/fins_model_list.c contains a list of Omron CPU models
 * and specifications important for proper FINS communications.
 */

#include "fins.h"

/*
 * struct fins_mcap_tp fins_model[];
 *
 * The array fins_model[] contains a list of known Omron PLC models and
 * their specifications.
 */

struct fins_mcap_tp fins_model[] = {
/*        version           FINS mode          PA-size   EX-banks	*/

	{ "CJ1G_CPU42",     FINS_MODE_CS,      10000,    0       },
	{ "CJ1G_CPU43",     FINS_MODE_CS,      20000,    0       },
	{ "CJ1G_CPU44",     FINS_MODE_CS,      30000,    1       },
	{ "CJ1G_CPU45",     FINS_MODE_CS,      60000,    3       },

	{ "CJ1G_CPU42H",    FINS_MODE_CS,      10000,    1       },
	{ "CJ1G_CPU43H",    FINS_MODE_CS,      20000,    1       },
	{ "CJ1G_CPU44H",    FINS_MODE_CS,      30000,    1       },
	{ "CJ1G_CPU45H",    FINS_MODE_CS,      60000,    3       },

	{ "CJ1H_CPU65H",    FINS_MODE_CS,      60000,    3       },
	{ "CJ1H_CPU66H",    FINS_MODE_CS,      120000,   7       },
	{ "CJ1H_CPU67H",    FINS_MODE_CS,      250000,   13      },

	{ "CJ1H_CPU64H-R",  FINS_MODE_CS,      30000,    1       },
	{ "CJ1H_CPU65H-R",  FINS_MODE_CS,      60000,    3       },
	{ "CJ1H_CPU66H-R",  FINS_MODE_CS,      120000,   7       },
	{ "CJ1H_CPU67H-R",  FINS_MODE_CS,      250000,   13      },

	{ "CJ1M_CPU11",     FINS_MODE_CS,      5000,     0       },
	{ "CJ1M_CPU12",     FINS_MODE_CS,      10000,    0       },
	{ "CJ1M_CPU13",     FINS_MODE_CS,      20000,    0       },

	{ "CJ1M_CPU21",     FINS_MODE_CS,      5000,     0       },
	{ "CJ1M_CPU22",     FINS_MODE_CS,      10000,    0       },
	{ "CJ1M_CPU23",     FINS_MODE_CS,      20000,    0       },

	{ "CJ2H_CPU64",     FINS_MODE_CS,      50000,    4       },
	{ "CJ2H_CPU65",     FINS_MODE_CS,      100000,   4       },
	{ "CJ2H_CPU66",     FINS_MODE_CS,      150000,   10      },
	{ "CJ2H_CPU67",     FINS_MODE_CS,      250000,   15      },
	{ "CJ2H_CPU68",     FINS_MODE_CS,      400000,   25      },

	{ "CJ2H_CPU64-EIP", FINS_MODE_CS,      50000,    4       },
	{ "CJ2H_CPU65-EIP", FINS_MODE_CS,      100000,   4       },
	{ "CJ2H_CPU66-EIP", FINS_MODE_CS,      150000,   10      },
	{ "CJ2H_CPU67-EIP", FINS_MODE_CS,      250000,   15      },
	{ "CJ2H_CPU68-EIP", FINS_MODE_CS,      400000,   25      },

	{ "CJ2M_CPU11",     FINS_MODE_CS,      5000,     1       },
	{ "CJ2M_CPU12",     FINS_MODE_CS,      10000,    1       },
	{ "CJ2M_CPU13",     FINS_MODE_CS,      20000,    1       },
	{ "CJ2M_CPU14",     FINS_MODE_CS,      30000,    4       },
	{ "CJ2M_CPU15",     FINS_MODE_CS,      60000,    4       },

	{ "CJ2M_CPU31",     FINS_MODE_CS,      5000,     1       },
	{ "CJ2M_CPU32",     FINS_MODE_CS,      10000,    1       },
	{ "CJ2M_CPU33",     FINS_MODE_CS,      20000,    1       },
	{ "CJ2M_CPU34",     FINS_MODE_CS,      30000,    4       },
	{ "CJ2M_CPU35",     FINS_MODE_CS,      60000,    4       },

	{ "CP1E_E10DR-A",   FINS_MODE_CS,      2000,     0       },
	{ "CP1E_E14DR-A",   FINS_MODE_CS,      2000,     0       },
	{ "CP1E_E20DR-A",   FINS_MODE_CS,      2000,     0       },
	{ "CP1E_E30DR-A",   FINS_MODE_CS,      2000,     0       },
	{ "CP1E_E40DR-A",   FINS_MODE_CS,      2000,     0       },
	{ "CP1E_E60SDR-A",  FINS_MODE_CS,      2000,     0       },

	{ "CP1E_N14DT-A",   FINS_MODE_CS,      8000,     0       },
	{ "CP1E_N20DT-A",   FINS_MODE_CS,      8000,     0       },
	{ "CP1E_N30DT-A",   FINS_MODE_CS,      8000,     0       },
	{ "CP1E_N40DT-A",   FINS_MODE_CS,      8000,     0       },
	{ "CP1E_N60DT-A",   FINS_MODE_CS,      8000,     0       },

	{ "CP1E_NA20DT-D",  FINS_MODE_CS,      8000,     0       },

	{ "CP1H_X40DR-A",   FINS_MODE_CS,      20000,    0       },
	{ "CP1H_XA40DR-A",  FINS_MODE_CS,      20000,    0       },
	{ "CP1H_Y200DT-D",  FINS_MODE_CS,      20000,    0       },

	{ "CP1L_EL20DT1-D", FINS_MODE_CS,      5000,     0       },
	{ "CP1L_EM40DT1-D", FINS_MODE_CS,      10000,    0       },

	{ "CP1L_J20DT1-D",  FINS_MODE_CS,      1000,     0       },

	{ "CP1L_L10DT1-D",  FINS_MODE_CS,      5000,     0       },
	{ "CP1L_L20DT1-D",  FINS_MODE_CS,      5000,     0       },

	{ "CP1L_M40DT1-D",  FINS_MODE_CS,      10000,    0       },

	{ "CS1D_CPU65H",    FINS_MODE_CS,      60000,    3       },
	{ "CS1D_CPU67H",    FINS_MODE_CS,      250000,   13      },

	{ "CS1D_CPU42S",    FINS_MODE_CS,      10000,    1       },
	{ "CS1D_CPU44S",    FINS_MODE_CS,      30000,    1       },
 
	{ "CS1D_CPU65S",    FINS_MODE_CS,      60000,    3       },
	{ "CS1D_CPU67S",    FINS_MODE_CS,      250000,   13      },

	{ "CS1G_CPU42",     FINS_MODE_CS,      10000,    0       },
	{ "CS1G_CPU43",     FINS_MODE_CS,      20000,    0       },
	{ "CS1G_CPU44",     FINS_MODE_CS,      30000,    1       },
	{ "CS1G_CPU45",     FINS_MODE_CS,      60000,    3       },

	{ "CS1G_CPU42H",    FINS_MODE_CS,      10000,    1       },
	{ "CS1G_CPU43H",    FINS_MODE_CS,      20000,    1       },
	{ "CS1G_CPU44H",    FINS_MODE_CS,      30000,    1       },
	{ "CS1G_CPU45H",    FINS_MODE_CS,      60000,    3       },

	{ "CS1H_CPU63",     FINS_MODE_CS,      20000,    0       },
	{ "CS1H_CPU64",     FINS_MODE_CS,      30000,    1       },
	{ "CS1H_CPU65",     FINS_MODE_CS,      60000,    3       },
	{ "CS1H_CPU66",     FINS_MODE_CS,      120000,   7       },
	{ "CS1H_CPU67",     FINS_MODE_CS,      250000,   13      },

	{ "CS1H_CPU63H",    FINS_MODE_CS,      20000,    1       },
	{ "CS1H_CPU64H",    FINS_MODE_CS,      30000,    1       },
	{ "CS1H_CPU65H",    FINS_MODE_CS,      60000,    3       },
	{ "CS1H_CPU66H",    FINS_MODE_CS,      120000,   7       },
	{ "CS1H_CPU67H",    FINS_MODE_CS,      250000,   13      },

	{ "NJ501",          FINS_MODE_UNKNOWN, 20000000, 0       },

	{ "NSJ8_TV01-M3D",  FINS_MODE_CS,      20000,    0       },
	{ "NSJ12_TV01-G5D", FINS_MODE_CS,      60000,    3       },

	{ NULL,             FINS_MODE_UNKNOWN, 0,        0       }

};  /* fins_mcap */
