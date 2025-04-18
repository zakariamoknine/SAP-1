#include <stdio.h>
#include <stdlib.h>

#ifndef ASM_FALSE
    #define ASM_FALSE 0
#endif

#ifndef ASM_TRUE
    #define ASM_TRUE 1
#endif

struct _assembler_state
{
    const char* filepath;
    const char* binarypath;
};

static struct _assembler_state _assembler = { 0 };

static int _assembler_parse_arguments(int argc, char** argv)
{
    if (argc == 1)
    {
        fprintf(stderr, "ERROR: No input, provide a single assembly file and the ouput file\n");
        fprintf(stderr, "\nExample:\n$ ./sap1-asm file_dir/file.asm output_dir/output.bin\n");
        return ASM_FALSE;
    }
    else if (argc == 2)
    {
        fprintf(stderr, "ERROR: A single file provided, provide a single assembly file and the ouput file\n");
        fprintf(stderr, "\nExample:\n$ ./sap1-asm file_dir/file.asm output_dir/output.bin\n");
        return ASM_FALSE;
    }
    else if (argc > 3)
    {
        fprintf(stderr, "ERROR: Too many arguments, provide a single asssembly file and the output file\n");
        fprintf(stderr, "\nExample:\n$ ./sap1-asm file_dir/file.asm output_dir/output.bin\n");
        return ASM_FALSE;
    }

    _assembler.filepath = argv[1];
    _assembler.binarypath = argv[2];
    
    if (!freopen(_assembler.filepath, "r", stdin))
    {
        fprintf(stderr, "ERROR: Can't open: %s\nNo such a file or directory\n", _assembler.filepath);
        return ASM_FALSE;
    }

    if (!freopen(_assembler.binarypath, "w", stdout))
    {
        fprintf(stderr, "ERROR: Can't open/create %s\nDirectory doesn't exist or permission denied\n", _assembler.binarypath);
        return ASM_FALSE;
    }

    printf("hello world");

    return ASM_TRUE;
}

int main(int argc, char** argv)
{
    if (!_assembler_parse_arguments(argc, argv))
    {
        return -1;
    }

    return 0;
}
