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

#define SAP1_MAX_LINES_PER_FILE 128

#define SAP1_MAX_CHARS_PER_LINE 120
#define SAP1_MAX_CHARS_PER_VARIABLE 32
#define SAP1_MAX_VARIABLES_PER_FILE 16

#define SAP1_MAX_TOKENS_PER_LINE 3
#define SAP1_MAX_CHARS_PER_TOKEN SAP1_MAX_CHARS_PER_VARIABLE

#define SAP1_MAX_INSTRUCTIONS_PER_FILE 16

#define SAP1_ENTRY_POINT_STR "_start:"
#define SAP1_DATE_SECTION_STR ".data"
#define SAP1_TEXT_SECTION_STR ".text"


static const char _sap1_opcodes[5][4] = { "LDA", "ADD", "SUB", "OUT", "HLT" };

struct _sap1_assembler_state
{
    const char* filepath;
    const char* binarypath;

    char tokens[SAP1_MAX_LINES_PER_FILE][SAP1_MAX_TOKENS_PER_LINE][SAP1_MAX_CHARS_PER_VARIABLE];
    int lexlines;

    int variables_count;
    struct
    {
        char name[SAP1_MAX_CHARS_PER_VARIABLE];
        uint8_t address;
        uint8_t value;
    } variables[SAP1_MAX_VARIABLES_PER_FILE];

    int instructions_count;
    struct
    {
        char opcode[4];
        uint8_t operand;
    } instructions[SAP1_MAX_INSTRUCTIONS_PER_FILE];
};

// assembler global state
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

void _assembler_uint8_to_binary_string(uint8_t value, char* binary_str) 
{
    for (int i = 7; i >= 0; i--) {
        binary_str[7 - i] = (value & (1 << i)) ? '1' : '0';
    }
    binary_str[8] = '\0';
}

static void _assembler_gen_binary()
{
    char outbin[16][9] = { 0 };

    for (int i = 0; i < 16; i++) {
        memset(outbin[i], '0', 8);
        outbin[i][8] = '\0';
    }

    char valbinstr[9];
    for (int i = 0; i < _assembler.variables_count; i++) {
        _assembler_uint8_to_binary_string(_assembler.variables[i].value, valbinstr);
        strcpy(outbin[_assembler.variables[i].address], valbinstr);
    }

    char instrbinstr[9] = { 0 };
    char finalinstrbuff[9];
    for (int i = 0; i< _assembler.instructions_count; i++) {
        uint8_t operand = _assembler.instructions[i].operand;

        for (int j = 7; j >= 0; j--) {
            instrbinstr[7 - j] = (operand & (1 << j)) ? '1' : '0';
        }

        if (!strcmp(_assembler.instructions[i].opcode, _sap1_opcodes[0])) {
            snprintf(finalinstrbuff, 9, "%s%s", "0000", instrbinstr + 4);
        }
        else if (!strcmp(_assembler.instructions[i].opcode, _sap1_opcodes[1])) {
            snprintf(finalinstrbuff, 9, "%s%s", "0001", instrbinstr + 4);
        }
        else if (!strcmp(_assembler.instructions[i].opcode, _sap1_opcodes[2])) {
            snprintf(finalinstrbuff, 9, "%s%s", "0010", instrbinstr + 4);
        }
        else if (!strcmp(_assembler.instructions[i].opcode, _sap1_opcodes[3])) {
            snprintf(finalinstrbuff, 9, "%s%s", "1110", instrbinstr + 4);
        }
        else if (!strcmp(_assembler.instructions[i].opcode, _sap1_opcodes[4])) {
            snprintf(finalinstrbuff, 9, "%s%s", "1111", instrbinstr + 4);
        }

        finalinstrbuff[8] = '\0';

        strcpy(outbin[i], finalinstrbuff);
    }

    for(int i = 0; i < 16; i++) {
        fprintf(stdout, "%s\n", outbin[i]);
    }
}

