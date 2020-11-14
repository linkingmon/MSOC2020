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
// Kernel: KB
// -------------------------------------------------
extern "C" {
void KB(int *A, int *R)
{
	#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
	#pragma HLS INTERFACE m_axi port=R offset=slave bundle=gmem
	#pragma HLS INTERFACE s_axilite port=A bundle=control
	#pragma HLS INTERFACE s_axilite port=R bundle=control
	#pragma HLS INTERFACE s_axilite port=return bundle=control

	int val;

	int TMP_BUF[SIZE_BUF_KpB];

	for (int i=0; i<SIZE_BUF_KpB; i++) {
		TMP_BUF[i] = A[i]+10;
	}

	for (int i=0; i<SIZE_BUF_KB; i++) {
		val  =  TMP_BUF[i]*3 + TMP_BUF[i+SIZE_RES]*5 + TMP_BUF[i+2*SIZE_RES]*7;
		R[i] = val;
	}

}
}
