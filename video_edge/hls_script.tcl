open_project hls_video_edge
set_top image_filter
add_files image_filter.cpp
add_files -tb image_filter_test.cpp
add_files -tb checkerboard.bmp
#
open_solution "solution1"
set_part {xc7z020clg484-1}
create_clock -period 6 -name default
csim_design
csynth_design
cosim_design -trace_level all -tool xsim
export_design -rtl verilog -format ip_catalog
exit

