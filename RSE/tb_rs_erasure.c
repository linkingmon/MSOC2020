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
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.// Copyright (c) 2015 Xilinx, Inc.
************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "rs_erasure.h"


int main(void){

	int k, i;
	char test_pass;

	FILE* fp=NULL;
	fp=fopen("tv_rs_erasure_in.txt", "rt");
	if(NULL==fp){
		printf("Error Opening Input Test Vector!\n");
		return(1);
	}
	
	FILE* fp_gold=NULL;
	fp_gold=fopen("tv_rs_erasure_mout.txt", "rt");
	if(NULL==fp_gold){
		printf("Error Opening Golden Output Test Vector!\n");
		return(1);
	}
	
	int a[NUM_TAPS+2]; // 2 is survival pattern and code id
	
	int fileend=0;
	
	unsigned char c[NUM_EQUATION];
	unsigned char d[NUM_TAPS];

	unsigned short survival_pattern;
	unsigned char  codeid;

	int errcnt       = 0;
	int linecnt      = 0;
	int suberrcnt[NUM_CODES];
	int sublincnt[NUM_CODES];
	
	
	for(i=0;i<NUM_EQUATION;i++) suberrcnt[i]=0;
	for(i=0;i<NUM_EQUATION;i++) sublincnt[i]=0;
	
	
	while(1){
		
		// read the data in
		// each line is codeid, survival pattern, data(1:12)
		for(k=0;k<(NUM_TAPS+2);k++){
			 int tmp;
			 int scnt = fscanf(fp, "%d", &tmp);
			 a[k] = tmp;

			 if (scnt < 0) {
				 fileend = 1;
				 break;
			 }
		}
		
		// quit the loop at the file end
		if(fileend) break;
		linecnt++;
		
		// process the data
		codeid = a[0]&3;
		survival_pattern = a[1]&((1<<RSCODE_LEN)-1);
		for(k=0;k<12;k++) d[k]=a[2+k]&((1<<GF_ORDER)-1);
		
		rs_erasure(c, d, survival_pattern, codeid);

		sublincnt[codeid]++;
		
		// read data from golden test vector
		unsigned char this_is_err=0;
		
		for (k = 0;k < NUM_EQUATION; k++) {
				int tmp;
				fscanf(fp_gold, "%d", &tmp);
				if (c[k] != tmp){
					this_is_err=1;
					// print 5 errors for each codeid
					if(suberrcnt[codeid]<=5)
						printf("[Err %3d] line %d\n", errcnt, linecnt);
				}
		}
		
		if(this_is_err){
			suberrcnt[codeid]++;
			errcnt++;
		}	
					
	}
	
	fclose(fp);
	fclose(fp_gold);

	for(i=0;i<NUM_CODES;i++){
			printf("\t[%d] %3d out of %3d test vectors failed.\n", i, suberrcnt[i], sublincnt[i]);
	}
	printf("Total %d Test Vectors, Err Count = %d.\n", linecnt, errcnt);
	if (errcnt) printf("Test Failed!\n");
	else printf("Test Passed!\n");

	return(errcnt);
	

}

