module register (
    input wire clk,
    input wire write_enable,

    input wire[7:0] input_bus_connection,

    output wire[7:0] arithmetic_value
);

    reg[7:0] register_buffer;

    always @(posedge clk) begin
        if (write_enable) begin
           register_buffer <= input_bus_connection;
        end
    end

    assign arithmetic_value = register_buffer;

endmodule
