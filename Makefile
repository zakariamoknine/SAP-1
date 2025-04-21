RTLSRC := $(wildcard rtl/*.v)
ASMBIN := sap1-asm

all: test_add test_sub test_arith

assembler:
	make -C assembler

test_%: assembler
	mkdir -p testbench/binary
	./out/bin/$(ASMBIN) testbench/assembly/$*.asm testbench/binary/$*.bin
	iverilog $(RTLSRC) testbench/$*_tb.v -o out/$*_design
	vvp out/$*_design

show_%:
	gtkwave out/$*_wave.vcd

clean:
	rm -rf out

.PHONY: all assembler clean