static int _assembler_parse()
{
    int datafound = SAP1_FALSE;

    int varcount = 0;
    int varaddress = 15;

    for (int i = 0; i < _assembler.lexlines; i++) {
        if (!strcmp(_assembler.tokens[i][0], SAP1_DATE_SECTION_STR)) { 
            datafound = SAP1_TRUE;
            continue;
        }

        if (!strcmp(_assembler.tokens[i][0], SAP1_TEXT_SECTION_STR)) { break; }

        if (datafound) {
            char* var_name = _assembler.tokens[i][0];
            char* var_equal_sign = _assembler.tokens[i][1];
            char* var_value_str = _assembler.tokens[i][2];

            if (!var_name || !var_equal_sign || !var_value_str || strcmp(var_equal_sign, "=")) {
                fprintf(stderr, "ERROR: .data section: Invalid syntax\n");
                return SAP1_FALSE;
            }

            if (isdigit(var_name[0])) {
                fprintf(stderr, "ERROR: .data section: Variable name cannot start with a digit/number\n");
                return SAP1_FALSE;
            }

            uint8_t var_value;
            if (!_assembler_check_uint8(var_value_str, &var_value)) {
                fprintf(stderr, "ERROR: .data section: Invalid value used for variable assignment\n");
                return SAP1_FALSE;
            }

            for (int j = 0; j < varcount; j++) {
                if (!strcmp(var_name, _assembler.variables[j].name)) {
                    fprintf(stderr, "ERROR: .data section: Duplicate variable name: '%s'\n", var_name);
                    return SAP1_FALSE;
                }
            }

            strncpy(_assembler.variables[varcount].name, var_name, SAP1_MAX_CHARS_PER_VARIABLE - 1);
            _assembler.variables[varcount].name[SAP1_MAX_CHARS_PER_VARIABLE - 1] = '\0';
            _assembler.variables[varcount].value = var_value;
            _assembler.variables[varcount].address = varaddress;

            varcount++;
            varaddress--;

            if (varcount > SAP1_MAX_VARIABLES_PER_FILE) {
                fprintf(stderr, "ERROR: .data section: You can't assign more than %d variables\n", SAP1_MAX_VARIABLES_PER_FILE);
                return SAP1_FALSE;
            }
        }
    }

    _assembler.variables_count = varcount;

    int entryfound = SAP1_FALSE;

    for (int i = 0; i < _assembler.lexlines; i++) {
        if (!strcmp(_assembler.tokens[i][0], SAP1_ENTRY_POINT_STR)) { 
            entryfound = SAP1_TRUE;
            continue;
        }

        if (entryfound) {
            char* opcode = _assembler.tokens[i][0];
            char* operand = _assembler.tokens[i][1];

            if (_assembler.tokens[i][2][0] != '\0') {
                fprintf(stderr, "ERROR: .text section: Invalid syntax, too many operands per instruction\n");
                return SAP1_FALSE;
            }

            if (strlen(opcode) != 3) {
                fprintf(stderr, "ERROR: .text section: Invalid instruction operation code\n");
                return SAP1_FALSE;
            }

            int validopcode = SAP1_FALSE;
            for (int j = 0; j < 5; j++) {
                if (!strcmp(opcode, _sap1_opcodes[j])) {
                    validopcode = SAP1_TRUE;
                }
            }

            if (!validopcode) {
                fprintf(stderr, "ERROR: .text section: Invalid instruction operation code\n");
                return SAP1_FALSE;
            }

            if (!strcmp(opcode, "OUT") || !strcmp(opcode, "HLT")) {
                if (operand[0] != '\0') {
                    fprintf(stderr, "ERROR: .text section: OUT and HLT instructions require no operand\n");
                    return SAP1_FALSE;
                }

                strcpy(_assembler.instructions[_assembler.instructions_count].opcode, opcode);
            }
            else {
                int validvar = SAP1_FALSE;
                for(int j = 0; j < _assembler.variables_count; j++) {
                    if (!strcmp(operand, _assembler.variables[j].name)) {
                        strcpy(_assembler.instructions[_assembler.instructions_count].opcode, opcode);
                        _assembler.instructions[_assembler.instructions_count].operand = _assembler.variables[j].address;
                        validvar = SAP1_TRUE;
                    }
                }

                if (!validvar) {
                    fprintf(stderr, "ERROR: .text section: Unknown variable used as operand\n");
                    return SAP1_FALSE;
                }
            }

            _assembler.instructions_count++;

            if (_assembler.instructions_count > SAP1_MAX_INSTRUCTIONS_PER_FILE) {
                fprintf(stderr, "ERROR: .text section: Too many instructions, program doesn't fit on SAP-1's 16 bytes of memory\n");
                return SAP1_FALSE;
            }
        }
    }

    if (_assembler.variables_count + _assembler.instructions_count > 16) {
        fprintf(stderr, "ERROR: Too many variables/instructions, program doesn't fit on SAP-1's 16 bytes of memory\n");
        return SAP1_FALSE;
    }

    return SAP1_TRUE;
}

