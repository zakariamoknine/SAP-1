module cpu (
    input wire clk,
    input wire clear
);

    // 8-bit bus shared between all digital units
    wire [7:0] main_data_bus;

    wire instruction_write_enable;
    wire instruction_output_enable;
    wire[3:0] instruction_operation_code;

    wire pc_output_enable;
    wire pc_pc_increment;

    wire mar_write_enable;
    wire[3:0] mar_address_value;

    wire ram_output_enable;

    wire accumulator_write_enable;
    wire accumulator_output_enable;
    wire[7:0] accumulator_arith_value;

    wire register_write_enable;
    wire[7:0] register_arith_value;

    wire alu_output_enable;
    wire alu_decoded_alu_mux;

    wire outport_write_enable;
    wire[7:0] outport_display;

    controller controller_instance (
        .clk(clk),
        .clear(clear),
        .instruction_operation_code(instruction_operation_code),
        .accumulator_output_enable(accumulator_output_enable),
        .accumulator_write_enable(accumulator_write_enable),
        .alu_output_enable(alu_output_enable),
        .alu_decoded_alu_mux(alu_decoded_alu_mux),
        .instruction_write_enable(instruction_write_enable),
        .instruction_output_enable(instruction_output_enable),
        .mar_write_enable(mar_write_enable),
        .outport_write_enable(outport_write_enable),
        .pc_output_enable(pc_output_enable),
        .pc_pc_increment(pc_pc_increment),
        .ram_output_enable(ram_output_enable),
        .register_write_enable(register_write_enable)
    );

    instruction instruction_instance (
        .clk(clk),
        .clear(clear),
        .write_enable(instruction_write_enable),
        .output_enable(instruction_output_enable),
        .instruction_operation_code(instruction_operation_code),
        .instruction_data_bus(main_data_bus)
    );

    pc pc_instance (
        .clk(clk),
        .clear(clear),
        .output_enable(pc_output_enable),
        .pc_increment(pc_pc_increment),
        .pc_output_bus(main_data_bus)
    );

    mar mar_instance (
        .clk(clk),
        .write_enable(mar_write_enable),
        .mar_input_bus(main_data_bus),
        .mar_address_value(mar_address_value)
    );

    ram ram_instance (
        .output_enable(ram_output_enable),
        .memory_address(mar_address_value),
        .ram_output_bus(main_data_bus)
    );

    accumulator accumulator_instance (
        .clk(clk),
        .write_enable(accumulator_write_enable),
        .output_enable(accumulator_output_enable),
        .accumulator_arith_value(accumulator_arith_value),
        .accumulator_data_bus(main_data_bus)
    );

    register register_instance (
        .clk(clk),
        .write_enable(register_write_enable),
        .register_input_bus(main_data_bus),
        .register_arith_value(register_arith_value)
    );

    alu alu_instance (
        .output_enable(alu_output_enable),
        .decoded_alu_mux(alu_decoded_alu_mux),
        .accumulator_value(accumulator_arith_value),
        .register_value(register_arith_value),
        .alu_output_bus(main_data_bus)
    );

    outport outport_instance (
        .clk(clk),
        .write_enable(outport_write_enable),
        .outport_input_bus(main_data_bus),
        .binary_display(outport_display)
    );

endmodule
