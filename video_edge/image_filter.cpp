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

#include "image_filter.h"

//______________________________________________________________________________

void image_filter(AXI_STREAM& INPUT_STREAM, AXI_STREAM& OUTPUT_STREAM) {

#pragma HLS INTERFACE axis port=INPUT_STREAM bundle=VIDEO_IN
#pragma HLS INTERFACE axis port=OUTPUT_STREAM bundle=VIDEO_OUT
#pragma HLS INTERFACE s_axilite port=return   bundle=CONTROL_BUS

//#pragma HLS INTERFACE s_axilite port=rows     bundle=CONTROL_BUS //offset=0x14
//#pragma HLS INTERFACE s_axilite port=cols     bundle=CONTROL_BUS //offset=0x1C

#pragma HLS dataflow
  
//assert(rows <= MAX_HEIGHT);
//assert(cols <= MAX_WIDTH);
const int rows = MAX_HEIGHT;
const int cols = MAX_WIDTH;
#pragma HLS INTERFACE ap_stable port=rows
#pragma HLS INTERFACE ap_stable port=cols

RGB_IMAGE  img_0  (rows, cols);
RGB_IMAGE  img_1a (rows, cols);
RGB_IMAGE  img_1b (rows, cols);
RGB_IMAGE  img_2a (rows, cols);
RGB_IMAGE  img_2b (rows, cols);
RGB_IMAGE  img_3  (rows, cols);

// Convert AXI4 Stream data to hls::mat format
myAXIvideo2Mat<32, 220, 220, HLS_8UC3>(INPUT_STREAM, img_0);

// copy to 2 channels
replicate_by2<RGB_IMAGE,RGB_PIXEL>(img_0, img_1a, img_1b, rows, cols);

// gradient
grad_vertical<RGB_IMAGE>(img_1a, img_2a, rows, cols);
grad_horizontal<RGB_IMAGE>(img_1b, img_2b, rows, cols);

// combine
// combine2<RGB_IMAGE,RGB_PIXEL>(img_2a, img_2b, img_3, rows, cols);
add_with_color<RGB_IMAGE,RGB_PIXEL>(img_2a, img_2b, img_3, rows, cols);

// Convert the hls::mat format to AXI4 Stream format with SOF, EOL signals
myMat2AXIvideo<32, 220, 220, HLS_8UC3>(img_3, OUTPUT_STREAM);

}




//______________________________________________________________________________
// this design works with CSIM and Synthesis but needs a framebuffer

