#include "data.h"

int main(int argc, char *argv[])
{

    char *input_filename;
    int i;
    /* Create a symbol table which will create in 'first_pass' and used in 'second_pass' */
    SymbolTable symbol_table;
    /* Create an int array which will hold the binary code */
    int *binary_code;

    /* Validate input files */
    if (argc < 2)
    {
        printf("Error: Missing input files for running the assembler: ./assembler <filename0> <filename1> ...\n");
        return 1;
    }

    /* For each input file activate the assmbler */
    for (i = 1; i < argc; i++)
    {

        /* First part of the project: pre assembler which will clean macros & create '.am' file  */
        input_filename = argv[i];
        if (pre_asm(input_filename) != 0)
            continue;

        /* Initialize the symbol table & binary code array for each file */
        symbol_table.count = 0;
        symbol_table.entries = NULL;
        binary_code = NULL;

        /* Second part of the project: create the symbol table & the initial file of the binary code */
        if (first_pass(input_filename, &symbol_table, &binary_code) != 0)
            continue;

        /* Third part of the project: fill the missing parts of the binary code & create the output
            files (<filename>.ob, <filename>.ent, <filename>.ext ) and encode*/
        if (second_pass(input_filename, &symbol_table, &binary_code) != 0)
            continue;

        if (encryption(&binary_code, input_filename, 15, 20) != 0)
            continue;

        printf("Ran the assembler successfully for %s! \n", input_filename);

        /* Free the memory allocated for the symbol table & binary_code */
        freeSymbolTable(&symbol_table);
        /*free(binary_code);*/
    }

    return 0;
}