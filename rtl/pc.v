module pc (
    input wire clk,
    input wire clear,
    input wire output_enable,

    input wire increment,

    output wire[7:0] output_bus_connection
);

    reg[7:0] pc_reg;

    always @(posedge clk or posedge clear) begin
        if (clear) begin
            pc_reg <= 8'b0;
        end else if (increment) begin
            pc_reg <= pc_reg + 1;
        end
    end

    assign output_bus_connection = output_enable ? pc_reg : 8'bZ;

endmodule