#if 0
void image_filter_problem(AXI_STREAM& INPUT_STREAM, AXI_STREAM& OUTPUT_STREAM) {

#pragma HLS INTERFACE axis depth=2000000 port=INPUT_STREAM bundle=VIDEO_IN
#pragma HLS INTERFACE axis depth=2000000 port=OUTPUT_STREAM bundle=VIDEO_OUT
//#pragma HLS INTERFACE axis depth=10000 port=INPUT_STREAM bundle=VIDEO_IN
//#pragma HLS INTERFACE axis depth=10000 port=OUTPUT_STREAM bundle=VIDEO_OUT
//#pragma HLS INTERFACE axis port=INPUT_STREAM bundle=VIDEO_IN
//#pragma HLS INTERFACE axis port=OUTPUT_STREAM bundle=VIDEO_OUT

//#pragma HLS INTERFACE s_axilite port=rows     bundle=CONTROL_BUS //offset=0x14
//#pragma HLS INTERFACE s_axilite port=cols     bundle=CONTROL_BUS //offset=0x1C

#pragma HLS INTERFACE s_axilite port=return   bundle=CONTROL_BUS
//#pragma HLS INTERFACE ap_stable port=rows
//#pragma HLS INTERFACE ap_stable port=cols

#pragma HLS dataflow
  
//assert(rows <= MAX_HEIGHT);
//assert(cols <= MAX_WIDTH);
const int rows = MAX_HEIGHT;
const int cols = MAX_WIDTH;

const int rows_half = rows/2;
const int cols_half = cols/2;


GRAY_IMAGE      img_0 (rows,      cols);
GRAY_IMAGE_HALF img_1 (rows_half, cols_half);

GRAY_IMAGE_HALF img_2a(rows_half, cols_half);
GRAY_IMAGE_HALF img_2b(rows_half, cols_half);
GRAY_IMAGE_HALF img_2c(rows_half, cols_half);
GRAY_IMAGE_HALF img_2d(rows_half, cols_half);

GRAY_IMAGE_HALF img_3a(rows_half, cols_half);
GRAY_IMAGE_HALF img_3b(rows_half, cols_half);
GRAY_IMAGE_HALF img_3c(rows_half, cols_half);

GRAY_IMAGE_HALF img_4a(rows_half, cols_half);
GRAY_IMAGE_HALF img_4b(rows_half, cols_half);
GRAY_IMAGE_HALF img_4c(rows_half, cols_half);
GRAY_IMAGE_HALF img_4d(rows_half, cols_half);
GRAY_IMAGE_HALF img_4e(rows_half, cols_half);

GRAY_IMAGE_HALF img_5a(rows_half, cols_half);
GRAY_IMAGE_HALF img_5b(rows_half, cols_half);
GRAY_IMAGE_HALF img_5c(rows_half, cols_half);
GRAY_IMAGE_HALF img_5d(rows_half, cols_half);

GRAY_IMAGE      img_6(rows, cols);

// Convert AXI4 Stream data to hls::mat format
hls::AXIvideo2Mat(INPUT_STREAM, img_0);

// 1. scale down by 2
// hls::Resize can't do II=1
//hls::Resize(img_0, img_1);
resize_img(img_0, img_1, rows, cols);

// 2. copy to 3 channels
replicate_by3(img_1, img_2a, img_2b, img_2c, rows_half, cols_half);

// 3. gradient
passthru(img_2a, img_3a, rows_half, cols_half);
grad_vertical<GRAY_IMAGE_HALF>(img_2b, img_3b, rows_half, cols_half);
grad_horizontal<GRAY_IMAGE_HALF>(img_2c, img_3c, rows_half, cols_half);

// 4. copy to 2 channels
passthru(img_3a, img_4a, rows_half, cols_half);
replicate_by2(img_3b, img_4b, img_4c, rows_half, cols_half);
replicate_by2(img_3c, img_4d, img_4e, rows_half, cols_half);

// 5 add 2 channels
passthru(img_4a, img_5a, rows_half, cols_half);
passthru(img_4b, img_5b, rows_half, cols_half);
passthru(img_4d, img_5c, rows_half, cols_half);
add<GRAY_IMAGE_HALF, GRAY_PIXEL>(img_4c, img_4e, img_5d, rows_half, cols_half);

// 5. combine
combine4(img_5a, img_5b, img_5c, img_5d, img_6, rows, cols);

// Convert the hls::mat format to AXI4 Stream format with SOF, EOL signals
hls::Mat2AXIvideo(img_6, OUTPUT_STREAM);

}
#endif
//______________________________________________________________________________
// decimate image by 2 in row/col
//
void resize_img(
	GRAY_IMAGE& img_in, 
	GRAY_IMAGE_HALF& img_out, 
	int rows, 
	int cols) {

GRAY_PIXEL pin;
GRAY_PIXEL pout;

L_row: for(int row = 0; row < rows; row++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=1080
		 
L_col: for(int col = 0; col < cols; col++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=1920
#pragma HLS pipeline rewind

           img_in >> pin;

		   if ( (row %2 == 0) & (col %2 == 0)) {
		       pout = pin;
               img_out << pout;
		   }
        }
    }

}

//______________________________________________________________________________
// replicate to 4 streams
//
void replicate_by4(
	GRAY_IMAGE_HALF& img_in, 
	GRAY_IMAGE_HALF& img_out0, 
	GRAY_IMAGE_HALF& img_out1, 
	GRAY_IMAGE_HALF& img_out2, 
	GRAY_IMAGE_HALF& img_out3, 
	int rows, 
	int cols) {

GRAY_PIXEL pin;
GRAY_PIXEL pout;

L_row: for(int row = 0; row < rows; row++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=1080
		 
L_col: for(int col = 0; col < cols; col++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=1920
#pragma HLS pipeline rewind

           img_in >> pin;

		   pout = pin;

           img_out0 << pout;
           img_out1 << pout;
           img_out2 << pout;
           img_out3 << pout;
        }
    }

}

