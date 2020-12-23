############# ORIGINAL C CODE PURELY SEQUENTIAL #####################
# solution 1: pipeline accumulator
open_project hls_fp_acc_prj
set_top hls_fp_accumulator
add_files fp_accum.cpp
open_solution "solution1"
set_part {xc7z020clg484-1}
add_files -tb fp_accum.cpp
create_clock -period 10 -name default
csim_design -clean
csynth_design
close_project

# solution2: adder tree
open_project hls_fp_acc_prj
set_top hls_fp_accumulator
add_files fp_accum.cpp -cflags "-DDB_OPTIMIZED"
add_files -tb fp_accum.cpp
open_solution "solution2"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
csim_design -clean
csynth_design
close_project

# solution3: adder tree + array partition
open_project hls_fp_acc_prj
set_top hls_fp_accumulator
add_files fp_accum.cpp -cflags "-DDB_OPTIMIZED -DPARTITION"
add_files -tb fp_accum.cpp
open_solution "solution3"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
csim_design -clean
csynth_design
csynth_design
close_project

# solution4: solution3 + interface
open_project hls_fp_acc_prj
set_top hls_fp_accumulator
add_files fp_accum.cpp -cflags "-DINTERFACE"
add_files -tb fp_accum.cpp
open_solution "solution4"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
csim_design -clean
csynth_design
cosim_design -ldflags {{-Wl,--stack=268435456}} -trace_level all -tool xsim
export_design -rtl verilog -format ip_catalog
close_project
quit