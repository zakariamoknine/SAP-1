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
    wire pc_increment;

    wire mar_write_enable;
    wire[3:0] mar_address_value;

    wire ram_output_enable;

    wire accumulator_write_enable;
    wire accumulator_output_enable;
    wire[7:0] accumulator_arithmetic_value;

    wire register_write_enable;
    wire[7:0] register_arithmetic_value;

    wire alu_output_enable;
    wire alu_operation_selection;

    wire outport_write_enable;
    wire[7:0] outport_display;

    controller controller_instance (
        .clk(clk),
        .clear(clear),
        .instruction_operation_code(instruction_operation_code),
        .accumulator_output_enable(accumulator_output_enable),
        .accumulator_write_enable(accumulator_write_enable),
        .alu_output_enable(alu_output_enable),
        .alu_operation_selection(alu_operation_selection),
        .instruction_write_enable(instruction_write_enable),
        .instruction_output_enable(instruction_output_enable),
        .mar_write_enable(mar_write_enable),
        .outport_write_enable(outport_write_enable),
        .pc_output_enable(pc_output_enable),
        .pc_increment(pc_increment),
        .ram_output_enable(ram_output_enable),
        .register_write_enable(register_write_enable)
    );

    instruction instruction_instance (
        .clk(clk),
        .clear(clear),
        .write_enable(instruction_write_enable),
        .output_enable(instruction_output_enable),
        .operation_code(instruction_operation_code),
        .inout_bus_connection(main_data_bus)
    );

    pc pc_instance (
        .clk(clk),
        .clear(clear),
        .output_enable(pc_output_enable),
        .increment(pc_increment),
        .output_bus_connection(main_data_bus)
    );

    mar mar_instance (
        .clk(clk),
        .write_enable(mar_write_enable),
        .input_bus_connection(main_data_bus),
        .address_value(mar_address_value)
    );

    ram ram_instance (
        .output_enable(ram_output_enable),
        .memory_address(mar_address_value),
        .output_bus_connection(main_data_bus)
    );

    accumulator accumulator_instance (
        .clk(clk),
        .write_enable(accumulator_write_enable),
        .output_enable(accumulator_output_enable),
        .arithmetic_value(accumulator_arithmetic_value),
        .inout_bus_connection(main_data_bus)
    );

    register register_instance (
        .clk(clk),
        .write_enable(register_write_enable),
        .input_bus_connection(main_data_bus),
        .arithmetic_value(register_arithmetic_value)
    );

    alu alu_instance (
        .output_enable(alu_output_enable),
        .operation_selection(alu_operation_selection),
        .accumulator_arithmetic_value(accumulator_arithmetic_value),
        .register_arithmetic_value(register_arithmetic_value),
        .output_bus_connection(main_data_bus)
    );

    outport outport_instance (
        .clk(clk),
        .write_enable(outport_write_enable),
        .input_bus_connection(main_data_bus),
        .binary_display(outport_display)
    );

endmodule
