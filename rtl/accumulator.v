module accumulator (
    input wire clk,
    input wire write_enable,
    input wire output_enable,

    output wire[7:0] arithmetic_value,

    inout wire[7:0] inout_bus_connection
);

    reg[7:0] accumulator_reg;

    always @(posedge clk) begin
        if (write_enable) begin
            accumulator_reg <= inout_bus_connection;
        end
    end

    assign arithmetic_value = accumulator_reg;
    assign inout_bus_connection = (output_enable & ~write_enable) ? accumulator_reg : 8'bZ;

endmodule
