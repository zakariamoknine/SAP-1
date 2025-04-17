module pc (
    input wire clk,
    input wire clear,
    input wire output_enable,

    input wire pc_increment,

    output wire[7:0] pc_output_bus
);

    reg[7:0] pc_buffer;

    always @(posedge clk or posedge clear) begin
        if (clear) begin
            pc_buffer <= 8'b0;
        end else if (pc_increment) begin
            pc_buffer <= pc_buffer + 1;
        end
    end

    assign pc_output_bus = output_enable ? pc_buffer : 8'bZ;

endmodule
