module outport (
    input wire clk,
    input wire write_enable,

    input wire[7:0] outport_input_bus,

    output wire[7:0] binary_display
);

    reg[7:0] outport_buffer;

    always @(posedge clk) begin
        if (write_enable) begin
            outport_buffer <= outport_input_bus;
        end
    end

    assign binary_display = outport_buffer;

endmodule