static int _assembler_lex()
{
    char linestr[SAP1_MAX_CHARS_PER_LINE];
    int line = 1;

    char* token;
    while (fgets(linestr, sizeof(linestr) ,stdin)) {
        // trauncate comments
        for (int i = 0; i < strlen(linestr); i++) {
            if (linestr[i] == ';') {
                linestr[i] = '\0';
                break;
            }
        }

        // skip lines that contain only spaces, tabs or new lines
        int j = 0;
        while (linestr[j] == ' ' || linestr[j] == '\t') { j++; }
        if (linestr[j] == '\n' || linestr[j] == '\0') continue;

        // add extra spaces around '=' sign for easier tokenization
        for (int i = 0; linestr[i]; i++) {
            if (linestr[i] == '=') {
                memmove(&linestr[i + 2], &linestr[i + 1], strlen(&linestr[i + 1]) + 1);
                linestr[i + 1] = ' ';
                memmove(&linestr[i + 1], &linestr[i], strlen(&linestr[i]) + 1);
                linestr[i] = ' ';
                i += 2;
            }
        }

        // tokenize and store tokens
        int tokeni = 0;
        token = strtok(linestr, " \t\n");
        while (token != NULL) {
            if (strlen(token) > SAP1_MAX_CHARS_PER_TOKEN) {
                fprintf(stderr, "ERROR: Invalid syntax, you can't name varibales very large names\n");
                return SAP1_FALSE;
            }

            strncpy(_assembler.tokens[line][tokeni], token, SAP1_MAX_CHARS_PER_TOKEN - 1);
            _assembler.tokens[line][tokeni][SAP1_MAX_CHARS_PER_TOKEN - 1] = '\0';

            token = strtok(NULL, " \t\n");
            tokeni++;
        }

        if (tokeni > SAP1_MAX_TOKENS_PER_LINE) {
            fprintf(stderr, "ERROR: Invalid syntax\n");
            return SAP1_FALSE;
        }

        line++;
    }

    _assembler.lexlines = line;

    // check for entry point _start, .data and .text sections in the right order
    int startfound = SAP1_FALSE;
    int datafound = SAP1_FALSE;
    int textfound = SAP1_FALSE;
    
    int expectingentry = SAP1_FALSE;

    for (int i = 0; i < _assembler.lexlines; i++) {
        if (_assembler.tokens[i][0][0] == '\0') { continue; }

        if (expectingentry) {
            if (!strcmp(_assembler.tokens[i][0], SAP1_ENTRY_POINT_STR)) {
                startfound = SAP1_TRUE;
                expectingentry = SAP1_FALSE;
            }
            else {
                fprintf(stderr, "ERROR: An entry point labeled %s must immediately follow the .text section\n", SAP1_ENTRY_POINT_STR);
                return SAP1_FALSE;
            }
        }

        if (!strcmp(_assembler.tokens[i][0], SAP1_DATE_SECTION_STR)) {
            if (textfound) {
                fprintf(stderr, "ERROR: .data section must come before the .text section\n");
                return SAP1_FALSE;
            }

            datafound = SAP1_TRUE;
        }

        if (!strcmp(_assembler.tokens[i][0], SAP1_TEXT_SECTION_STR)) {
            textfound = SAP1_TRUE;
            expectingentry = SAP1_TRUE;
        }
    }

    if (!startfound) {
        fprintf(stderr, "ERROR: Entry point not found, add an entry point labeled %s immediately following .text section\n", SAP1_ENTRY_POINT_STR);
        return SAP1_FALSE;
    }

    if (!datafound) {
        fprintf(stderr, "ERROR: .data section not found, it must exist even if empty\n");
        return SAP1_FALSE;
    }

    if (!textfound) {
        fprintf(stderr, "ERROR: .text section not found\n");
        return SAP1_FALSE;
    }

    return SAP1_TRUE;
}

static int _assembler_parse_arguments(int argc, char** argv)
{
    if (argc == 1) {
        fprintf(stderr, "ERROR: No input, provide a single assembly file and the ouput file\n");
        fprintf(stderr, "\nExample:\n$ ./sap1-asm file_dir/file.asm output_dir/output.bin\n");
        return SAP1_FALSE;
    }
    else if (argc == 2) {
        fprintf(stderr, "ERROR: A single file provided, provide a single assembly file AND the ouput file\n");
        fprintf(stderr, "\nExample:\n$ ./sap1-asm file_dir/file.asm output_dir/output.bin\n");
        return SAP1_FALSE;
    }
    else if (argc > 3) {
        fprintf(stderr, "ERROR: Too many arguments, provide a single asssembly file and the output file\n");
        fprintf(stderr, "\nExample:\n$ ./sap1-asm file_dir/file.asm output_dir/output.bin\n");
        return SAP1_FALSE;
    }

    _assembler.filepath = argv[1];
    _assembler.binarypath = argv[2];
    
    if (!freopen(_assembler.filepath, "r", stdin)) {
        fprintf(stderr, "ERROR: Can't open: %s\nNo such a file or directory\n", _assembler.filepath);
        return SAP1_FALSE;
    }

    if (!freopen(_assembler.binarypath, "w", stdout)) {
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

    _assembler_gen_binary();

    return SAP1_EXIT_SUCCESS;
}
