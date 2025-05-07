module outport (
    input wire clk,
    input wire write_enable,

    input wire[7:0] input_bus_connection,

    output wire[7:0] binary_display
);

    reg[7:0] outport_reg;

    always @(posedge clk) begin
        if (write_enable) begin
            outport_reg <= input_bus_connection;
        end
    end

    assign binary_display = outport_reg;

endmodule
