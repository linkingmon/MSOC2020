# Specify vivado project filename
set FOLDER Lab2-1
set FILENAME vvd_FIRN11MAXI
set DesignName hls_FIRN11MAXI

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

startgroup
create_bd_cell -type ip -vlnv xilinx.com:hls:fir_n11_maxi:1.0 fir_n11_maxi_0
endgroup

apply_bd_automation -rule xilinx.com:bd_rule:processing_system7 -config {make_external "FIXED_IO, DDR" apply_board_preset "1" Master "Disable" Slave "Disable" }  [get_bd_cells processing_system7_0]

startgroup
set_property -dict [list CONFIG.PCW_FPGA0_PERIPHERAL_FREQMHZ {100} CONFIG.PCW_USE_S_AXI_HP0 {1} CONFIG.PCW_QSPI_GRP_SINGLE_SS_ENABLE {1}] [get_bd_cells processing_system7_0]
endgroup

startgroup
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/processing_system7_0/M_AXI_GP0} Slave {/fir_n11_maxi_0/s_axi_AXILiteS} ddr_seg {Auto} intc_ip {New AXI Interconnect} master_apm {0}}  [get_bd_intf_pins fir_n11_maxi_0/s_axi_AXILiteS]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/fir_n11_maxi_0/m_axi_gmem} Slave {/processing_system7_0/S_AXI_HP0} ddr_seg {Auto} intc_ip {New AXI Interconnect} master_apm {0}}  [get_bd_intf_pins processing_system7_0/S_AXI_HP0]
endgroup

make_wrapper -files [get_files C:/Users/USER/Desktop/MSOC2020/$FOLDER/$FILENAME/$FILENAME.srcs/sources_1/bd/design_1/design_1.bd] -top
add_files -norecurse C:/Users/USER/Desktop/MSOC2020/$FOLDER/$FILENAME/$FILENAME.srcs/sources_1/bd/design_1/hdl/design_1_wrapper.v

launch_runs impl_1 -to_step write_bitstream -jobs 4

# Report Timing
# open_run impl_1
# report_timing_summary -delay_type min_max -report_unconstrained -check_timing_verbose -max_paths 10 -input_pins -routable_nets -name timing_1
