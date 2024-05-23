#include "data.h"

/*check if comaa in right place*/
int valid_comma(char *str)
{
    int i = 0;
    while (*str != '\0')
    {
        if (i < 0)
        {
            return -1;
        }

        else if (isdigit(*str) || isalpha(*str))
        {
            if (i > 0)
            {
                str++;
                continue;
            }
            i++;
        }
        else if (*str == ',')
        {
            i--;
        }
        str++;
    }
    return 1;
}
void calculate_L(int *l, int dist, int src)
{
    if (src == -1 || dist == -1)
    {
        (*l) += 1 + (dist == 2);
    }
    else
    {
        /* Since each aderssing methos is 1 execpt the 2nd method and if
            both are the 3rd method: L=1  */
        (*l) += 2 + (dist == 2) + (src == 2) - (dist == 3 && src == 3);
    }
}

int validate_source_method(int line_number, opcode op, int addressing_method)
{

    switch (op)
    {
    case mov:
    case cmp:
    case add:
    case sub:
        if (!(addressing_method >= 0 && addressing_method <= 3))
        {
            printf("Error in line %d: Invalid source addressing method for opcode1!\n", line_number);
            return -1;
        }
        break;

    case lea:
        if (!(addressing_method == 1 || addressing_method == 2))
        {
            printf("Error in line %d: Invalid source addressing method for opcode2!\n", line_number);
            return -1;
        }
        break;

    case is_not:
    case clr:
    case inc:
    case dec:
    case jmp:
    case bne:
    case red:
    case prn:
    case jsr:
    case rts:
    case hlt:
        if (addressing_method != -1)
        {
            printf("Error in line %d: The opcode should not have a source adderssing method!\n", line_number);
            return -1;
        }
        break;

    case no_opcode:
        printf("Error: Invalid opcode.\n");
        return -1;
    }

    /* If the combination is valid, return 0 */
    return 0;
}

int validate_destination_method(int line_number, opcode op, int addressing_method)
{

    switch (op)
    {
    case mov:
    case add:
    case sub:
    case is_not:
    case clr:
    case lea:
    case inc:
    case dec:
    case red:
        if (!(addressing_method >= 1 && addressing_method <= 3))
        {
            printf("Error in line %d: Invalid destination addressing method for opcode!\n", line_number);
            return -1;
        }
        break;

    case cmp:
    case prn:
        if (!(addressing_method >= 0 && addressing_method <= 3))
        {
            printf("Error in line %d: Invalid destination addressing method for opcode!\n", line_number);
            return -1;
        }
        break;

    case jmp:
    case bne:
    case jsr:
        if (!(addressing_method == 1 || addressing_method == 3))
        {
            printf("Error in line %d: Invalid destination addressing method for opcode!\n", line_number);
            return -1;
        }
        break;

    case rts:
    case hlt:
        if (addressing_method != -1)
        {
            printf("Error in line %d: The opcode should not have a destination adderssing method!\n", line_number);
            return -1;
        }

    case no_opcode:
        printf("Error: Invalid opcode.\n");
        return -1;
    }

    /* If the combination is valid, return 0 */
    return 0;
}

int get_addressing_method(int line_number, char *operand)
{

    char *tmp;
    char str_before_bracket[MAX_LINE_LENGTH], str_inside_bracket[MAX_LINE_LENGTH];
    int i = 0;

    if (operand == NULL || strlen(operand) == 0)
    {
        printf("Error in line %d: Declared an empty operand!\n", line_number);
        return -1;
    }

    if (operand[0] == '#')
        return 0;

    if (whichRegister(operand) != no_register)
        return 3;

    tmp = strchr(operand, '[');
    if (tmp == NULL)
    {

        if (is_valid_string_name(line_number, operand) != 0)
            return -1;
        return 1;
    }

    else
    {
        /* Extract substring before '[' */
        strncpy(str_before_bracket, operand, tmp - operand);
        str_before_bracket[tmp - operand] = '\0';

        /* Extract substring between '[' and ']' */
        strcpy(str_inside_bracket, tmp + 1);
        tmp = strchr(str_inside_bracket, ']');
        if (tmp == NULL)
        {
            printf("Error in line %d: Missing closing bracket ']'!\n", line_number);
            return -1;
        }
        *tmp = '\0';

        /* Check if the substring before '[' is a valid string name */
        if (is_valid_string_name(line_number, str_before_bracket) != 0)
            return -1;

        /* Check if the substring inside '[' and ']' is a valid number or symbol */
        while (str_inside_bracket[i] != '\0')
        {
            if (!isdigit(str_inside_bracket[i]))
            {
                if (is_valid_string_name(line_number, str_inside_bracket) != 0)
                    return -1;
                break;
            }
            i++;
        }

        /* Check if there are no characters after ']' */
        if (*(tmp + 1) != '\0')
        {
            printf("Error in line %d: Unexpected characters after closing bracket ']'!\n", line_number);
            return -1;
        }

        return 2;
    }
}

