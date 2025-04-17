RTLSRC := $(wildcard rtl/*.sv)

all: test_add test_sub

%:
	mkdir -p out
	iverilog rtl/$*.sv testbench/$*_tb.sv -o out/$*_design
	vvp out/$*_design

test_%:
	mkdir -p out
	iverilog $(RTLSRC) testbench/$*_tb.sv -o out/$*_design
	vvp out/$*_design

show_%:
	gtkwave out/$*_wave.vcd

clean:
	rm -rf out

.PHONY: all test clean