//______________________________________________________________________________
// replicate to 3 streams
//
void replicate_by3(
	GRAY_IMAGE_HALF& img_in, 
	GRAY_IMAGE_HALF& img_out0, 
	GRAY_IMAGE_HALF& img_out1, 
	GRAY_IMAGE_HALF& img_out2, 
	int rows, 
	int cols) {

GRAY_PIXEL pin;
GRAY_PIXEL pout;

L_row: for(int row = 0; row < rows; row++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=1080
		 
L_col: for(int col = 0; col < cols; col++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=1920
#pragma HLS pipeline rewind

           img_in >> pin;

		   pout = pin;

           img_out0 << pout;
           img_out1 << pout;
           img_out2 << pout;
        }
    }

}


//______________________________________________________________________________
// replicate to 2 streams
//
template<typename IMG_T, typename PIXEL_T>
void replicate_by2(
	IMG_T& img_in, 
	IMG_T& img_out0, 
	IMG_T& img_out1, 
	int rows, 
	int cols) {

PIXEL_T pin;
PIXEL_T pout;

L_row: for(int row = 0; row < rows; row++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=1080
		 
L_col: for(int col = 0; col < cols; col++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=1920
#pragma HLS pipeline rewind

           img_in >> pin;

		   pout = pin;

           img_out0 << pout;
           img_out1 << pout;
        }
    }

}
//______________________________________________________________________________
// pass thru
//
template<typename IMG_T, typename PIXEL_T>
void passthru(
	IMG_T& img_in, 
	IMG_T& img_out, 
	int rows, 
	int cols) {

PIXEL_T pin;
PIXEL_T pout;

L_row: for(int row = 0; row < rows; row++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=1080
		 
L_col: for(int col = 0; col < cols; col++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=1920
#pragma HLS pipeline rewind

           img_in >> pin;
		   pout = pin;
           img_out << pout;
        }
    }

}

//______________________________________________________________________________
// combine 2 streams
//
template<typename IMG_T, typename PIXEL_T>
void combine2(
	IMG_T& img_in0, 
	IMG_T& img_in1, 
	IMG_T& img_out, 
	int rows, 
	int cols) {

PIXEL_T pin0, pin1;
PIXEL_T pout;

L_row: for(int row = 0; row < rows; row++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=1080
		 
L_col: for(int col = 0; col < cols; col++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=1920
#pragma HLS pipeline rewind

	       img_in0 >> pin0;
	       img_in1 >> pin1;

		   pout = pin0 + pin1;
		   img_out << pout;

    }

}

}

//______________________________________________________________________________
//
template<typename IMG_T, typename PIXEL_T>
void add_with_color(
	IMG_T& img_in0, 
	IMG_T& img_in1, 
	IMG_T& img_out, 
	int rows, 
	int cols) {

PIXEL_T pin0, pin1;
PIXEL_T pout;

L_row: for(int row = 0; row < rows; row++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=1080
		 
L_col: for(int col = 0; col < cols; col++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=1920
#pragma HLS pipeline rewind

	       img_in0 >> pin0;
	       img_in1 >> pin1;

		   if (pin0.val[0]>100 | pin0.val[1]>100 | pin0.val[2]>100) { // vertical edges
			 pout.val[0] = 0;    // B
			 pout.val[1] = 255;  // G
			 pout.val[2] = 0;    // R
	       } else if (pin1.val[0]>100 | pin1.val[1]>100 | pin1.val[2]>100) { // horizontal edges
			 pout.val[0] = 0;
			 pout.val[1] = 0;
			 pout.val[2] = 255;
	       } else {
			 pout.val[0] = 0;
			 pout.val[1] = 0;
			 pout.val[2] = 0;
		   }
		   img_out << pout;
    }

}

}

