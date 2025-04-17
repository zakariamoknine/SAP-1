module alu (
    input wire output_enable,

    input wire decoded_alu_mux,

    input wire[7:0] accumulator_value,
    input wire[7:0] register_value,

    output wire[7:0] alu_output_bus
);

    reg[7:0] alu_result;

    localparam ADD = 1'b0;
    localparam SUB = 1'b1;

    always @(*) begin
        case (decoded_alu_mux)
            ADD: begin
                alu_result <= accumulator_value + register_value;
            end
            SUB: begin
                alu_result <= accumulator_value - register_value;
            end
        endcase
    end

    assign alu_output_bus = output_enable ? alu_result : 8'bZ;

endmodule
