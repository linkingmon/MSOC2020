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
#include "hls_opencv.h"

#include <iostream>
using namespace std;
//______________________________________________________________________________
// test sequence of grey scale image

int main (int argc, char** argv) {

IplImage* src;
IplImage* dst;

#if 0
src = cvLoadImage(INPUT_IMAGE1);
cout << "=================================="<<endl;
cout << "nChannels = " << src->nChannels << endl;
cout << "depth = " << src->depth << endl;
cout << "width = " << src->width << endl;
cout << "height = " << src->height << endl;
cout << "=================================="<<endl;
return 0;
#endif

// AXI stream version
AXI_STREAM src_axi, dst_axi;


for (int k=0;k<1;k++) {

    src = cvLoadImage(INPUT_IMAGE);
    dst = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);

    IplImage2AXIvideo(src, src_axi);

    //______________________________________________________________________________
    // DUT: 
    image_filter(src_axi, dst_axi);
    //______________________________________________________________________________
	
    // Convert the AXI4 Stream data to OpenCV format
    AXIvideo2IplImage(dst_axi, dst);

}

cvSaveImage(OUTPUT_IMAGE, dst);
cvReleaseImage(&src);
cvReleaseImage(&dst);

#if 0
char tempbuf[2000];
sprintf(tempbuf, "diff --brief -w %s %s", OUTPUT_IMAGE, OUTPUT_IMAGE_GOLDEN);

int ret = system(tempbuf);
if (ret != 0) {
    printf("????????????? Test Failed!\n");
    ret = 1;
} else {
    printf("------------- Test Passed!\n");
}

return ret;
#endif

return 0;

}

//
//______________________________________________________________________________
// test sequence of grey scale image

#if 0
int main (int argc, char** argv) {

IplImage* src;
IplImage* src_gray;
IplImage* dst_gray;

int depthGray=8, nChannelsGray=1;


#if 0
src = cvLoadImage(INPUT_IMAGE1);
cout << "=================================="<<endl;
cout << "nChannels = " << src->nChannels << endl;
cout << "depth = " << src->depth << endl;
cout << "width = " << src->width << endl;
cout << "height = " << src->height << endl;
cout << "=================================="<<endl;
return 0;
#endif

//src_gray = cvCreateImage(cvGetSize(src), depthGray, nChannelsGray);
//dst_gray = cvCreateImage(cvGetSize(src), depthGray, nChannelsGray);
src_gray = cvCreateImage(cvSize(MAX_WIDTH, MAX_HEIGHT), depthGray, nChannelsGray);
dst_gray = cvCreateImage(cvSize(MAX_WIDTH, MAX_HEIGHT), depthGray, nChannelsGray);

// AXI stream version
AXI_STREAM src_axi, dst_axi;


for (int k=0;k<3;k++) {

	if (k==0) 
        src = cvLoadImage(INPUT_IMAGE1);
	else if (k==1) 
        src = cvLoadImage(INPUT_IMAGE2);
	else if (k==2) 
        src = cvLoadImage(INPUT_IMAGE3);
	else if (k==3) 
        src = cvLoadImage(INPUT_IMAGE4);

    cvCvtColor(src, src_gray, CV_RGB2GRAY);

    // Convert OpenCV format to AXI4 Stream format
    IplImage2AXIvideo(src_gray, src_axi);

    //______________________________________________________________________________
    // DUT:  the function to be synthesized
    //image_filter(src_axi, dst_axi, src->height, src->width);
    image_filter(src_axi, dst_axi);
    //______________________________________________________________________________
	
    // Convert the AXI4 Stream data to OpenCV format
    AXIvideo2IplImage(dst_axi, dst_gray);

	//cvReleaseImage(&src);
	//cvReleaseImage(&src_gray);
}

// Standard OpenCV image functions
cvSaveImage(OUTPUT_IMAGE, dst_gray);

cvReleaseImage(&src);
cvReleaseImage(&src_gray);
cvReleaseImage(&dst_gray);

#if 0
char tempbuf[2000];
sprintf(tempbuf, "diff --brief -w %s %s", OUTPUT_IMAGE, OUTPUT_IMAGE_GOLDEN);

int ret = system(tempbuf);
if (ret != 0) {
    printf("????????????? Test Failed!\n");
    ret = 1;
} else {
    printf("------------- Test Passed!\n");
}

return ret;
#endif

return 0;

}
#endif


//______________________________________________________________________________
// used for frame count

#if 0
int main (int argc, char** argv) {

IplImage* dst = cvCreateImage(cvSize(col_tf, row_tf), 8, 3);

// AXI stream version
AXI_STREAM dst_axi;

char tempbuf[100];

for (int k=0;k<2*lastFrame;k++) {
    image_filter(dst_axi, row_tf, col_tf);
    // Convert the AXI4 Stream data to OpenCV format
    AXIvideo2IplImage(dst_axi, dst);
    // Standard OpenCV image functions
    sprintf(tempbuf, "%s_%d.bmp", "output", k);
    //cvSaveImage(OUTPUT_IMAGE, dst);
    cvSaveImage(tempbuf, dst);
}

cvReleaseImage(&dst);

return 0;

}

#endif
//______________________________________________________________________________
// used for color conversion