//______________________________________________________________________________
// combine 4 streams
//
void combine4(
	GRAY_IMAGE_HALF& img_in0, 
	GRAY_IMAGE_HALF& img_in1, 
	GRAY_IMAGE_HALF& img_in2, 
	GRAY_IMAGE_HALF& img_in3, 
	GRAY_IMAGE&      img_out, 
	int rows, 
	int cols) {

GRAY_PIXEL pin;
GRAY_PIXEL pout;

L_row: for(int row = 0; row < rows; row++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=1080
		 
L_col: for(int col = 0; col < cols; col++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=1920
#pragma HLS pipeline rewind

        	if(row < rows/2) { 
        			   if(col < cols/2) {    //-------------- top half/left half
                            img_in0 >> pin;
        				    pout = pin;
        		       } else {              //-------------- top half/right half
                            img_in1 >> pin;
        				    pout = pin;
        			   }
        	} else {     
        				if(col < cols/2) {   //-------------- bottom half/left half
                            img_in2 >> pin;
        				    pout = pin;
        		        } else {             //-------------- bottom half/right half
                            img_in3 >> pin;
        				    pout = pin;
        			    }
            }

           img_out << pout;
        }
    }

}

//______________________________________________________________________________
// gradient vertical
//
template<typename IMG_T>
void grad_vertical(IMG_T& img_in, IMG_T& img_out, int rows, int cols) {

// 2D kernel for veritical gradient
const COEF_T coef_v[KS][KS]= {
  {1,2,0,-2,-1},
  {4,8,0,-8,-4},
  {6,12,0,-12,-6},
  {4,8,0,-8,-4},
  {1,2,0,-2,-1}
};
hls::Window<KS,KS,COEF_T> Sv;
for (int r=0; r<KS; r++) for (int c=0; c<KS; c++) Sv.val[r][c] = coef_v[r][c];

// point
hls::Point_<INDEX_T> anchor;
anchor.x=-1;
anchor.y=-1;

hls::Filter2D <hls::BORDER_CONSTANT> (img_in, img_out, Sv, anchor);

}

//______________________________________________________________________________
// gradient horizontal 
//
template<typename IMG_T>
void grad_horizontal(IMG_T& img_in, IMG_T& img_out, int rows, int cols ) {

// 2D kernel for horizontal gradient
const COEF_T coef_h[KS][KS]= {
  {-1,-4,-6,-4,-1},
  {-2,-8,-12,-8,-2},
  {0, 0, 0, 0, 0},
  {2,8,12,8,2},
  {1,4,6,4,1}
};
hls::Window<KS,KS,COEF_T> Sh;
for (int r=0; r<KS; r++) for (int c=0; c<KS; c++) Sh.val[r][c] = coef_h[r][c];

// point
hls::Point_<INDEX_T> anchor;
anchor.x=-1;
anchor.y=-1;

hls::Filter2D <hls::BORDER_CONSTANT> (img_in, img_out, Sh, anchor);

}

//______________________________________________________________________________
// add two image
//
template<typename IMG_T, typename PIXEL_T>
void add(IMG_T& img_in0, IMG_T& img_in1, IMG_T& img_out, int rows, int cols) {

PIXEL_T pin0, pin1;
PIXEL_T pout;

L_row: for(int row = 0; row < rows; row++) {
#pragma HLS LOOP_TRIPCOUNT min=720 max=1080
		 
L_col: for(int col = 0; col < cols; col++) {
#pragma HLS LOOP_TRIPCOUNT min=1280 max=1920
#pragma HLS pipeline rewind

           img_in0 >> pin0;
           img_in1 >> pin1;

		   //pout = (pin0 + pin1)/2;
		   pout = (pin0 + pin1);

           img_out << pout;
        }
    }

}


//______________________________________________________________________________
// to create test frame
//

#if 0
void image_filter_cf(AXI_STREAM& OUTPUT_STREAM, int rows, int cols) {

#pragma HLS INTERFACE axis port=OUTPUT_STREAM bundle=VIDEO_OUT

#pragma HLS INTERFACE s_axilite port=rows     bundle=CONTROL_BUS //offset=0x14
#pragma HLS INTERFACE s_axilite port=cols     bundle=CONTROL_BUS //offset=0x1C

#pragma HLS INTERFACE s_axilite port=return   bundle=CONTROL_BUS
#pragma HLS INTERFACE ap_stable port=rows
#pragma HLS INTERFACE ap_stable port=cols

#pragma HLS dataflow
  
RGB_IMAGE  img_0(rows, cols);
//GRAY_IMAGE img_1(rows, cols);

assert(rows <= MAX_HEIGHT);
assert(cols <= MAX_WIDTH);

createFrame(img_0, rows, cols);

// Convert the hls::mat format to AXI4 Stream format with SOF, EOL signals
hls::Mat2AXIvideo(img_0, OUTPUT_STREAM);

}

