module mar (
    input wire clk,
    input wire write_enable,

    input wire[7:0] input_bus_connection,

    output wire[3:0] address_value
);

    reg[3:0] mar_reg;

    always @(posedge clk) begin
        if (write_enable) begin
           mar_reg <= input_bus_connection[3:0];
        end
    end

    assign address_value = mar_reg;

endmodule
