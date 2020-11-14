/**********
Copyright (c) 2018, Xilinx, Inc.
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
**********/

#include "kernel.h"

// -------------------------------------------------
// Kernel: KCalc
// -------------------------------------------------
extern "C" {
void KCalc(int *A, int *B, int *R)
{
	#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
	#pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
	#pragma HLS INTERFACE m_axi port=R offset=slave bundle=gmem
	#pragma HLS INTERFACE s_axilite port=A bundle=control
	#pragma HLS INTERFACE s_axilite port=B bundle=control
	#pragma HLS INTERFACE s_axilite port=R bundle=control
	#pragma HLS INTERFACE s_axilite port=return bundle=control

	int val1, val2;
    int TMP_R[SIZE_RES];

	int TMP_A[SIZE_RES], TMP_B[SIZE_RES];
	for (int i=0; i<SIZE_RES; i++) { 
		#pragma HLS PIPELINE
		TMP_A[i] = A[i]; TMP_B[i] = B[i]; 
	}


	for (int i=0; i<SIZE_RES; i++) {
		val1 = (TMP_A[i] - TMP_B[i]) * (TMP_A[i] + TMP_B[i]);

		if (val1 >= 0)
		    val2 = val1 % 3;
		else
			val2 = (val1 % 6) * val1;

		TMP_R[i] = val2;
	}

	for (int i=0; i<SIZE_RES; i++) {
		#pragma HLS PIPELINE
		R[i] = TMP_R[i];
	}
}
}