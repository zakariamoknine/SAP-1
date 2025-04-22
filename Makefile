RTLSRC := $(wildcard rtl/*.v)
ASMBIN := sap1-asm

all: test_add test_sub test_arith

asm:
	make -C assembler

test_%: asm
	mkdir -p out/bin/
	./out/bin/$(ASMBIN) testbench/assembly/$*.asm out/bin/$*.bin
	iverilog $(RTLSRC) testbench/$*_tb.v -o out/$*_design
	vvp out/$*_design

show_%:
	gtkwave out/$*_wave.vcd

clean:
	rm -rf out

.PHONY: all asm clean
