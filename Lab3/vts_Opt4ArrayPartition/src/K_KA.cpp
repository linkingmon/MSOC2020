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

#define USE_BURST_TRANSFER_ARRAY_PARTITION

// -------------------------------------------------
// Kernel: KA
// -------------------------------------------------
extern "C" {
void KA(int *A, int *R)
{
	#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
	#pragma HLS INTERFACE m_axi port=R offset=slave bundle=gmem
	#pragma HLS INTERFACE s_axilite port=A bundle=control
	#pragma HLS INTERFACE s_axilite port=R bundle=control
	#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifndef USE_BURST_TRANSFER_ARRAY_PARTITION
#else
	int TMP_A[SIZE_DataIn_1];

	#pragma HLS array_partition variable=TMP_A cyclic factor=4 dim=1

	for (int i=0; i<SIZE_DataIn_1; i++) TMP_A[i]= A[i];
#endif

	loop_st_1:
	for (int i=0; i<SIZE_BUF_KA; i++) {
#ifndef USE_BURST_TRANSFER_ARRAY_PARTITION
		R[i] = A[2*i]*3 + A[2*i+1]*5 + A[2*i+2]*7 + A[2*i+3]*9;
#else
		R[i] = TMP_A[2*i]*3 + TMP_A[2*i+1]*5 + TMP_A[2*i+2]*7 + TMP_A[2*i+3]*9;
#endif
	}

}
}

