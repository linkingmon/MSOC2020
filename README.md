# MSOC2020

## Overview
|    Project name    |                            Content                             |          Related Topics           |
|:------------------:|:--------------------------------------------------------------:|:---------------------------------:|
|        Lab1        |                 Vivado-HLS & Vivado work flow                  |   Vivado-HLS, PYNQ, Multiplier    |
|  Lab2-1 & Lab2-2   |               AXI-Master & AXI-Stream interface                |           AXI-M, AXI-S            |
|     Video_edge     | Video Edge detection acceleration design by OpenCV-HLS library | OpenCV-hls, PYNQ, video-streaming |
|        Lab3        |                       XRT, HW emulation                        |               Vitis               |
| H264 Video Decoder |                     NALU-to-YUV converting                     |               Vitis               |
|        CNN         |                       Digit Recognition                        |  Vivado-HLS, Dataflow, streaming  |
|    CORDIC_SQRT     |                       CORDIC for L2-norm                       |      PYNQ, Vivado-HLS, AXIS       |
|      FP_ACCUM      |                   Adder-tree, floating-point                   |            PYNQ, AXI_M            |

## Operating instructions
1. Run HLS solution on Vivado_hls prompt:
```
vivado_hls -f hls_script.tcl
```
2. Run Vivado:
```
source vvd_script.tcl
```
3. Rename bit & hwh file: create "burn_file" folder and run MakeBit.bat
4. Run the pynq host program *.ipynb on jupyter notebook