//______________________________________________________________________________
// create test frame
//
void createFrame(RGB_IMAGE& img_out, int rows, int cols ) {

RGB_PIXEL pin;
RGB_PIXEL pout;

static short frameCnt = 0;

if (frameCnt<nOfBlank) {
  pout.val[0]=255;
  pout.val[1]=255;
  pout.val[2]=255;
} else {
  pout.val[0]=0;
  pout.val[1]=0;
  pout.val[2]=frameCnt<<1;
} 


L_row: for(int row = 0; row < rows; row++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=1080
		 
L_col: for(int col = 0; col < cols; col++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=1920
#pragma HLS pipeline rewind

		   // streaming out
           img_out << pout;
        }
    }

if (frameCnt==lastFrame-1) frameCnt=0; else frameCnt++;

}

#endif

//______________________________________________________________________________
// color conversion
//

#if 0
void image_filter(AXI_STREAM& INPUT_STREAM, AXI_STREAM& OUTPUT_STREAM, int rows, int cols) {

//#pragma HLS INTERFACE axis port=INPUT_STREAM bundle=VIDEO_IN
#pragma HLS INTERFACE axis port=OUTPUT_STREAM bundle=VIDEO_OUT

#pragma HLS INTERFACE s_axilite port=rows     bundle=CONTROL_BUS //offset=0x14
#pragma HLS INTERFACE s_axilite port=cols     bundle=CONTROL_BUS //offset=0x1C

#pragma HLS INTERFACE s_axilite port=return   bundle=CONTROL_BUS
#pragma HLS INTERFACE ap_stable port=rows
#pragma HLS INTERFACE ap_stable port=cols

#pragma HLS dataflow
  
RGB_IMAGE  img_0(rows, cols);
GRAY_IMAGE img_1(rows, cols);

// Convert AXI4 Stream data to hls::mat format
hls::AXIvideo2Mat(INPUT_STREAM, img_0);

assert(rows <= MAX_HEIGHT);
assert(cols <= MAX_WIDTH);

// color conversion
hls::CvtColor<HLS_RGB2GRAY>(img_0, img_1);

// Convert the hls::mat format to AXI4 Stream format with SOF, EOL signals
hls::Mat2AXIvideo(img_1, OUTPUT_STREAM);

}
#endif

//______________________________________________________________________________
#if 0
void image_filter_scale(AXI_STREAM& INPUT_STREAM, AXI_STREAM& OUTPUT_STREAM, int rows, int cols, RATIO_T K) {

#pragma HLS INTERFACE axis port=INPUT_STREAM  bundle=VIDEO_IN
#pragma HLS INTERFACE axis port=OUTPUT_STREAM bundle=VIDEO_OUT

#pragma HLS INTERFACE s_axilite port=rows     bundle=CONTROL_BUS //offset=0x14
#pragma HLS INTERFACE s_axilite port=cols     bundle=CONTROL_BUS //offset=0x1C

#pragma HLS INTERFACE s_axilite port=return   bundle=CONTROL_BUS

// to make sure these does not play into synthesis 
#pragma HLS INTERFACE ap_stable port=rows
#pragma HLS INTERFACE ap_stable port=cols
#pragma HLS INTERFACE ap_stable port=K

#pragma HLS dataflow
  
assert(rows <= MAX_HEIGHT);
assert(cols <= MAX_WIDTH);

int dst_rows = rows*K;
int dst_cols = cols*K;

RGB_IMAGE      img_0(rows, cols);
RGB_IMAGE_HALF img_1(dst_rows, dst_cols);

// Convert AXI4 Stream data to hls::mat format
hls::AXIvideo2Mat(INPUT_STREAM, img_0);

hls::Resize(img_0, img_1);
//hls::Resize(img_0, img_1, HLS_INTER_CUBIC);

// Convert the hls::mat format to AXI4 Stream format with SOF, EOL signals
hls::Mat2AXIvideo(img_1, OUTPUT_STREAM);

}


