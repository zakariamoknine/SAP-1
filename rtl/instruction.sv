module instruction (
    input wire clk,
    input wire clear,
    input wire write_enable,
    input wire output_enable,

    output wire[3:0] instruction_operation_code,

    inout wire[7:0] instruction_data_bus
);

    reg[7:0] instruction_buffer;

    always @(posedge clk) begin
        if (clear) begin
            instruction_buffer <= 8'b0;
        end else if (write_enable) begin
            instruction_buffer <= instruction_data_bus;
        end
    end

    assign instruction_operation_code = instruction_buffer[7:4];
    assign instruction_data_bus = (output_enable & ~write_enable) ? instruction_buffer : 8'bZ;

endmodule
