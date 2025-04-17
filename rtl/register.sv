module register (
    input wire clk,
    input wire write_enable,

    input wire[7:0] register_input_bus,

    output wire[7:0] register_arith_value
);

    reg[7:0] register_buffer;

    always @(posedge clk) begin
        if (write_enable) begin
           register_buffer <= register_input_bus;
        end
    end

    assign register_arith_value = register_buffer;

endmodule
