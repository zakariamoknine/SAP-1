include Makefile.conf

RTLSRC := $(wildcard rtl/*.v)

all: test_add test_sub test_arith

compile:
	mkdir -p build/bin/
	$(MAKE) -C assembler
	iverilog -o build/cpu_design.vvp -s cpu -g2012 $(RTLSRC)

test_%: compile
	./build/bin/$(SAP1ASM) testbench/assembly/$*.asm build/bin/$*.bin
	MODULE=testbench.test_$* vvp -M $$(cocotb-config --prefix)/cocotb/libs -m libcocotbvpi_icarus build/cpu_design.vvp

clean:
	rm -rf build/ results.xml

.PHONY: all compile clean
