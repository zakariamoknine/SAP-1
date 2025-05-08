import cocotb
from cocotb.triggers import RisingEdge
from .init import init_test

@cocotb.test()
async def test_arith(dut):
    await init_test(dut, "out/bin/arith.bin")

    for _ in range(100):
        await RisingEdge(dut.clk)

    output = dut.outport_instance.binary_display.value.integer
    cocotb.log.info(f"* TESTING: 65 + 98 - 17 + 107 - 144 + 54 - 78")
    cocotb.log.info(f"* OUTPUT: {output}")

    assert output == 85, f"Expected output 136, but got {output}"
