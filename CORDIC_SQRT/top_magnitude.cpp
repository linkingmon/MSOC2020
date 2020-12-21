/************************************************
Copyright (c) 2016, Xilinx, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors 
may be used to endorse or promote products derived from this software 
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
************************************************/

#include <stdio.h>
#include <math.h>
#include "cordic_defines.h"



void func1_process_magnitude(dinp_t real_data[NC], dinp_t imag_data[NC], dout_t magn_data[NC])
{

	unsigned short int i;

	L1:for (i=0; i<NC/4; i++)
	{

#if  defined(SQRT_DOUBLE) 	// SOLUTION 0
		magn_data[i] = process_magnitude_double(    real_data[i],  imag_data[i]);
#elif  defined(SQRT_REF)	// SOLUTION 1
		magn_data[i] = process_magnitude_ref(    real_data[i],  imag_data[i]);
#elif  defined(SQRT_FLOAT)	// SOLUTION 2
		magn_data[i] = process_magnitude_float(  real_data[i],  imag_data[i]);
#elif defined(SQRT_CORDIC)	// SOLUTION 3
		magn_data[i] = process_magnitude_cordic( real_data[i],  imag_data[i]);
//#elif defined(SQRT_INT)		// SOLUTION 4
//		magn_data[i] = process_magnitude_int(    real_data[i],  imag_data[i]);
#endif
	}

}



void func2_process_magnitude(dinp_t real_data[NC], dinp_t imag_data[NC], dout_t magn_data[NC])
{

	unsigned short int i;

	L2: for (i=NC/4; i<NC/2; i++)
	{
#if  defined(SQRT_DOUBLE) 	// SOLUTION 0
		magn_data[i] = process_magnitude_double(    real_data[i],  imag_data[i]);
#elif  defined(SQRT_REF)	// SOLUTION 1
		magn_data[i] = process_magnitude_ref(    real_data[i],  imag_data[i]);
#elif  defined(SQRT_FLOAT)	// SOLUTION 2
		magn_data[i] = process_magnitude_float(  real_data[i],  imag_data[i]);
#elif defined(SQRT_CORDIC)	// SOLUTION 3
		magn_data[i] = process_magnitude_cordic( real_data[i],  imag_data[i]);
//#elif defined(SQRT_INT)		// SOLUTION 4
//		magn_data[i] = process_magnitude_int(    real_data[i],  imag_data[i]);
#endif
		}

}


void func3_process_magnitude(dinp_t real_data[NC], dinp_t imag_data[NC], dout_t magn_data[NC])
{

	unsigned short int i;

	L3: for (i=NC/2; i<3*NC/4; i++)
	{
#if  defined(SQRT_DOUBLE) 	// SOLUTION 0
		magn_data[i] = process_magnitude_double(    real_data[i],  imag_data[i]);
#elif  defined(SQRT_REF)	// SOLUTION 1
		magn_data[i] = process_magnitude_ref(    real_data[i],  imag_data[i]);
#elif  defined(SQRT_FLOAT)	// SOLUTION 2
		magn_data[i] = process_magnitude_float(  real_data[i],  imag_data[i]);
#elif defined(SQRT_CORDIC)	// SOLUTION 3
		magn_data[i] = process_magnitude_cordic( real_data[i],  imag_data[i]);
//#elif defined(SQRT_INT)		// SOLUTION 4
//		magn_data[i] = process_magnitude_int(    real_data[i],  imag_data[i]);
#endif
		}

}


void func4_process_magnitude(dinp_t real_data[NC], dinp_t imag_data[NC], dout_t magn_data[NC])
{

	unsigned short int i;

	L4: for (i=3*NC/4; i<NC; i++)
	{
#if  defined(SQRT_DOUBLE) 	// SOLUTION 0
		magn_data[i] = process_magnitude_double(    real_data[i],  imag_data[i]);
#elif  defined(SQRT_REF)	// SOLUTION 1
		magn_data[i] = process_magnitude_ref(    real_data[i],  imag_data[i]);
#elif  defined(SQRT_FLOAT)	// SOLUTION 2
		magn_data[i] = process_magnitude_float(  real_data[i],  imag_data[i]);
#elif defined(SQRT_CORDIC)	// SOLUTION 3
		magn_data[i] = process_magnitude_cordic( real_data[i],  imag_data[i]);
//#elif defined(SQRT_INT)		// SOLUTION 4
//		magn_data[i] = process_magnitude_int(    real_data[i],  imag_data[i]);
#endif
		}

}

void func_process_magnitude(dinp_t real_data[NC], dinp_t imag_data[NC], dout_t magn_data[NC])
{

	unsigned short int i;

	L: for (i=0; i<NC; i++)
	{
#if  defined(SQRT_DOUBLE) 	// SOLUTION 0
		magn_data[i] = process_magnitude_double(    real_data[i],  imag_data[i]);
#elif  defined(SQRT_REF)	// SOLUTION 1
		magn_data[i] = process_magnitude_ref(    real_data[i],  imag_data[i]);
#elif  defined(SQRT_FLOAT)	// SOLUTION 2
		magn_data[i] = process_magnitude_float(  real_data[i],  imag_data[i]);
#elif defined(SQRT_CORDIC)	// SOLUTION 3
		magn_data[i] = process_magnitude_cordic( real_data[i],  imag_data[i]);
//#elif defined(SQRT_INT)		// SOLUTION 4
//		magn_data[i] = process_magnitude_int(    real_data[i],  imag_data[i]);
#endif
		}

}


void top_process_magnitude(dinp_t real_data[NC], dinp_t imag_data[NC], dout_t magn_data[NC])
{

	unsigned short int i;

#if  defined(SQRT_DOUBLE)  	  // SOLUTION 0
	printf("now running 64-bit floating point method\n");
#elif  defined(SQRT_REF)	  // SOLUTION 1
	printf("now running 32-bit floating point method\n");
#elif  defined(SQRT_FLOAT)	  // SOLUTION 2
	printf("now running 32-bit floating point sqrt on 32-bit integers\n");
//#elif defined(SQRT_INT)
//	printf("now running int cordic approssimation method\n");
#elif ( defined(SQRT_CORDIC) & (!defined(SQRT_AP_INT)) )	  // SOLUTION 3
	printf("now running full cordic 32-bit approssimation method\n");
#elif ( defined(SQRT_CORDIC) & defined(SQRT_AP_INT) )	  // SOLUTION 4
	printf("now running full cordic 18-bit approssimation method\n");
#else
#error < YOU MUST SELECT A SQRT METHOD IN COMPILATION !>
#endif

#if  defined(ONE_loop)  	  // SOLUTION 0
 func_process_magnitude( real_data, imag_data, magn_data);
#else
 func1_process_magnitude( real_data, imag_data, magn_data);
 func2_process_magnitude( real_data, imag_data, magn_data);
 func3_process_magnitude( real_data, imag_data, magn_data);
 func4_process_magnitude( real_data, imag_data, magn_data);
#endif
 
    return;
}
