`timescale 1ns/1ps

module arith_tb;

    reg clk = 0;
    reg clear = 1;

    cpu cpu_instance (
        .clk(clk),
        .clear(clear)
    );

    always #5 clk = ~clk;

    initial begin
        $dumpfile("out/arith_wave.vcd");
        $dumpvars(0, arith_tb);

        $readmemb("out/bin/arith.bin", cpu_instance.ram_instance.ram_memory);

        clear = 1;

        #20;

        clear = 0;

        #1000;

        $display("\n=== Testing: arith.bin ==========\n");
        $display("OPERATION: 65 + 98 - 17 + 107 - 144 + 54 - 78");
        $display("OUTPUT: %0d", cpu_instance.outport_display);
        $display("\n=================================\n");

        $finish;
    end

endmodule