int get_num_of_operands(int line_number, char *line)
{

    int count = 0;
    if (line == NULL || strlen(line) == 0)
        return 0;

    while (*line)
    {
        if (*line == ',')
        {
            count++;
        }
        line++;
    }
    if (count > 1)
    {
        printf("Error in line %d: Too many oprerand or too many commas in the instruction!\n", line_number);
        return -1;
    }
    return count + 1;
}

int get_opcode_operands(opcode op)
{

    switch (op)
    {
    case hlt:
    case rts:
        return 0;
    case is_not:
    case clr:
    case inc:
    case dec:
    case jmp:
    case bne:
    case red:
    case prn:
    case jsr:
        return 1;
    case lea:
    case sub:
    case add:
    case cmp:
    case mov:
        return 2;
    default:
        return -1; /* Error: Invalid opcode */
    }
}

void add_new_code(int **code, int *size, int value)
{

    /* Increment the size of the array */
    (*size)++;

    /* Allocate memory for the array */
    if (*code == NULL)
    {
        *code = malloc(sizeof(int));
    }
    else
    {
        *code = realloc(*code, (*size) * sizeof(int));
    }

    /* Add the new value at the end of the array */
    (*code)[(*size) - 1] = value;
}

int is_reserved(char str[MAX_NAME_LENGTH])
{

    /* Opcode or Register */
    if ((whichOpcode(str) != no_opcode) || (whichRegister(str) != no_register))
        return 0;

    /* Instruction */
    if (strcmp(whichInstruction(str), "no_instruction") != 0)
        return 0;

    return 1;
}

int is_valid_number(int line_number, char *value)
{
    int i = 0;

    /* Check if the number starts with + or - */
    if ((value[i] == '-') || (value[i] == '+'))
        i++;

    /* Check if it's an empty value */
    if (value[i] == '\0')
    {
        printf("Error in line %d: Declared an empty value\n", line_number);
        return 1;
    }

    /* Check about non-digit characters */
    while (value[i] != '\0')
    {
        if (!isdigit(value[i]))
        {
            printf("Error in line %d: The number value contains a non-digit character or invalid digit: %s\n", line_number, value);
            return 1;
        }
        i++;
    }

    return 0;
}

int is_valid_string_name(int line_number, char *str)
{
    int i = 0;

    /* Check if it's an empty value */
    if (str[i] == '\0')
    {
        printf("Error in line %d: Declared an empty symbol/label name\n", line_number);
        return 1;
    }

    /* Check if first character is from the alphabet */
    if (!isalpha(str[i]))
    {
        printf("Error in line %d: The symbol/label name starts with an invalid character: %s\n", line_number, str);
        return 1;
    }

    /* Check max limit of the name */
    if (strlen(str) > MAX_NAME_LENGTH)
    {
        printf("Error in line %d: Declared a symbol/label name with more then 31 characters: %s\n", line_number, str);
        return 1;
    }

    /* Check if it's not a reserved word */
    if (is_reserved(str) == 0)
    {
        printf("Error in line %d: Declared a symbol/label name with a reserved word: %s\n", line_number, str);
        return 1;
    }

    /* Check about invalid characters */
    while (str[i] != '\0')
    {
        if ((!isalpha(str[i])) && (!isdigit(str[i])))
        {
            printf("Error in line %d: The name contains a invalid character: %s\n", line_number, str);
            return 1;
        }
        i++;
    }

    return 0;
}

void increase_datas(SymbolTable *st, int IC)
{

    SymbolEntry *current;

    for (current = st->entries; current->next != NULL; current = current->next)
    {
        if (current->type == data)
            current->value += IC + IC_START_ADDRESS;
    }
}

