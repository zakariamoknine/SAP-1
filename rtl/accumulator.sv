module accumulator (
    input wire clk,
    input wire write_enable,
    input wire output_enable,

    output wire[7:0] accumulator_arith_value,

    inout wire[7:0] accumulator_data_bus
);

    reg[7:0] accumulator_buffer;

    always @(posedge clk) begin
        if (write_enable) begin
            accumulator_buffer <= accumulator_data_bus;
        end
    end

    assign accumulator_arith_value = accumulator_buffer;
    assign accumulator_data_bus = (output_enable & ~write_enable) ? accumulator_buffer : 8'bZ;

endmodule
