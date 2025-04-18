module accumulator (
    input wire clk,
    input wire write_enable,
    input wire output_enable,

    output wire[7:0] arithmetic_value,

    inout wire[7:0] inout_bus_connection
);

    reg[7:0] accumulator_buffer;

    always @(posedge clk) begin
        if (write_enable) begin
            accumulator_buffer <= inout_bus_connection;
        end
    end

    assign arithmetic_value = accumulator_buffer;
    assign inout_bus_connection = (output_enable & ~write_enable) ? accumulator_buffer : 8'bZ;

endmodule
