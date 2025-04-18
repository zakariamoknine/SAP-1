#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SAP1_FALSE
    #define SAP1_FALSE 0
#endif

#ifndef SAP1_TRUE
    #define SAP1_TRUE 1
#endif

#define SAP1_EXIT_FAILURE 1
#define SAP1_EXIT_SUCCESS 0

#define SAP1_MAX_CHARS_PER_LINE 32
#define SAP1_MAX_CHARS_PER_VARIABLE 21

struct _sap1_assembler_state
{
    const char* filepath;
    const char* binarypath;

    char** variable_name[16][SAP1_MAX_CHARS_PER_VARIABLE];
    uint8_t variable_value[16];
};

static struct _sap1_assembler_state _assembler = { 0 };


static int _assembler_init_data_table()
{
    char linestr[SAP1_MAX_CHARS_PER_LINE];
    int line = 0;
    while (fgets(linestr, sizeof(linestr) ,stdin))
    {
        line++;
        char* charptr = linestr;
        while (*charptr == ' ' || *charptr == '\t') { charptr++; }
        if (*charptr == '\0' || *charptr == '\t' || *charptr == '\n') { continue; }
        if (!strcmp(charptr, ".data\n"))
        {

        }
    }

    char* parsed_lines;

    return SAP1_TRUE;
}

static int _assembler_parse_arguments(int argc, char** argv)
{
    if (argc == 1)
    {
        fprintf(stderr, "ERROR: No input, provide a single assembly file and the ouput file\n");
        fprintf(stderr, "\nExample:\n$ ./sap1-asm file_dir/file.asm output_dir/output.bin\n");
        return SAP1_FALSE;
    }
    else if (argc == 2)
    {
        fprintf(stderr, "ERROR: A single file provided, provide a single assembly file and the ouput file\n");
        fprintf(stderr, "\nExample:\n$ ./sap1-asm file_dir/file.asm output_dir/output.bin\n");
        return SAP1_FALSE;
    }
    else if (argc > 3)
    {
        fprintf(stderr, "ERROR: Too many arguments, provide a single asssembly file and the output file\n");
        fprintf(stderr, "\nExample:\n$ ./sap1-asm file_dir/file.asm output_dir/output.bin\n");
        return SAP1_FALSE;
    }

    _assembler.filepath = argv[1];
    _assembler.binarypath = argv[2];
    
    if (!freopen(_assembler.filepath, "r", stdin))
    {
        fprintf(stderr, "ERROR: Can't open: %s\nNo such a file or directory\n", _assembler.filepath);
        return SAP1_FALSE;
    }

    if (!freopen(_assembler.binarypath, "w", stdout))
    {
        fprintf(stderr, "ERROR: Can't open/create %s\nDirectory doesn't exist or permission denied\n", _assembler.binarypath);
        return SAP1_FALSE;
    }

    return SAP1_TRUE;
}

int main(int argc, char** argv)
{
    if (!_assembler_parse_arguments(argc, argv)) { return SAP1_EXIT_FAILURE; }
    if (!_assembler_init_data_table()) { return SAP1_EXIT_FAILURE; }

    return SAP1_EXIT_SUCCESS;
}
