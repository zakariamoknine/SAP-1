module mar (
    input wire clk,
    input wire write_enable,

    input wire[7:0] mar_input_bus,

    output wire[3:0] mar_address_value
);

    reg[3:0] mar_buffer;

    always @(posedge clk) begin
        if (write_enable) begin
           mar_buffer <= mar_input_bus[3:0];
        end
    end

    assign mar_address_value = mar_buffer;

endmodule
