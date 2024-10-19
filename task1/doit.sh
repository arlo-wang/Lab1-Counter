#!/bin/sh

# cleanup
rm -rf obj_dir
rm -f counter.vcd

# run verilator to translate Verilog into C++, including C++ testbench
verilator -Wall --cc --trace counter.sv --exe counter_tb.cpp

# build C++ project via make automatically generated by Verilator
make -j -C obj_dir/ -f Vcounter.mk Vcounter

# run excutable simulation file
obj_dir/Vcounter

#list the name of the Vbuddy
ls /dev/tty.u*