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

#ifndef IMAGE_FILTER_H
#define IMAGE_FILTER_H

// maximum image size
//#define MAX_WIDTH  1920
//#define MAX_HEIGHT 1080
//#define MAX_WIDTH  238
//#define MAX_HEIGHT 179
#define MAX_WIDTH  220
#define MAX_HEIGHT 220

#include  "hls_video.h"
#include <ap_fixed.h>

typedef hls::stream<ap_axiu<32,1,1,1> >           AXI_STREAM;
//typedef hls::stream<ap_axiu<8,1,1,1> >            AXI_STREAM;

// check include/hls/hls_video_types.h for detail on pixel type
typedef hls::Mat<MAX_HEIGHT,   MAX_WIDTH,   HLS_8UC3> RGB_IMAGE;
typedef hls::Mat<MAX_HEIGHT/2, MAX_WIDTH/2, HLS_8UC3> RGB_IMAGE_HALF;
typedef hls::Scalar<3, unsigned char>                 RGB_PIXEL;

//#define WIDTH  220
//#define HEIGHT 220
//typedef hls::Mat<HEIGHT,   WIDTH,   HLS_8UC1> GRAY_IMAGE;
//typedef hls::Mat<HEIGHT/2, WIDTH/2, HLS_8UC1> GRAY_IMAGE_HALF;

typedef hls::Mat<MAX_HEIGHT,   MAX_WIDTH,   HLS_8UC1> GRAY_IMAGE;
typedef hls::Mat<MAX_HEIGHT/2, MAX_WIDTH/2, HLS_8UC1> GRAY_IMAGE_HALF;

typedef hls::Scalar<1, unsigned char>  GRAY_PIXEL;

typedef ap_fixed<8,4>        GAIN_T;
typedef ap_fixed<8,4,AP_RND> COEF_T;
typedef ap_int<16>           INDEX_T;
typedef ap_fixed<8,4>        RATIO_T;

// kernel size
const int KS = 5;

// test frame size
#if 0
const int row_tf = 60;
const int col_tf = 100;
const int nOfBlank  = 2;
const int lastFrame = 5;
#endif


//#define INPUT_IMAGE1          "image_one.png"
//#define INPUT_IMAGE2          "image_two.png"
//#define INPUT_IMAGE3          "image_three.png"
//#define INPUT_IMAGE4          "image_four.png"
//#define INPUT_IMAGE          "test_1080p.bmp"
//#define INPUT_IMAGE          "small_image.bmp"
#define INPUT_IMAGE          "checkerboard.bmp"
#define OUTPUT_IMAGE         "output.bmp"
#define OUTPUT_IMAGE_GOLDEN  "golden_output.bmp"

//void createFrame(RGB_IMAGE& img_out, int rows, int cols);
//void image_filter(AXI_STREAM& OUTPUT_STREAM, int rows, int cols);

template<int W, int ROWS, int COLS, int T>
int myAXIvideo2Mat(hls::stream<ap_axiu<W,1,1,1> >& AXI_video_strm,
                 hls::Mat<ROWS, COLS, T>& img)
{
    int res = 0;
    ap_axiu<W,1,1,1> axi;
    hls::Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> pix;
    int depth = HLS_TBITDEPTH(T);
    // std::cout << W << " " << depth << " " << HLS_MAT_CN(T) << "\n";
    assert(W >= depth*HLS_MAT_CN(T) && "Bit-Width of AXI stream must be greater than the total number of bits in a pixel");
    HLS_SIZE_T rows = img.rows;
    HLS_SIZE_T cols = img.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
//     bool sof = 0;
//  loop_wait_for_start: while (!sof) {
// #pragma HLS pipeline II=1
// #pragma HLS loop_tripcount avg=0 max=0
//         AXI_video_strm >> axi;
//         sof = axi.user.to_int();
//     }
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
        bool eol = 0;
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            // if (sof || eol) {
            //     sof = 0;
            //     eol = axi.last.to_int();
            // } 
            // else {
                // If we didn't reach EOL, then read the next pixel
                AXI_video_strm >> axi;
                // eol = axi.last.to_int();
                // bool user = axi.user.to_int();
                // if(user) {
                //     res |= ERROR_IO_SOF_EARLY;
                // }
            // }
            // if (eol && (j != cols-1) && (i != rows-1)) {
            //     res |= ERROR_IO_EOL_EARLY;
            // }
        loop_channels: for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(T); k++) {
                hls::AXIGetBitFields(axi, k*depth, depth, pix.val[k]);
            }
            img << pix;
        }
