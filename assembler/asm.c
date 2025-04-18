#include <stdio.h>
#include <stdlib.h>

static const char* filepath;

void read_assembly_file()
{

}

void write_binary_output()
{

}

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        fprintf(stderr, "ERROR: No input, provide a single assembly file\n");
        return -1;
    }
    else if (argc > 2)
    {
        fprintf(stderr, "ERROR: Too many arguments, provide a single asssembly file\n");
        return -1;
    }

    filepath = argv[1];

    read_assembly_file();

    write_binary_output();

    return 0;
}
