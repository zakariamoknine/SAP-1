module pc (
    input wire clk,
    input wire clear,
    input wire output_enable,

    input wire increment,

    output wire[7:0] output_bus_connection
);

    reg[7:0] pc_buffer;

    always @(posedge clk or posedge clear) begin
        if (clear) begin
            pc_buffer <= 8'b0;
        end else if (increment) begin
            pc_buffer <= pc_buffer + 1;
        end
    end

    assign output_bus_connection = output_enable ? pc_buffer : 8'bZ;

endmodule
