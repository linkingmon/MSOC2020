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
#include <stdlib.h>
#include <math.h>
#include "cordic_defines.h"

// 2^32: 4294967296
//#define AMP     (0xFFFFFFFF-1)
#define AMP 32768
// 2^24
//#define MAXNUM  1024
////(16777216) 
////#define MAXNUM  (AMP) 




int main(void)
{

	int ret_val = 0;
	float rel_err;
	unsigned long int i, j;
	int max_err, tot_err;

	dinp_t x, y;
	
	dinp_t real[NC];
    dinp_t imag[NC];
	dout_t magn[NC];
	dout_t ref[NC];

	int d;

	FILE *fil1 = fopen("cordic_isqrt.txt", "wt");
	if(!fil1)
		printf("ERROR: could not open output file1\n");


	for (i=0; i<NC; i++)
	{
		x = floor( (((float)rand())/RAND_MAX) * AMP); // real part
		y = floor( (((float)rand())/RAND_MAX) * AMP); // imaginary part
		if (x > AMP) x = AMP; // clipping
		if (y > AMP) y = AMP; // clipping
		real[i] = x;
		imag[i] = y;
		ref[i]  = process_magnitude_ref(real[i],imag[i]); // reference magnitude result
	}


	// CALL DESIGN UNDER TEST
	printf("running Design Under Test\n");
	top_process_magnitude(real, imag, magn);

	// CHECK RESULTS
	printf("check results\n");
	tot_err = 0;
	max_err = 0;
	for (j=0; j<NC; j++)
	{
		d = (magn[j] - ref[j]);
		d = ABS(d);
		tot_err += d;
		if (d >= max_err) 
		{
			max_err = d;
			i = j; // store the address of max error
		}
		fprintf(fil1, "%10d %10d %10d %10d %10d %10d\n", j, (int) real[j], (int) imag[j], (int) magn[j], (int) ref[j], d);
	}

	// SIMULATION RESULT
	if (max_err > THRESHOLD)
	{
		ret_val = 1;
		printf("Test failed !\n\n");
	}
	else
	{
		ret_val = 0;
		printf("Test successful!\n\n");
	}


	fclose(fil1);
		rel_err = ((float) tot_err)/NC; // relative error
		printf("RELATIVE ERROR = %6.4f\n",rel_err);
		if (max_err > 0)
			printf("MAX ERROR = %d in sqrt(%6d^2 + %6d^2): EXPECTED=%6d, EFFECTIVE=%6d\n", max_err, (int) real[i], (int) imag[i], (int) ref[i], (int) magn[i]);

	return ret_val;

}


