RTLSRC := $(wildcard rtl/*.sv)


all:

test:
	mkdir -p out
	iverilog $(RTLSRC) testbench/arithmetics_tb.sv -o out/arithmetics_design
	vvp out/arithmetics_design

%:
	mkdir -p out
	iverilog rtl/$*.sv testbench/$*_tb.sv -o out/$*_design
	vvp out/$*_design

show_%:
	gtkwave out/$*_wave.vcd

clean:
	rm -rf out


.PHONY: all clean
