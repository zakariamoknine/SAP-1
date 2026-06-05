import cocotb
from cocotb.triggers import RisingEdge
from .init import init_test

@cocotb.test()
async def test_add(dut):
    await init_test(dut, "build/bin/add.bin")

    for _ in range(100):
        await RisingEdge(dut.clk)

    output = dut.outport_instance.binary_display.value.integer
    cocotb.log.info(f"* TESTING: 16 + 64")
    cocotb.log.info(f"* OUTPUT: {output}")

    assert output == 62, f"Expected output 62, but got {output}"
