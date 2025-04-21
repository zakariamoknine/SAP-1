RTLSRC := $(wildcard rtl/*.sv)

all: test_add test_sub test_arith

assembler:
	make -C assembler

%:
	mkdir -p out
	iverilog rtl/$*.sv testbench/$*_tb.sv -o out/$*_design
	vvp out/$*_design

test_%: assembler
	mkdir -p testbench/binary
	./out/bin/sap1-asm testbench/assembly/$*.asm testbench/binary/$*.bin
	iverilog $(RTLSRC) testbench/$*_tb.sv -o out/$*_design
	vvp out/$*_design

show_%:
	gtkwave out/$*_wave.vcd

clean:
	rm -rf out

.PHONY: all assembler clean
