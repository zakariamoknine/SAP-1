import cocotb
from cocotb.triggers import RisingEdge
from .init import init_test

@cocotb.test()
async def test_sub(dut):
    await init_test(dut, "build/bin/sub.bin")

    for _ in range(100):
        await RisingEdge(dut.clk)

    output = dut.outport_instance.binary_display.value.integer
    cocotb.log.info(f"* TESTING: 215 - 79")
    cocotb.log.info(f"* OUTPUT: {output}")

    assert output == 136, f"Expected output 85, but got {output}"
