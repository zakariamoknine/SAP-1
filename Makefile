RTLSRC := $(wildcard rtl/*.v)

all: test_add test_sub test_arith

compile:
	mkdir -p out/bin/
	make -C assembler
	iverilog -o out/cpu_design.vvp -s cpu -g2012 $(RTLSRC)

test_%: compile
	./out/bin/sap1-asm testbench/assembly/$*.asm out/bin/$*.bin
	MODULE=testbench.test_$* vvp -M $$(cocotb-config --prefix)/cocotb/libs -m libcocotbvpi_icarus out/cpu_design.vvp

clean:
	rm -rf out/ results.xml

.PHONY: all compile clean