int is_label(char *str)
{
    /* Check if str finishes with ':' */
    if (str[strlen(str) - 1] != ':')
        return 1;

    return 0;
}

void add_new_entry(SymbolTable *st, char *symbol_name, SymbolType type, int symbol_value)
{
    SymbolEntry *current;

    /* Create new symbol_enrty */
    SymbolEntry *entry = (SymbolEntry *)malloc(sizeof(SymbolEntry));

    strcpy(entry->key, symbol_name);
    entry->value = symbol_value;
    entry->type = type;
    entry->next = NULL;

    /* Add to Symbol Table */
    if (st->entries == NULL)
        st->entries = entry;

    else
    {
        for (current = st->entries; current->next != NULL; current = current->next)
            ;
        current->next = entry;
    }

    /* Increase Symbol Table size */
    st->count++;

    return;
}

int get_symbol_value(SymbolTable *st, char *symbol_name)
{
    /* Return symbol (named 'symbol_name') his value */
    SymbolEntry *current;

    for (current = st->entries; current->next != NULL; current = current->next)
    {
        if (strcmp(current->key, symbol_name) == 0)
            return current->value;
    }

    return -1;
}

int is_exsits(SymbolTable *st, char *key)
{
    int index = 0;
    SymbolEntry *current;

    for (current = st->entries; current != NULL; current = current->next)
    {
        if (strcmp(current->key, key) == 0)
            return index;
        index++;
    }

    return -1;
}

int check_symbol(int line_number, SymbolTable *st, char *symbol_name)
{
    /* Check if name is not a knwon symbol */
    if (is_reserved(symbol_name) == 0)
    {
        printf("Error in line %d: Declared a symbol name with a reserved word: %s\n", line_number, symbol_name);
        return 1;
    }

    /* Check if symbol already exsits */
    if (is_exsits(st, symbol_name) != -1)
    {
        printf("Error in line %d: Already declared '%s'\n", line_number, symbol_name);
        return 1;
    }

    return 0;
}

