module ram (
    input wire output_enable,

    input wire[3:0] memory_address,

    output wire[7:0] output_bus_connection
);

    reg[7:0] ram_memory[0:15];

    assign output_bus_connection = output_enable ? ram_memory[memory_address] : 8'bZ; 

endmodule
