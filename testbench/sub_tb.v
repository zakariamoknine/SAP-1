`timescale 1ns/1ps

module sub_tb;

    reg clk = 0;
    reg clear = 1;

    cpu cpu_instance (
        .clk(clk),
        .clear(clear)
    );

    always #5 clk = ~clk;

    initial begin
        $dumpfile("out/sub_wave.vcd");
        $dumpvars(0, sub_tb);

        clear = 1;

        $readmemb("testbench/binary/sub.bin", cpu_instance.ram_instance.ram_memory);

        #20;
        clear = 0;

        #1000;
        $display("\n=== Testing: sub.bin ============\n");
        $display("OPERATION: 215 - 79");
        $display("OUTPUT: %0d", cpu_instance.outport_display);
        $display("\n=================================\n");

        $finish;
    end

endmodule
