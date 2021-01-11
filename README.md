# MSOC2020


## Overview
|    Project name    |                 Content                  |          Related Topics           | Slides | Report |
|:------------------:|:----------------------------------------:|:---------------------------------:|:------:|:------:|
|        Lab1        |      Vivado-HLS & Vivado work flow       |   Vivado-HLS, PYNQ, Multiplier    |   V    |   V    |
|  Lab2-1 & Lab2-2   |  AXI-Master, AXI-Stream interface, FIR   |           AXI-M, AXI-S            |   *    |   V    |
|     Video_edge     | Video Edge detection, OpenCV-HLS library | OpenCV-hls, PYNQ, video-streaming |   V    |   V    |
|        Lab3        |            XRT, HW emulation             |               Vitis               |        |   V    |
| H264 Video Decoder |          NALU-to-YUV converting          |               Vitis               |   V    |        |
|        CNN         |            Digit Recognition             |  Vivado-HLS, Dataflow, streaming  |   V    |        |
|    CORDIC_SQRT     |            CORDIC for L2-norm            |      PYNQ, Vivado-HLS, AXIS       |        |   V    |
|      FP_ACCUM      |        Adder-tree, floating-point        |            PYNQ, AXI_M            |        |   V    |
|        RSE         |           Reed Solomon erasure           |    PYNQ, AXILite, Pipeline, II    |        |   V    |
\* Slides of Lab2 is in Lab1/
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