int first_pass(char *input_filename, SymbolTable *st, int **binary_code, int *insteraction_counter, int *data_counter)
{
    char input_file_path[MAX_LINE_LENGTH];
    FILE *input_file;
    char line[MAX_LINE_LENGTH], first_field[MAX_LINE_LENGTH];
    char *rest_of_line, *symbol_name, *str_value, *label_name, *strings, character, *operand;
    int line_count = 0, symbol_value, data_value, num_of_operands;
    int IC = 0, DC = 0, L, i, source_addressing_method_number, destination_addressing_method_number;
    opcode op_code;
    int *insteractions_code = NULL, *datas_code = NULL, code_value;
    Bool label_flag = False, error_flag = False;

    /* Check if input file ('.am') exsits: */
    strcpy(input_file_path, input_filename);
    strcat(input_file_path, ".am");
    input_file = fopen(input_file_path, "r");

    if (!input_file)
    {
        printf("Error: The '.am' file named '%s' does not exsit\n", input_filename);
        return 1;
    }

    printf("Running first-pass for %s\n", input_filename);

    /* Read input file line by line: */
    while (fgets(line, MAX_LINE_LENGTH, input_file))
    {

        line_count++;
        label_flag = False;

        /*if (strchr(line, '\n') == NULL)
        {
            printf("Error in line %d: Exceeding character limit (81 is the max)\n", line_count);
            error_flag = True;
            while ((c = fgetc(input_file)) != '\n' && c != EOF)
                ;
            continue;
        }*/

        /* Ignore comments */
        if (line[0] == ';')
            continue;

        /* Replace last tab to '\0' */
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        /* Get first filed */
        sscanf(line, "%s", first_field);

        /* "define" declartion */
        if (strcmp(first_field, DEFINE_DECLARTION) == 0)
        {

            rest_of_line = line + strlen(DEFINE_DECLARTION);

            symbol_name = strtok(rest_of_line, "=");
            clean_spaces(symbol_name);
            if (is_valid_string_name(line_count, symbol_name) != 0)
            {
                error_flag = True;
                continue;
            }

            str_value = strtok(NULL, "=");
            clean_spaces(str_value);
            if (is_valid_number(line_count, str_value))
            {
                error_flag = True;
                continue;
            }
            symbol_value = atoi(str_value);

            /* Check validation of the symbol */
            if (check_symbol(line_count, st, symbol_name) != 0)
            {
                error_flag = True;
                continue;
            }

            add_new_entry(st, symbol_name, mdefine, symbol_value);
            continue;
        }

        if (is_label(first_field) == 0)
        {

            /* Get the label name & validate */
            label_name = (char *)malloc(strlen(first_field));
            strcpy(label_name, first_field);
            label_name[strlen(first_field) - 1] = '\0';
            clean_spaces(label_name);

            if (is_valid_string_name(line_count, label_name) != 0)
            {
                error_flag = True;
                continue;
            }

            rest_of_line = line + strlen(first_field);
            clean_spaces(rest_of_line);
            if (strlen(rest_of_line) <= 1)
            {
                printf("Error in line %d:  %s has a missing instruction/action after label!\n", line_count, first_field);
                label_flag = True;
                continue;
            }

            strcpy(line, rest_of_line);
            sscanf(line, "%s", first_field);

            label_flag = True;
        }

        if (strcmp(first_field, STRING_DECLARTION) == 0)
        {
            if (label_flag == True)
            {

                /* Check validation of the symbol */
                if (check_symbol(line_count, st, label_name) != 0)
                {
                    error_flag = True;
                    continue;
                }

                /* Add label to symbol table*/
                add_new_entry(st, label_name, data, DC);
            }

            /* STRING */
            strings = strchr(line, '"');
            ;

            if (strings == NULL)
            {
                printf("Error in line %d: Declared a '.string' statement without '\"'\n", line_count);
                error_flag = True;
                continue;
            }

            strings++;
            while (*strings != '"')
            {
                /*if ((!isalpha(*strings)) && (!isdigit(*strings)) && (!isspace(*strings)))
                {
                    printf("Error in line %d: Invalid characters in string!\n", line_count);
                    error_flag = True;
                    break;
                }*/
                character = *strings;
                add_new_code(&datas_code, &DC, (int)character);
                strings++;
            }

            /* Add '0' by the end representing '/0' */
            add_new_code(&datas_code, &DC, 0);
            continue;
        }

        else if (strcmp(first_field, DATA_DECLARTION) == 0)
        {

            if (label_flag == True)
            {

                /* Check validation of the symbol */
                if (check_symbol(line_count, st, label_name) != 0)
                {
                    error_flag = True;
                    continue;
                }

                /* Add label to symbol table */
                add_new_entry(st, label_name, data, DC);
            }

            /* DATA */
            rest_of_line = line + strlen(DATA_DECLARTION);
            clean_spaces(rest_of_line);
            if (valid_comma(rest_of_line) == -1)
            {
                printf("Error in line %d: Unintended comma placements %s\n", line_count, rest_of_line);
                error_flag = True;
                continue;
            }
            str_value = strtok(rest_of_line, ",");

            if (str_value == NULL)
            {
                printf("Error in line %d: Declared a '.data' statement without any numbers\n", line_count);
                error_flag = True;
                continue;
            }

            while (str_value != NULL)
            {
                clean_spaces(str_value);
                if (is_exsits(st, str_value) != -1)
                {
                    data_value = get_symbol_value(st, str_value);
                }
                else if (is_valid_number(line_count, str_value))
                {
                    error_flag = True;
                    break;
                }
                else
                {
                    data_value = atoi(str_value);
                }

                add_new_code(&datas_code, &DC, data_value);
                str_value = strtok(NULL, ",");
            }
            continue;
        }

        else if (strcmp(first_field, EXTERN_DECLARTION) == 0)
        {

            /* EXTRNAL */
            /* Insert symbol as external */
            label_name = line + strlen(EXTERN_DECLARTION);
            clean_spaces(label_name);
            add_new_entry(st, label_name, external, 0);
            continue;
        }

        else if (strcmp(first_field, ENTRY_DECLARTION) == 0)
        {
            continue;
        }

        else
        {

            if (label_flag == True)
            {

                /* Check validation of the symbol */
                if (check_symbol(line_count, st, label_name) != 0)
                {
                    error_flag = True;
                    continue;
                }

                add_new_entry(st, label_name, code, IC + IC_START_ADDRESS);
            }

            if (whichOpcode(first_field) == no_opcode)
            {
                printf("Error in line %d: '%s' is not an known opcode or Label that not defined properly!\n", line_count, first_field);
                error_flag = True;
                continue;
            }

            code_value = 0;
            L = 0;
            rest_of_line = line + strlen(first_field);
            op_code = whichOpcode(first_field);
            clean_spaces(rest_of_line);
            num_of_operands = get_num_of_operands(line_count, rest_of_line);

            /* Too much commas */
            if (num_of_operands == -1)
                continue;

            /* Validate number of operands to opcode type */
            if (get_opcode_operands(op_code) != num_of_operands)
            {
                printf("Error in line %d: The number of operands are invalid for %s!\n", line_count, first_field);
                error_flag = True;
                continue;
            }

            if (num_of_operands == 0)
                add_new_code(&insteractions_code, &IC, op_code << 6);

            else if (num_of_operands == 1)
            {
                /* Find the distention adressing method and validate*/
                destination_addressing_method_number = get_addressing_method(line_count, rest_of_line);
                if (destination_addressing_method_number == -1)
                {
                    error_flag = True;
                    continue;
                }

                /* Validate that the destination method is valid to the opcode */
                if (validate_destination_method(line_count, op_code, destination_addressing_method_number) != 0)
                {
                    error_flag = True;
                    continue;
                }

                /* Add destination adderssing method & op code */
                code_value += destination_addressing_method_number << 2;
                code_value += op_code << 6;
                add_new_code(&insteractions_code, &IC, code_value);

                /* Calculate L & increase IC and insteractions code */
                calculate_L(&L, destination_addressing_method_number, -1);
                for (i = 0; i < L; i++)
                {
                    add_new_code(&insteractions_code, &IC, 0);
                }
            }

            else
            {
                /* num_of_operands == 2 */
                /* Find the source adressing method and validate */
                operand = strtok(rest_of_line, ",");
                clean_spaces(operand);
                source_addressing_method_number = get_addressing_method(line_count, rest_of_line);
                if (source_addressing_method_number == -1)
                {
                    error_flag = True;
                    continue;
                }

                /* Validate that the source method is valid to the opcode */
                if (validate_source_method(line_count, op_code, source_addressing_method_number) != 0)
                {
                    error_flag = True;
                    continue;
                }

                /* Add source adderssing method */
                code_value += source_addressing_method_number << 4;

                /* Find the distention adressing method and validate*/
                operand = strtok(NULL, ",");
                /*if (operand == NULL)
                {
                    printf("Error: In line %d - Missing operand - command 'mov/add/sub' should have 2 operands.\n", line_count);
                    error_flag = True;
                    continue;
                }*/
                clean_spaces(operand);
                destination_addressing_method_number = get_addressing_method(line_count, operand);
                if (destination_addressing_method_number == -1)
                {
                    error_flag = True;
                    continue;
                }

                /* Validate that the destination method is valid to the opcode */
                if (validate_destination_method(line_count, op_code, destination_addressing_method_number) != 0)
                {
                    error_flag = True;
                    continue;
                }

                /* Add destination adderssing method & op code */
                code_value += destination_addressing_method_number << 2;
                code_value += op_code << 6;
                add_new_code(&insteractions_code, &IC, code_value);

                /* Calculate L & increase IC and insteractions code */
                calculate_L(&L, destination_addressing_method_number, source_addressing_method_number);
                for (i = 0; i < L; i++)
                {
                    add_new_code(&insteractions_code, &IC, 0);
                }
            }
        }
    }

    /* If a an error found stop the running */
    if (error_flag == True)
        return 1;

    /* Increase each '.data' symbol in the symbol table by IC + IC_START_ADDRESS(=100) */
    increase_datas(st, IC);

    /* Close the input file */
    fclose(input_file);

    /* Create binary_code & set IC and DC */
    *binary_code = (int *)malloc((DC + IC) * sizeof(int));

    for (i = 0; i < IC; i++)
        (*binary_code)[i] = insteractions_code[i];

    for (i = 0; i < DC; i++)
    {
        (*binary_code)[IC + i] = datas_code[i];
    }
    free(insteractions_code);
    free(datas_code);

    *insteraction_counter = IC;
    *data_counter = DC;

    return 0;
}