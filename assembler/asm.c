#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>

#ifndef SAP1_FALSE
    #define SAP1_FALSE 0
#endif

#ifndef SAP1_TRUE
    #define SAP1_TRUE 1
#endif

#define SAP1_EXIT_FAILURE 1
#define SAP1_EXIT_SUCCESS 0

#define SAP1_MAX_LINES_PER_FILE 68

#define SAP1_MAX_CHARS_PER_LINE 82
#define SAP1_MAX_CHARS_PER_VARIABLE 32
#define SAP1_MAX_VARIABLES_PER_FILE 16

#define SAP1_MAX_TOKENS_PER_LINE 3
#define SAP1_MAX_CHARS_PER_TOKEN SAP1_MAX_CHARS_PER_VARIABLE

#define SAP1_ENTRY_POINT_STR "_start:"
#define SAP1_DATE_SECTION_STR ".data"
#define SAP1_TEXT_SECTION_STR ".text"


struct _sap1_assembler_state
{
    const char* filepath;
    const char* binarypath;

    char tokens[SAP1_MAX_LINES_PER_FILE][SAP1_MAX_TOKENS_PER_LINE][SAP1_MAX_CHARS_PER_VARIABLE];
    int lexlines;

    int variable_count;
    struct
    {
        char name[SAP1_MAX_VARIABLES_PER_FILE][SAP1_MAX_CHARS_PER_VARIABLE];
        uint8_t address;
        uint8_t value;
    } variable[SAP1_MAX_VARIABLES_PER_FILE];
};

static struct _sap1_assembler_state _assembler = { 0 };

static int _assembler_check_uint8(const char* str, uint8_t* out)
{
    char* endptr;
    long val = strtol(str, &endptr, 10);

    if (str == endptr) return SAP1_FALSE;

    if (*endptr != '\0') return SAP1_FALSE;

    if (val < 0 || val > 255) return SAP1_FALSE;

    *out = (uint8_t)val;

    return SAP1_TRUE;
}

static int _assembler_parse()
{
    int datafound = SAP1_FALSE;

    int i;
    for (i = 0; i < _assembler.lexlines; i++)
    {
        if (!strcmp(_assembler.tokens[i][0], ".data")) 
        { 
            datafound = SAP1_TRUE;
            continue;
        }

        if (!strcmp(_assembler.tokens[i][0], ".text")) { break; }

        if (datafound)
        {
            char* var_name = _assembler.tokens[i][0];
            char* var_equal_sign = _assembler.tokens[i][1];
            char* var_value_str = _assembler.tokens[i][2];

            if (!var_name || !var_equal_sign || !var_value_str || strcmp(var_equal_sign, "="))
            {
                fprintf(stderr, "ERROR: .data section, Invalid syntax\n");
                return SAP1_FALSE;
            }

            if (isdigit(var_name[0]))
            {
                fprintf(stderr, "ERROR: Variable name cannot start with a digit/number\n");
                return SAP1_FALSE;
            }

            uint8_t var_value;
            if (!_assembler_check_uint8(var_value_str, &var_value))
            {
                fprintf(stderr, "ERROR: .data section: Invalid value used for variable assignement\n");
                return SAP1_FALSE;
            }
        }
    }

    return SAP1_TRUE;
}

static int _assembler_lex()
{
    char linestr[SAP1_MAX_CHARS_PER_LINE];
    int line = 1;

    char* token;
    while (fgets(linestr, sizeof(linestr) ,stdin))
    {
        // trauncate comments
        int i = 0;
        for (i = 0; i < strlen(linestr); i++)
        {
            if (linestr[i] == ';')
            {
                linestr[i] = '\0';
                break;
            }
        }

        // skip lines that contain only spaces, tabs or new lines
        int j = 0;
        while (linestr[j] == ' ' || linestr[j] == '\t') { j++; }
        if (linestr[j] == '\n' || linestr[j] == '\0') continue;

        // add extra spaces around '=' sign for easier tokenization
        int k = 0;
        for (k = 0; linestr[k]; k++)
        {
            if (linestr[k] == '=') 
            {
                memmove(&linestr[k + 2], &linestr[k + 1], strlen(&linestr[k + 1]) + 1);
                linestr[k + 1] = ' ';

                memmove(&linestr[k + 1], &linestr[k], strlen(&linestr[k]) + 1);
                linestr[k] = ' ';

                k += 2;
            }
        }

        // tokenize and store tokens
        int tokeni = 0;
        char* token = strtok(linestr, " \t\n");
        while (token != NULL && tokeni < SAP1_MAX_TOKENS_PER_LINE)
        {
            strncpy(_assembler.tokens[line][tokeni], token, SAP1_MAX_CHARS_PER_TOKEN - 1);
            _assembler.tokens[line][tokeni][SAP1_MAX_CHARS_PER_TOKEN - 1] = '\0';

            token = strtok(NULL, " \t\n");
            tokeni++;
        }

        line++;
    }

    _assembler.lexlines = line;

    // check for entry point _start, .data and .text sections in the right order
    int startfound = SAP1_FALSE;
    int datafound = SAP1_FALSE;
    int textfound = SAP1_FALSE;

    int i;
    for (i = 0; i < _assembler.lexlines; i++)
    {
        if (_assembler.tokens[i][0][0] == '\0') { continue; }

        if (!strcmp(_assembler.tokens[i][0], SAP1_ENTRY_POINT_STR))
        {
            startfound = SAP1_TRUE;
        }

        if (!strcmp(_assembler.tokens[i][0], SAP1_DATE_SECTION_STR))
        {
            if (textfound)
            {
                fprintf(stderr, "ERROR: .data section must come before the .text section\n");
                return SAP1_FALSE;
            }

            datafound = SAP1_TRUE;
        }

        if (!strcmp(_assembler.tokens[i][0], SAP1_TEXT_SECTION_STR))
        {
            textfound = SAP1_TRUE;
        }
    }

    if (!startfound)
    {
        fprintf(stderr, "ERROR: Entry point not found, add an entry point labeled _start\n");
        return SAP1_FALSE;
    }

    if (!datafound)
    {
        fprintf(stderr, "ERROR: .data section not found, it must exist even if empty\n");
        return SAP1_FALSE;
    }

    if (!textfound)
    {
        fprintf(stderr, "ERROR: .text section not found\n");
        return SAP1_FALSE;
    }

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
        fprintf(stderr, "ERROR: A single file provided, provide a single assembly file AND the ouput file\n");
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

    if (!_assembler_lex()) { return SAP1_EXIT_FAILURE; }

    if (!_assembler_parse()) { return SAP1_EXIT_FAILURE; }

    return SAP1_EXIT_SUCCESS;
}