//______________________________________________________________________________

void image_filter_yy(AXI_STREAM& INPUT_STREAM, AXI_STREAM& OUTPUT_STREAM, int rows, int cols) {

#pragma HLS INTERFACE axis port=INPUT_STREAM bundle=VIDEO_IN
#pragma HLS INTERFACE axis port=OUTPUT_STREAM bundle=VIDEO_OUT

#pragma HLS INTERFACE s_axilite port=rows     bundle=CONTROL_BUS //offset=0x14
#pragma HLS INTERFACE s_axilite port=cols     bundle=CONTROL_BUS //offset=0x1C

#pragma HLS INTERFACE s_axilite port=return   bundle=CONTROL_BUS
#pragma HLS INTERFACE ap_stable port=rows
#pragma HLS INTERFACE ap_stable port=cols

#pragma HLS dataflow
  
RGB_IMAGE img_0(rows, cols);
RGB_IMAGE img_0a(rows, cols);
RGB_IMAGE img_0b(rows, cols);
RGB_IMAGE img_1a(rows, cols);
RGB_IMAGE img_1b(rows, cols);
RGB_IMAGE img_2(rows, cols);
RGB_IMAGE img_3(rows, cols);
//RGB_PIXEL pix(50, 50, 50);

// Convert AXI4 Stream data to hls::mat format
hls::AXIvideo2Mat(INPUT_STREAM, img_0);

assert(rows <= MAX_HEIGHT);
assert(cols <= MAX_WIDTH);

replicate(img_0, img_0a, img_0b, rows, cols);
grad_vertical(img_0a, img_1a, rows, cols);
grad_horizontal(img_0b, img_1b, rows, cols);
add(img_1a, img_1b, img_2, rows, cols);
gain(img_2, img_3, rows, cols, 2);

// Convert the hls::mat format to AXI4 Stream format with SOF, EOL signals
hls::Mat2AXIvideo(img_3, OUTPUT_STREAM);

}


//______________________________________________________________________________

void image_filter_xx(AXI_STREAM& INPUT_STREAM, AXI_STREAM& OUTPUT_STREAM, int rows, int cols) {

#pragma HLS INTERFACE axis port=INPUT_STREAM bundle=VIDEO_IN
#pragma HLS INTERFACE axis port=OUTPUT_STREAM bundle=VIDEO_OUT

#pragma HLS INTERFACE s_axilite port=rows     bundle=CONTROL_BUS //offset=0x14
#pragma HLS INTERFACE s_axilite port=cols     bundle=CONTROL_BUS //offset=0x1C

#pragma HLS INTERFACE s_axilite port=return   bundle=CONTROL_BUS
#pragma HLS INTERFACE ap_stable port=rows
#pragma HLS INTERFACE ap_stable port=cols

#pragma HLS dataflow
  
RGB_IMAGE img_0(rows, cols);
RGB_IMAGE img_0a(rows, cols);
RGB_IMAGE img_0b(rows, cols);
RGB_IMAGE img_1a(rows, cols);
RGB_IMAGE img_1b(rows, cols);
//RGB_IMAGE img_2(rows, cols);
//RGB_IMAGE img_3(rows, cols);
//RGB_PIXEL pix(50, 50, 50);

// Convert AXI4 Stream data to hls::mat format
hls::AXIvideo2Mat(INPUT_STREAM, img_0);

assert(rows <= MAX_HEIGHT);
assert(cols <= MAX_WIDTH);

// processing pipeline
#if 0
swap(img_0, img_1, rows, cols);
gain(img_1, img_2, rows, cols, 1);
gain(img_2, img_3, rows, cols, 2);
#endif

replicate(img_0, img_0a, img_0b, rows, cols);
grad_vertical(img_0a, img_1a, rows, cols);
grad_horizontal(img_0b, img_1b, rows, cols);

// Convert the hls::mat format to AXI4 Stream format with SOF, EOL signals
hls::Mat2AXIvideo(img_1b, OUTPUT_STREAM);

}

