ifdef O
	BUILD_DIR=$(shell readlink -m $(O))
else
	BUILD_DIR=$(CURDIR)/../build/bin
endif

include ../Makefile.conf

all:
	mkdir -p $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)/$(SAP1ASM) asm.c

.PHONY: all clean
