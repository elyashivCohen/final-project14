#include "data.h"

void freeMacros(Macro macros[], int count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        if (macros[i].lines != NULL)
        {
            free(macros[i].lines);
            macros[i].lines = NULL;
        }
    }
}

/* Function to ignore spaces in the begning of a line: */
void ignoreLeadingSpaces(char *line)
{

    char *start = line;

    /* Find the first non-space character */
    while (*line && isspace(*line))
    {
        line++;
    }

    /* Shift the remaining characters to the beginning of the string */
    while (*line)
    {
        *start = *line;
        start++;
        line++;
    }
    /* Null-terminate the new string */
    *start = '\0';
}

/* Function to find macro number according to macro's name (if exsits) */
int findMacro(Macro *m, int size, char *name)
{

    int ind = 0;
    while (ind < size)
    {

        if (strcmp(m[ind].name, name) == 0)
            return ind;
        ind++;
    }
    return -1;
}

/* Function to add lines to the 'lines' member of the Macro struct */
void addLinesToMacro(Macro *macro, const char *newLine)
{

    /* If the Macro struct has no lines yet, allocate memory for the new lines */
    if (macro->lines == NULL)
    {
        macro->lines = malloc(strlen(newLine)); /* +1 if needed */
        strcpy(macro->lines, newLine);
    }

    else
    {
        /* Allocate memory to concatenate new lines and existing lines */
        macro->lines = realloc(macro->lines, strlen(macro->lines) + strlen(newLine)); /*  + 2 */
        /* strcat(macro->lines, "\n"); Add a newline between existing and new lines */
        strcat(macro->lines, newLine); /* Append the new lines */
    }
}

/* The "pre_asm" function definition: */
int pre_asm(char *input_filename)
{
    char as_file_path[MAX_LINE_LENGTH], am_file_path[MAX_LINE_LENGTH];
    FILE *input_file, *output_file;
    Macro macros[MAX_MACRO_COUNT];
    int line_count = 1, macro_count = 0, macro_num;
    char line[MAX_LINE_LENGTH];
    char first_field[MAX_LINE_LENGTH];
    Bool mcr_flag = False;
    char macro_name[MAX_LINE_LENGTH];

    /* Check if input file ('.as') exsits: */
    strcpy(as_file_path, input_filename);
    strcat(as_file_path, ".as");
    input_file = fopen(as_file_path, "r");

    if (!input_file)
    {
        printf("Error: The input file '%s' does not have '.as' file\n", input_filename);
        return 1;
    }

    /* Try to create '.am' file */
    strcpy(am_file_path, input_filename);
    strcat(am_file_path, ".am");
    output_file = fopen(am_file_path, "w");
    if (!output_file)
    {
        printf("Error: Failed to open output file %s\n", am_file_path);
        return 1;
    }

    printf("Running PRE-asmebler for %s\n", input_filename);

    /* Read input file line by line: */
    while (fgets(line, MAX_LINE_LENGTH, input_file))
    {

        /* Get line without leading spaces & eplace all tabs with space*/
        ignoreLeadingSpaces(line);
        /* replaceTabs(line); */

        /* Ignore empty line (and '\n') */
        if (strlen(line) <= 1)
        {
            line_count++;
            continue;
        }

        /* Get first field */
        sscanf(line, "%s", first_field);

        /* Check if a macro has been used */
        macro_num = findMacro(macros, macro_count, first_field);
        if (macro_num != -1)
        {
            writeToFile(output_file, macros[macro_num].lines);
        }

        /* If a new mcro declared */
        else if (strcmp(first_field, START_OF_MACRO) == 0)
        {
            mcr_flag = True;
            /* Get macro name*/
            sscanf(line, "%*s %s", macro_name);

            /* Check if macro name doesn't already exsits*/
            if (findMacro(macros, macro_count, macro_name) != -1)
            {
                printf("Error in line %d: Declaring macro with the same name: %s\n", line_count, macro_name);
                return 1;
            }

            /* Check if the macro name is not an reserved word */
            else if ((whichOpcode(macro_name) != no_opcode) || strcmp(whichInstruction(macro_name), "no_instruction") != 0)
            {
                printf("Error in line: %d: Can't declare macro with preserved instruction name: %s\n", line_count, macro_name);
                return 1;
            }

            /* Add new macro */
            strcpy(macros[macro_count].name, macro_name);
        }

        /* If inside a macro: */
        else if (mcr_flag == True)
        {

            /* Check if end of mcr: */
            if (strcmp(first_field, END_OF_MACRO) == 0)
            {
                mcr_flag = False;
                macro_count++;
            }
            /* Add line to macro table */
            else
            {
                addLinesToMacro(&macros[macro_count], line);
            }
        }

        /* Write the line into .am file */
        else
        {
            writeToFile(output_file, line);
        }

        line_count++;
    }

    /* Free macros memorey */
    freeMacros(macros, macro_count);

    /* Save & close files */
    fclose(output_file);
    fclose(input_file);

    return 0;
}