//______________________________________________________________________________
// swapping color
//
void swap(RGB_IMAGE& img_in, RGB_IMAGE& img_out, int rows, int cols) {

RGB_PIXEL pin;
RGB_PIXEL pout;

L_row: for(int row = 0; row < rows; row++) {
#pragma HLS LOOP_TRIPCOUNT min=720 max=1080
		 
L_col: for(int col = 0; col < cols; col++) {
#pragma HLS LOOP_TRIPCOUNT min=1280 max=1920
#pragma HLS pipeline rewind

		    // streaming in
            img_in >> pin;

        	if(row < rows/2) { 
        			   if(col < cols/2) { //-------------- top half/left half
        			        //switch red and green
        				    pout.val[0] = pin.val[0];
        				    pout.val[1] = pin.val[2];
        				    pout.val[2] = pin.val[1];
        		       } else {              //-------------- top half/right half
        			        //switch red and blue
        				    pout.val[0] = pin.val[2];
        				    pout.val[1] = pin.val[1];
        				    pout.val[2] = pin.val[0];
        			   }
        	} else {     
        				if(col < cols/2) { // ------------------ bottom half/left half
        			        //switch green and blue
        				    pout.val[0] = pin.val[1];
        				    pout.val[1] = pin.val[0];
        				    pout.val[2] = pin.val[2];
        		        } else {             // ------------------ bottom half/right half
        			        //don't switch
        				    pout.val[0] = pin.val[0];
        				    pout.val[1] = pin.val[1];
        				    pout.val[2] = pin.val[2];
        			    }
            }

		   // streaming out
           img_out << pout;
        }
    }

}

//______________________________________________________________________________
// gain
//
void gain(RGB_IMAGE& img_in, RGB_IMAGE& img_out, int rows, int cols, GAIN_T K) {

RGB_PIXEL pin;
RGB_PIXEL pout;

L_row: for(int row = 0; row < rows; row++) {
#pragma HLS LOOP_TRIPCOUNT min=720 max=1080
		 
L_col: for(int col = 0; col < cols; col++) {
#pragma HLS LOOP_TRIPCOUNT min=1280 max=1920
#pragma HLS pipeline rewind

		   // streaming in
           img_in >> pin;

		   pout.val[0] = K * pin.val[0];
		   pout.val[1] = K * pin.val[1];
		   pout.val[2] = K * pin.val[2];

		   // streaming out
           img_out << pout;
        }
    }

}

//______________________________________________________________________________
// replicate stream
//
void replicate(RGB_IMAGE& img_in, RGB_IMAGE& img_out0, RGB_IMAGE& img_out1, int rows, int cols) {

RGB_PIXEL pin;
RGB_PIXEL pout;

L_row: for(int row = 0; row < rows; row++) {
#pragma HLS LOOP_TRIPCOUNT min=720 max=1080
		 
L_col: for(int col = 0; col < cols; col++) {
#pragma HLS LOOP_TRIPCOUNT min=1280 max=1920
#pragma HLS pipeline rewind

           img_in >> pin;

		   pout = pin;

           img_out0 << pout;
           img_out1 << pout;
        }
    }

}


#endif


//______________________________________________________________________________
// using hls::Sobel
//
#if 0
void image_filter(AXI_STREAM& INPUT_STREAM, AXI_STREAM& OUTPUT_STREAM, int rows, int cols) {

//Create AXI streaming interfaces for the core
GRAY_IMAGE img_0(rows, cols);
GRAY_IMAGE img_1(rows, cols);
//GRAY_IMAGE img_2(rows, cols);
//GRAY_IMAGE img_3(rows, cols);
//GRAY_IMAGE img_4(rows, cols);
//GRAY_IMAGE img_5(rows, cols);
//GRAY_PIXEL pix(50, 50, 50);

// Convert AXI4 Stream data to hls::mat format
hls::AXIvideo2Mat(INPUT_STREAM, img_0);

// Execute the video pipelines
hls::Sobel<1,0,3>(img_0, img_1);

#if 0
hls::SubS(img_1, pix, img_2);
hls::Scale(img_2, img_3, 2, 0);
hls::Erode(img_3, img_4);
hls::Dilate(img_4, img_5);
#endif

// Convert the hls::mat format to AXI4 Stream format
//hls::Mat2AXIvideo(img_5, OUTPUT_STREAM);
hls::Mat2AXIvideo(img_1, OUTPUT_STREAM);

}

#endif
