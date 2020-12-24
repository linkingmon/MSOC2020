#### solution1: original
open_project hls_RSE_prj
add_files rs_erasure.c
add_files -tb  tb_rs_erasure.c
add_files -tb  tv/tv_rs_erasure_in.txt
add_files -tb  tv/tv_rs_erasure_mout.txt
set_top rs_erasure
open_solution "solution1"
set_part {xc7z020clg484-1}
create_clock -period 3.3

set_directive_interface -mode m_axi -depth 4 "rs_erasure" c
set_directive_interface -mode m_axi -depth 12 "rs_erasure" d
set_directive_interface -mode s_axilite "rs_erasure" survival_pattern
set_directive_interface -mode s_axilite "rs_erasure" codeidx
csim_design
csynth_design
close_project


#### solution2: pipeline
open_project hls_RSE_prj
add_files rs_erasure.c
add_files -tb  tb_rs_erasure.c
add_files -tb  tv/tv_rs_erasure_in.txt
add_files -tb  tv/tv_rs_erasure_mout.txt
set_top rs_erasure
open_solution "solution2"
set_part {xc7z020clg484-1}
create_clock -period 3.3

set_directive_interface -mode m_axi -depth 4 "rs_erasure" c
set_directive_interface -mode m_axi -depth 12 "rs_erasure" d
set_directive_interface -mode s_axilite "rs_erasure" survival_pattern
set_directive_interface -mode s_axilite "rs_erasure" codeidx
set_directive_pipeline "rs_erasure/rs_erasure_label0"
set_directive_pipeline "rs_erasure/rs_erasure_label1"
set_directive_pipeline "rs_erasure/rs_erasure_label2"
csim_design
csynth_design
close_project

#### solution3: code restructure
open_project hls_RSE_prj
add_files rs_erasure.c -cflags "-DOPTIMIZE"
add_files -tb  tb_rs_erasure.c
add_files -tb  tv/tv_rs_erasure_in.txt
add_files -tb  tv/tv_rs_erasure_mout.txt
set_top rs_erasure
open_solution "solution3"
set_part {xc7z020clg484-1}
create_clock -period 3.3

set_directive_interface -mode m_axi -depth 4 "rs_erasure" c
set_directive_interface -mode m_axi -depth 12 "rs_erasure" d
set_directive_interface -mode s_axilite "rs_erasure" survival_pattern
set_directive_interface -mode s_axilite "rs_erasure" codeidx
set_directive_pipeline "rs_erasure/rs_erasure_label0"
set_directive_pipeline "rs_erasure/rs_erasure_label1"
set_directive_pipeline "rs_erasure/rs_erasure_label2"
csim_design
csynth_design
close_project

#### solution4: code restructure
open_project hls_RSE_prj
add_files rs_erasure.c
add_files -tb  tb_rs_erasure.c
add_files -tb  tv/tv_rs_erasure_in.txt
add_files -tb  tv/tv_rs_erasure_mout.txt
set_top rs_erasure
open_solution "solution4"
set_part {xc7z020clg484-1}
create_clock -period 3.3


set_directive_array_partition -type complete "rs_erasure" c
set_directive_array_partition -type complete "rs_erasure" d
set_directive_interface -mode s_axilite "rs_erasure" c
set_directive_interface -mode s_axilite "rs_erasure" d
set_directive_interface -mode s_axilite "rs_erasure" survival_pattern
set_directive_interface -mode s_axilite "rs_erasure" codeidx
set_directive_pipeline -II 1  rs_erasure
set_directive_interface -mode s_axilite "rs_erasure"
csim_design
csynth_design
close_project
cosim_design -trace_level all -tool xsim
export_design -rtl verilog -format ip_catalog
quit