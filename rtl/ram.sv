module ram (
    input wire output_enable,

    input wire[3:0] memory_address,

    output wire[7:0] ram_output_bus
);

    reg[7:0] ram_memory[0:15];

    assign ram_output_bus = output_enable ? ram_memory[memory_address] : 8'bZ; 

endmodule