#if 0
int main (int argc, char** argv) {

// Load data in OpenCV image format
IplImage* src = cvLoadImage(INPUT_IMAGE);
//IplImage* dst = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
IplImage* dst = cvCreateImage(cvGetSize(src), 8,1);

// AXI stream version
AXI_STREAM src_axi, dst_axi;

// Convert OpenCV format to AXI4 Stream format
IplImage2AXIvideo(src, src_axi);

//_ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ 
// DUT:  the function to be synthesized
image_filter(src_axi, dst_axi, src->height, src->width);
//_ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ 

// Convert the AXI4 Stream data to OpenCV format
AXIvideo2IplImage(dst_axi, dst);

// Standard OpenCV image functions
cvSaveImage(OUTPUT_IMAGE, dst);

//opencv_image_filter(src, dst);
//cvSaveImage(OUTPUT_IMAGE_GOLDEN, dst);

cvReleaseImage(&src);
cvReleaseImage(&dst);

#if 1
char tempbuf[2000];
sprintf(tempbuf, "diff --brief -w %s %s", OUTPUT_IMAGE, OUTPUT_IMAGE_GOLDEN);

int ret = system(tempbuf);
if (ret != 0) {
    printf("Test Failed!\n");
    ret = 1;
} else {
    printf("Test Passed!\n");
}

return ret;
#endif

return 0;

}

#endif

//______________________________________________________________________________
// used for rescaling

#if 0
int main (int argc, char** argv) {

// Load data in OpenCV image format
IplImage* src = cvLoadImage(INPUT_IMAGE);

//IplImage* dst = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);

// dimension of source/destination
int src_h = src->height;
int src_w = src->width;
RATIO_T K = 0.5;
int dst_h = src_h*K;
int dst_w = src_w*K;

cout << endl <<"output image dimension: row * height = " << dst_h <<" * " << dst_w <<endl; 

IplImage* dst = cvCreateImage(cvSize(dst_w, dst_h), src->depth, src->nChannels);

// AXI stream version
AXI_STREAM src_axi, dst_axi;

// Convert OpenCV format to AXI4 Stream format
IplImage2AXIvideo(src, src_axi);

// DUT:  the function to be synthesized
image_filter(src_axi, dst_axi, src->height, src->width, K);

// Convert the AXI4 Stream data to OpenCV format
AXIvideo2IplImage(dst_axi, dst);

// Standard OpenCV image functions
cvSaveImage(OUTPUT_IMAGE, dst);

cvReleaseImage(&src);
cvReleaseImage(&dst);

int ret = 0;

#if 0
// compare the output
char tempbuf[2000];
sprintf(tempbuf, "diff --brief -w %s %s", OUTPUT_IMAGE, OUTPUT_IMAGE_GOLDEN);

ret = system(tempbuf);
if (ret != 0) {
    printf("Test Failed!\n");
    ret = 1;
} else {
    printf("Test Passed!\n");
}
#endif

return ret;

}
#endif


//______________________________________________________________________________
// for GRAY
#if 0
int main (int argc, char** argv) {

// Load data in OpenCV image format
IplImage* src = cvLoadImage(INPUT_IMAGE);
IplImage* dst = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);

// convert to gray scale image
int depthGray=8, nChannelsGray=1;
IplImage* src_gray = cvCreateImage(cvGetSize(src), depthGray, nChannelsGray);

cvCvtColor(src, src_gray, CV_RGB2GRAY);
//cvCvtColor(src, src_gray, CV_BGR2GRAY);  // any difference from RGB2GRAY?
//cv::cvtColor(src, src_gray, CV_RGB2GRAY);

IplImage* dst_gray = cvCreateImage(cvGetSize(src), depthGray, nChannelsGray);

// AXI stream version
AXI_STREAM src_axi, dst_axi;

// Convert OpenCV format to AXI4 Stream format
IplImage2AXIvideo(src_gray, src_axi);

// DUT:  the function to be synthesized
image_filter(src_axi, dst_axi, src->height, src->width);

//______________________________________________________________________________

// Convert the AXI4 Stream data to OpenCV format
AXIvideo2IplImage(dst_axi, dst_gray);

// Standard OpenCV image functions
cvSaveImage(OUTPUT_IMAGE, dst_gray);

//opencv_image_filter(src, dst);
//cvSaveImage(OUTPUT_IMAGE_GOLDEN, dst);

cvReleaseImage(&src);
//cvReleaseImage(&dst);
cvReleaseImage(&src_gray);
cvReleaseImage(&dst_gray);


#if 0
char tempbuf[2000];
sprintf(tempbuf, "diff --brief -w %s %s", OUTPUT_IMAGE, OUTPUT_IMAGE_GOLDEN);

int ret = system(tempbuf);
if (ret != 0) {
    printf("Test Failed!\n");
    ret = 1;
} else {
    printf("Test Passed!\n");
}

return ret;
#endif

return 0;

}
#endif


#if 0
//______________________________________________________________________________
int main (int argc, char** argv) {

// Load data in OpenCV image format
IplImage* src = cvLoadImage(INPUT_IMAGE);
IplImage* dst = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);

// AXI stream version
AXI_STREAM src_axi, dst_axi;

// Convert OpenCV format to AXI4 Stream format
IplImage2AXIvideo(src, src_axi);

// DUT:  the function to be synthesized
image_filter(src_axi, dst_axi, src->height, src->width);

// Convert the AXI4 Stream data to OpenCV format
AXIvideo2IplImage(dst_axi, dst);

// Standard OpenCV image functions
cvSaveImage(OUTPUT_IMAGE, dst);

//opencv_image_filter(src, dst);
//cvSaveImage(OUTPUT_IMAGE_GOLDEN, dst);

cvReleaseImage(&src);
cvReleaseImage(&dst);

#if 0
char tempbuf[2000];
sprintf(tempbuf, "diff --brief -w %s %s", OUTPUT_IMAGE, OUTPUT_IMAGE_GOLDEN);

int ret = system(tempbuf);
if (ret != 0) {
    printf("Test Failed!\n");
    ret = 1;
} else {
    printf("Test Passed!\n");
}

return ret;
#endif

return 0;

}

#endif
