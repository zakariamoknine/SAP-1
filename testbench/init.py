import os
import cocotb
from cocotb.triggers import RisingEdge
from cocotb.clock import Clock

async def init_test(dut, program_path):
    assert os.path.exists(program_path), f"{program_path} not found!"

    with open(program_path, 'r') as f:
        program_lines = f.readlines()

    for i, line in enumerate(program_lines):
        value = int(line.strip(), 2)
        dut.ram_instance.ram_memory[i].value = value

    clock = Clock(dut.clk, 10, units="ns")
    cocotb.start_soon(clock.start())

    dut.clear.value = 1
    await RisingEdge(dut.clk)
    dut.clear.value = 0
