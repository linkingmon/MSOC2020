# Specify vivado project filename
set FOLDER Lab1
set FILENAME vvd_Multip2Num_impl
set DesignName hls_Multiplication_impl

# Create project
create_project $FILENAME C:/Users/USER/Desktop/MSOC2020/$FOLDER/$FILENAME -part xc7z020clg484-1

# Update IP
set_property board_part em.avnet.com:zed:part0:1.4 [current_project]
set_property  ip_repo_paths  C:/Users/USER/Desktop/MSOC2020/$FOLDER/$DesignName [current_project]
update_ip_catalog

# Create block design
create_bd_design "design_1"
update_compile_order -fileset sources_1
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:processing_system7:5.5 processing_system7_0
endgroup
update_compile_order -fileset sources_1
startgroup
create_bd_cell -type ip -vlnv xilinx.com:hls:multip_2num:1.0 multip_2num_0
endgroup

# Connection
apply_bd_automation -rule xilinx.com:bd_rule:processing_system7 -config {make_external "FIXED_IO, DDR" apply_board_preset "1" Master "Disable" Slave "Disable" }  [get_bd_cells processing_system7_0]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/processing_system7_0/M_AXI_GP0} Slave {/multip_2num_0/s_axi_AXILiteS} ddr_seg {Auto} intc_ip {New AXI Interconnect} master_apm {0}}  [get_bd_intf_pins multip_2num_0/s_axi_AXILiteS]
regenerate_bd_layout

# Wrap
make_wrapper -files [get_files C:/Users/USER/Desktop/MSOC2020/Lab1/vvd_Multip2Num_impl/vvd_Multip2Num_impl.srcs/sources_1/bd/design_1/design_1.bd] -top

# Implement
add_files -norecurse C:/Users/USER/Desktop/MSOC2020/Lab1/vvd_Multip2Num_impl/vvd_Multip2Num_impl.srcs/sources_1/bd/design_1/hdl/design_1_wrapper.v
launch_runs impl_1 -to_step write_bitstream -jobs 4
open_run impl_1

report_timing_summary -delay_type min_max -report_unconstrained -check_timing_verbose -max_paths 10 -input_pins -routable_nets -name timing_1