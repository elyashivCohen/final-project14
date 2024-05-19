#include "data.h"

extern int is_valid_number(int line_number, char *value);
extern char *regArr[AMOUNT_OF_REGISTER];
static int IC, L = 0, line_number = 0;
FILE *ext_file;
char *fileName;

/*SymbolEntry *is_a_label(char *first_field, SymbolTable *Table)
{
    SymbolEntry *tmp;

    tmp = (SymbolEntry *)malloc(sizeof(SymbolEntry));

    if (Table == NULL || first_field == NULL)
        return NULL;
    for (tmp = Table->entries; tmp->next != NULL; tmp = tmp->next)
    {
        if (!strcmp(tmp->key, first_field) && (tmp->type == 1 || tmp->type == 3))
        {
            return tmp;
        }
    }
    return NULL;
}*/
int mark_as_entry(char *cur_feild, SymbolTable *Table)
{
    SymbolEntry *tmp;

    cur_feild = strtok(NULL, "\n\t\r ");
    if ((tmp = get_entry(cur_feild, Table)))
    {
        tmp->type = entry;
    }
    tmp = NULL;
    return 1;
}

int print_to_entry_file(SymbolTable *Table)
{
    SymbolEntry *entry;
    FILE *ent_file;
    char *curFileName;

    curFileName = (char *)malloc(strlen(fileName) + 4);
    sprintf(curFileName, "%s%s", fileName, ".ent");

    for (entry = Table->entries; entry->next != NULL; entry = entry->next)
    {
        if (entry->type == 4)
        {
            if (ent_file == NULL && !(ent_file = fopen(curFileName, "w")))
            {
                perror("cant open this file");
                return 0;
            }
            if (entry->value < 1000)
                fprintf(ent_file, "%s\t0%d\n", entry->key, entry->value);
            else
                fprintf(ent_file, "%s\t%d\n", entry->key, entry->value);
        }
    }
    entry = NULL;
    fclose(ent_file);
    free(curFileName);
    return 1;
}
int print_to_ext_file(int line, char *key)
{
    char *curFileName;
    if (!fileName)
    {
        printf("file name argument didnt exsis\n");
    }

    curFileName = (char *)malloc(strlen(fileName) + 4);
    sprintf(curFileName, "%s%s", fileName, ".ext");
    if (ext_file == NULL)
    {
        ext_file = fopen(curFileName, "w");
        if (ext_file == NULL)
        {
            perror("cant open this file");
            return 0;
        }
    }
    line += 100;
    if (line < 1000)
        fprintf(ext_file, "%s\t0%d\n", key, line);
    else
        fprintf(ext_file, "%s\t%d\n", key, line);
    return 1;
}
Bool is_extern(SymbolEntry *entry, SymbolTable *Table)
{
    if (entry->type == 3)
    {
        return True;
    }
    return False;
}

void args_classify(char *args, SymbolTable *Table, int **arr, opcodeType type)
{
    int i;
    char *firstPart, *secondPart;
    SymbolEntry *entry;
    /*static FILE *entPTR, *extPTR;*/

    entry = (SymbolEntry *)malloc(sizeof(SymbolEntry));
    if ((*args) == '#')
    {
        args++;
        if (!is_valid_number(get_line_number(), args))
        {
            (**arr) = atoi(args);
            (**arr) <<= 2;
        }
        entry = get_entry(args, Table);
        if (entry)
        {
            (**arr) = entry->value;
            (**arr) <<= 2;
            if (is_extern(entry, Table))
            {
                print_to_ext_file(L + IC, entry->key);
            }
        }
    }
    else if ((i = is_register(args)))
    {
        (**arr) = (i - 1);
    }
    else if (is_index_add(args, &firstPart, &secondPart))
    {
        (**arr) = get_entry_val(firstPart, Table);
        (**arr) <<= 2;
        (**arr) += 2;

        (*arr)++;
        if (atoi(secondPart))
            (**arr) = atoi(secondPart);
        else
            (**arr) = get_entry_val(secondPart, Table);
        (**arr) <<= 2;
        L = L + 2;
        return;
    }
    entry = get_entry(args, Table);
    if (entry)
    {
        (**arr) = entry->value;
        (**arr) <<= 2;
        if (entry->type == 3)
        {
            (**arr) = (**arr) + 1;
            print_to_ext_file(L + IC, entry->key);
        }
        else if (entry->type == 4 || entry->type == 2)
        {
            (**arr) = (**arr) + 2;
        }
    }
    L++;
}

int is_index_add(char *arg, char **firstPart, char **secondPart)
{
    char *tmp;
    int i, start, end;

    i = 0, start = 0, end = 0;
    tmp = arg;
    while (*arg != '\0')
    {

        if (*arg == '[')
        {
            start = i;
        }
        else if (*arg == ']')
        {
            end = i;
        }
        arg++;
        i++;
    }
    arg = tmp;
    if (start == end || start == 0 || end == 0)
    {
        return 0;
    }
    else
    {
        *firstPart = (char *)malloc(sizeof(int) * MAX_LINE_LENGTH);
        *secondPart = (char *)malloc(sizeof(int) * MAX_LINE_LENGTH);

        strncpy(*firstPart, arg, start); /* Copy first part*/
        (*firstPart)[start] = '\0';      /* Null-terminate first part*/

        strncpy(*secondPart, arg + start + 1, end - start - 1); /*  Copy second part */
        (*secondPart)[end - start - 1] = '\0';                  /* Null-terminate second part */
    }
    return 1;
}
int is_register(char *reg)
{
    int i;
    for (i = 0; i < AMOUNT_OF_REGISTER; i++)
    {
        if (!strcmp(regArr[i], reg))
        {
            return (i + 1);
        }
    }
    return 0;
}