//    loop_wait_for_eol: while (!eol) {
//#pragma HLS pipeline II=1
//#pragma HLS loop_tripcount avg=0 max=0
//            // Keep reading until we get to EOL
//            AXI_video_strm >> axi;
//            eol = axi.last.to_int();
//            res |= ERROR_IO_EOL_LATE;
//        }
    }
    return res;
}

template<int W, int ROWS, int COLS, int T>
int myMat2AXIvideo(hls::Mat<ROWS, COLS, T>& img,
                 hls::stream<ap_axiu<W,1,1,1> >& AXI_video_strm)
{
    int res = 0;
    hls::Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> pix;
    ap_axiu<W,1,1,1> axi;
    int depth = HLS_TBITDEPTH(T);
    // std::cout << W << " " << depth << " " << HLS_MAT_CN(T) << "\n";
    assert(W >= depth*HLS_MAT_CN(T) && "Bit-Width of AXI stream must be greater than the total number of bits in a pixel");
    HLS_SIZE_T rows = img.rows;
    HLS_SIZE_T cols = img.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    bool sof = 1;
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            if (sof) {
                axi.user = 1;
                sof = 0;
            } else {
                axi.user = 0;
            }
            if (j == (cols-1) && i == (rows-1)) {
                axi.last = 1;
            } else {
                axi.last = 0;
            }
            img >> pix;
            axi.data = -1;
        loop_channels: for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(T); k++) {
                hls::AXISetBitFields(axi, k*depth, depth, pix.val[k]);
            }
            axi.keep = -1;
            AXI_video_strm << axi;
        }
    }
    return res;
}

void image_filter(
	AXI_STREAM& INPUT_STREAM, 
	AXI_STREAM& OUTPUT_STREAM ); 
	//int rows, 
	//int cols);

void resize_img(
	GRAY_IMAGE& img_in, 
	GRAY_IMAGE_HALF& img_out, 
	int rows, 
	int cols);

void replicate_by4(
	GRAY_IMAGE_HALF& img_in, 
	GRAY_IMAGE_HALF& img_out0, 
	GRAY_IMAGE_HALF& img_out1, 
	GRAY_IMAGE_HALF& img_out2, 
	GRAY_IMAGE_HALF& img_out3, 
	int rows, 
	int cols);

void replicate_by3(
	GRAY_IMAGE_HALF& img_in, 
	GRAY_IMAGE_HALF& img_out0, 
	GRAY_IMAGE_HALF& img_out1, 
	GRAY_IMAGE_HALF& img_out2, 
	int rows, 
	int cols);

template<typename IMG_T, typename PIXEL_T>
void replicate_by2(
	IMG_T& img_in, 
	IMG_T& img_out0, 
	IMG_T& img_out1, 
	int rows, 
	int cols);

template<typename IMG_T, typename PIXEL_T>
void passthru(
	GRAY_IMAGE_HALF& img_in, 
	GRAY_IMAGE_HALF& img_out, 
	int rows, 
	int cols);

void combine4(
	GRAY_IMAGE_HALF& img_in0, 
	GRAY_IMAGE_HALF& img_in1, 
	GRAY_IMAGE_HALF& img_in2, 
	GRAY_IMAGE_HALF& img_in3, 
	GRAY_IMAGE& img_out, 
	int rows, 
	int cols);

template<typename IMG_T, typename PIXEL_T>
void combine2(
	IMG_T& img_in0, 
	IMG_T& img_in1, 
	IMG_T& img_out, 
	int rows, 
	int cols);

template<typename IMG_T>
void grad_vertical(IMG_T& img_in, IMG_T& img_out, int rows, int cols);

template<typename IMG_T>
void grad_horizontal(IMG_T& img_in, IMG_T& img_out, int rows, int cols);

template<typename IMG_T, typename PIXEL_T>
void add(IMG_T& img_in0, IMG_T& img_in1, IMG_T& img_out, int rows, int cols);

template<typename IMG_T, typename PIXEL_T>
void add_with_color(
	IMG_T& img_in0, 
	IMG_T& img_in1, 
	IMG_T& img_out, 
	int rows, 
	int cols);

//void replicate(RGB_IMAGE& img_in, RGB_IMAGE& img_out0, RGB_IMAGE& img_out1, int rows, int cols);
//void add(RGB_IMAGE& img_in0, RGB_IMAGE& img_in1, RGB_IMAGE& img_out, int rows, int cols);
//void gain(RGB_IMAGE& img_in, RGB_IMAGE& img_out, int rows, int cols, GAIN_T K);

#endif

