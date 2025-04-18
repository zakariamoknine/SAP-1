module controller (
    input wire clk,
    input wire clear,

    input wire[3:0] instruction_operation_code,

    output reg accumulator_output_enable,
    output reg accumulator_write_enable,

    output reg alu_output_enable,
    output reg alu_operation_selection,

    output reg instruction_write_enable,
    output reg instruction_output_enable,

    output reg mar_write_enable,

    output reg outport_write_enable,

    output reg pc_output_enable,
    output reg pc_increment,

    output reg ram_output_enable,

    output reg register_write_enable
);

    // states
    localparam T1 = 3'b001;
    localparam T2 = 3'b010;
    localparam T3 = 3'b011;
    localparam T4 = 3'b100;
    localparam T5 = 3'b101;
    localparam T6 = 3'b110;

    // operation codes
    localparam LDA = 4'b0000;
    localparam ADD = 4'b0001;
    localparam SUB = 4'b0010;
    localparam OUT = 4'b1110;
    localparam HLT = 4'b1111;

    reg[2:0] state;

    always @(negedge clk or posedge clear) begin
        if (clear) begin
            state <= 3'b000;
        end else begin
            if (state == T6) begin
                state <= T1;
                // NOTE: this is a simulation-only hack to freeze the CPU when
                // HLT is executed, MUST be removed in real FPGA systhesis and
                // replaced with something like clock gating.
                if (instruction_operation_code == HLT) begin
                    state <= T6;
                end
            end else begin
                state <= state + 1;
            end
        end
    end

    always @(*) begin
        // init all conditions to 0
        accumulator_output_enable = 1'b0;
        accumulator_write_enable = 1'b0;
        alu_output_enable = 1'b0;
        alu_operation_selection = 1'b0;
        instruction_output_enable = 1'b0;
        mar_write_enable = 1'b0;
        outport_write_enable = 1'b0;
        pc_output_enable = 1'b0;
        pc_increment = 1'b0;
        register_write_enable = 1'b0;
        instruction_write_enable = 1'b0;
        ram_output_enable = 1'b0;

        case (state)
            T1: begin
                pc_output_enable = 1'b1;
                mar_write_enable = 1'b1;
            end
            T2: begin
                pc_increment = 1'b1;
            end
            T3: begin
                instruction_write_enable = 1'b1;
                ram_output_enable = 1'b1;
            end
            T4: begin
                case (instruction_operation_code)
                    LDA: begin
                        mar_write_enable = 1'b1;
                        instruction_output_enable = 1'b1;
                    end
                    ADD: begin
                        mar_write_enable = 1'b1;
                        instruction_output_enable = 1'b1;
                    end
                    SUB: begin
                        mar_write_enable = 1'b1;
                        instruction_output_enable = 1'b1;
                    end
                    OUT: begin
                        outport_write_enable = 1'b1;
                        accumulator_output_enable = 1'b1;
                    end
                    HLT: begin
                        // do nothing
                    end
                endcase
            end
            T5: begin
                case (instruction_operation_code)
                    LDA: begin
                        ram_output_enable = 1'b1;
                        accumulator_write_enable = 1'b1;
                    end
                    ADD: begin
                        ram_output_enable = 1'b1;
                        register_write_enable = 1'b1;
                    end
                    SUB: begin
                        ram_output_enable = 1'b1;
                        register_write_enable = 1'b1;
                    end
                    OUT: begin
                        // do nothing
                    end
                    HLT: begin
                        // do nothing
                    end
                endcase
            end
            T6: begin
                case (instruction_operation_code)
                    LDA: begin
                        // do nothing
                    end
                    ADD: begin
                        alu_operation_selection = 1'b0; // ALU_ADD: 0, ALU_SUB: 1
                        alu_output_enable = 1'b1;
                        accumulator_write_enable = 1'b1;
                    end
                    SUB: begin
                        alu_operation_selection = 1'b1; // ALU_ADD: 0, ALU_SUB: 1
                        alu_output_enable = 1'b1;
                        accumulator_write_enable = 1'b1;
                    end
                    OUT: begin
                        // do nothing
                    end
                    HLT: begin
                        // do nothing
                    end
                endcase
            end
        endcase
    end

endmodule
