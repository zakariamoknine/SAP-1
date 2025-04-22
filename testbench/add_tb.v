`timescale 1ns/1ps

module add_tb;

    reg clk = 0;
    reg clear = 1;

    cpu cpu_instance (
        .clk(clk),
        .clear(clear)
    );

    always #5 clk = ~clk;

    initial begin
        $dumpfile("out/add_wave.vcd");
        $dumpvars(0, add_tb);
        $readmemb("out/bin/add.bin", cpu_instance.ram_instance.ram_memory);

        clear = 1;

        #20;

        clear = 0;

        #1000;

        $display("\n=== Testing: add.bin ============\n");
        $display("OPERATION: 19 + 43");
        $display("OUTPUT: %0d", cpu_instance.outport_display);
        $display("\n=================================\n");

        $finish;
    end

endmodule
