module instruction (
    input wire clk,
    input wire clear,
    input wire write_enable,
    input wire output_enable,

    output wire[3:0] operation_code,

    inout wire[7:0] inout_bus_connection
);

    reg[7:0] instruction_buffer;

    always @(posedge clk) begin
        if (clear) begin
            instruction_buffer <= 8'b0;
        end else if (write_enable) begin
            instruction_buffer <= inout_bus_connection;
        end
    end

    assign operation_code = instruction_buffer[7:4];
    assign inout_bus_connection = (output_enable & ~write_enable) ? instruction_buffer : 8'bZ;

endmodule
