module alu (
    input wire output_enable,

    input wire operation_selection,

    input wire[7:0] accumulator_arithmetic_value,
    input wire[7:0] register_arithmetic_value,

    output wire[7:0] output_bus_connection
);

    reg[7:0] alu_result;

    localparam ADD = 1'b0;
    localparam SUB = 1'b1;

    always @(*) begin
        case (operation_selection)
            ADD: begin
                alu_result <= accumulator_arithmetic_value + register_arithmetic_value;
            end
            SUB: begin
                alu_result <= accumulator_arithmetic_value - register_arithmetic_value;
            end
        endcase
    end

    assign output_bus_connection = output_enable ? alu_result : 8'bZ;

endmodule