int prase_args(SymbolTable *Table, char *token, int **arr, opcodeType type)
{
    char *secondArgs, *firstArgs;
    int tmp;
    token = strtok(NULL, "\n\t\r ,");
    firstArgs = (char *)malloc(strlen(token) + 1);
    strcpy(firstArgs, token);

    (*arr)++;
    if (type == 1)
    {
        args_classify(firstArgs, Table, arr, type);
    }
    else if (type == 2)
    {
        token = strtok(NULL, "\n\t\r ");
        secondArgs = (char *)malloc(strlen(token) + 1);
        strcpy(secondArgs, token);

        if (is_register(firstArgs) && is_register(secondArgs))
        {
            args_classify(firstArgs, Table, arr, type);
            (**arr) = (**arr) << 5;
            tmp = (**arr);
            args_classify(secondArgs, Table, arr, type);
            (**arr) = (**arr) << 2;
            (**arr) += tmp;
            L--;
        }
        else
        {
            if (is_register(firstArgs))
            {
                args_classify(firstArgs, Table, arr, type);
                (**arr) = (**arr) << 5;
                (*arr)++;
                args_classify(secondArgs, Table, arr, type);
            }
            else if (is_register(secondArgs))
            {
                args_classify(firstArgs, Table, arr, type);
                (*arr)++;
                args_classify(secondArgs, Table, arr, type);
                (**arr) <<= 2;
            }
            else
            {
                args_classify(firstArgs, Table, arr, type);
                (*arr)++;
                args_classify(secondArgs, Table, arr, type);
            }
        }
    }
    (*arr)++;
    return 0;
}
int parse_opcode(char *token, SymbolTable *Table, int **arr)
{
    opcodeType type;
    type = address_mathods(token);
    IC++;
    switch (type)
    {
    case 1:
        prase_args(Table, token, arr, ONE_OPRAND);
        break;
    case 2:
        prase_args(Table, token, arr, TWO_OPRAND);
        break;
    default:
        break;
    }
    return 1;
}
void parsing_a_line(instructionType ins, char *cur_feild, char *prev_word, SymbolTable *st, int **arr)
{
    /*
        Line analysis according to the nature of the first command in the line
    */
    switch (ins)
    {
    case 0:
        parse_label(prev_word, cur_feild, st, arr);
        break;
    case 1:
        parse_opcode(cur_feild, st, arr);
        break;
    case 5:
        mark_as_entry(cur_feild, st);
        break;
    case 7:
        printf("An error occurred Something improper in the input %s\n", prev_word);
        break;
    default:
        break;
    }
}

void parse_label(char label[MAX_LINE_LENGTH], char *token, SymbolTable *Table, int **arr)
{
    instructionType sec_ins;

    token = strtok(NULL, "\n\t\r ");

    sec_ins = get_instruction_type(token);
    switch (sec_ins)
    {
    case 0:
        printf("erorr: you statment are not legal - %s can not be in the argument in LABEL", token);
        break;
    case 1:
        parse_opcode(token, Table, arr);
        break;
    case 5:
        printf("%s should be the first argument in the line", token);
    case 6:
        printf("%s should be the first argument in the line", token);
    default:
        break;
    }
}

void incLineCount()
{
    line_number++;
}

int ignoreEmptyLine(char *string)
{
    if (*string == ';' || strlen(string) <= 1)
    {
        incLineCount();
        return 1;
    }
    return 0;
}
int get_line_number()
{
    return line_number;
}

int second_pass(char *input_filename, SymbolTable *Table, int **arr)
{
    int *tmp;
    char line[MAX_LINE_LENGTH], firstField[MAX_LINE_LENGTH], *currFileName, *token;
    instructionType instruction;
    FILE *file_ptr;

    ext_file = NULL;

    currFileName = (char *)malloc(strlen(input_filename) + EXT_LEN);
    fileName = (char *)malloc(strlen(input_filename) + EXT_LEN);

    strcpy(fileName, input_filename);
    sprintf(currFileName, "%s%s", input_filename, OUTPUT_EXT);

    file_ptr = fopen(currFileName, "r");
    if (!file_ptr)
    {
        perror("Error: can't open am file");
        exit(EXIT_FAILURE);
    }

    tmp = (*arr);
    IC = 0;
    printf("Running the second pass for %s! \n", input_filename);
    while (fgets(line, MAX_LINE_LENGTH, file_ptr) != NULL)
    {
        line_number++;
        if (ignoreEmptyLine(line))
        {
            continue;
        }

        token = strtok(line, "\n\t\r ");
        strcpy(firstField, token);
        instruction = get_instruction_type(token);

        parsing_a_line(instruction, token, firstField, Table, arr);
        IC = IC + L;
        L = 0;
    }
    (*arr) = tmp;
    print_to_entry_file(Table);

    fclose(file_ptr);

    free(currFileName);

    fclose(ext_file);

    return 0;
}