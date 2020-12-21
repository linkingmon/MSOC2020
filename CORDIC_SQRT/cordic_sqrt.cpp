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

#include "cordic_defines.h"
/*

  mode 1: rotation;  Rotates (x0,y0) by z0
    (xn,yn)=(cos(z0),sin(z0)) for (x0,y0)=(1,0)
    x[i+1] = x[i] - y[i] * d[i] * 2^(-1)
    y[i+1] = y[i] + x[i] * d[i] * 2^(-i)
    z[i+1] = z[i]        - d[i] * atan(2^(-i))

	d[i] = +1 if y[i]<0 else -1

	xn = An*sqrt(x[0]^2+y[0]^2)
	yn = 0;
	zn = z[0] + atan(x[0]/y[0])
		 ___	
	An = | | sqrt(1+2^(-2*i))
		  n

	Gain (An) = 1.64676; n = 10 (0 to 10)

  mode 0: vectoring; Rotates (x0,y0) to X-axis
    (xn,zn)=(r,theta)

  -PI/2<z0<PI/2 -> original
  
  EG extended to 
  0 to 2PI
*/


#ifdef SQRT_CORDIC
#define ROT 11


template <int TOT, int INT>
ap_fixed<TOT, INT> cordic_sqrt(dinp_t x0, dinp_t y0)
{

	  unsigned char i;


  ap_fixed<TOT, INT> x, y, xp, yp, x2;

  // to compensate the cordic gain of G = 1.0/0.6072
  const ap_fixed<16,1, AP_RND_MIN_INF, AP_SAT> inv_G = 0.607253031529134; // that is 0.6072 == 453 / 746;


  xp=x0;
  yp=y0;

for (i=0;i<ROT;i++)
  {
#pragma HLS PIPELINE II=1
    if (yp<0) {
      x = xp - (yp>>i);
      y = yp + (xp>>i);
    } else {
      x = xp + (yp>>i);
      y = yp - (xp>>i);
    }
	xp=x;
	yp=y;
  }

// compensating the cordic gain
#pragma HLS RESOURCE variable=x2 core=MUL6S


  x2     =  xp * inv_G; //xn = ((x * 453) / 746) ;

//x =   (xp *311)>>9; // much less accurate


  return x2;

}


dout_t process_magnitude_cordic(dinp_t real_data, dinp_t imag_data) 
{
#pragma	HLS INLINE OFF

#ifndef SQRT_AP_INT
dout_t mag_data = (dout_t) cordic_sqrt<40,32>( real_data, imag_data ); // for CORDIC SOLUTION 3 WHEN SQRT_AP_INT IS NOT DEFINED
#else
dout_t mag_data = (dout_t) cordic_sqrt<24,18>( real_data, imag_data ); // for CORDIC SOLUTION 4  WHEN SQRT_AP_INT IS DEFINED
#endif

return mag_data;
}

#endif //SQRT_CORDIC

