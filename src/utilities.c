#include "data.h"
/*array of opcode commands*/
char *opcodearray[AMOUNT_OF_OPCODE] = {
    "mov",
    "cmp",
    "add",
    "sub",
    "not",
    "clr",
    "lea",
    "inc",
    "dec",
    "jmp",
    "bne",
    "red",
    "prn",
    "jsr",
    "rts",
    "hlt",
};
/*array of opcode register name*/
char *regArr[AMOUNT_OF_REGISTER] = {
    "r0",
    "r1",
    "r2",
    "r3",
    "r4",
    "r5",
    "r6",
    "r7",
    "r8",
};

/*check if the token is opcode*/
opcode is_opcode(char *token)
{
    int i;
    for (i = 0; i < AMOUNT_OF_OPCODE; i++)
    {
        if (!strcmp(token, opcodearray[i]))
        {
            return (opcode)opcodearray[i];
        }
    }
    return no_opcode;
}

int get_entry_val(char *first_field, SymbolTable *Table)
{
    /*get the value of given label/data string name */
    int val = 0;
    SymbolEntry *entry;

    entry = get_entry(first_field, Table);
    if (entry != NULL)
    {
        val = entry->value;
    }
    entry = NULL;
    return val;
}

instructionType get_instruction_type(char *word)
{
    /*get the type of instruction by the given string pointer*/
    opcode ins = is_opcode(word);
    if (ins > 0)
    {
        return OPCODE;
    }
    else if (!strcmp(word, DEFINE_DECLARTION))
    {
        return DEF_INS;
    }
    else if (!strcmp(word, EXTERN_DECLARTION))
    {
        return EXT_INS;
    }
    else if (!strcmp(word, ENTRY_DECLARTION))
    {
        return ENT_INS;
    }
    else if (*(word + strlen(word) - 1) == ':')
    {
        return LABEL;
    }
    else if (!strcmp(word, DATA_DECLARTION))
    {
        return DATA_INS;
    }
    else if (!strcmp(word, STRING_DECLARTION))
    {
        return STR_INS;
    }
    return EROOR;
}

opcodeType address_mathods(char *opName)
{
    /*return the amount of argument of any given opcode command*/
    int i;
    for (i = 0; i < (AMOUNT_OF_OPCODE + 1); i++)
    {
        if (!strcmp(opcodearray[i], opName))
        {
            if (i < 4 || i == 6)
            {
                return TWO_OPRAND;
            }
            else if (i < 14 && i != 6)
            {
                return ONE_OPRAND;
            }
            else
                return ZERO_OPRAND;
        }
    }
    return -1;
}

/* Function to replace tabs ('\t') with spaces (" ") */
void replaceTabs(char *str)
{
    char *ptr = str;

    while (*ptr != '\0')
    {
        if (*ptr == '\t')
        {
            *ptr = ' ';
        }
        ptr++;
    }
}

/* Write lines into file */
void writeToFile(FILE *file, char *lines)
{
    if (file)
    {
        fprintf(file, "%s", lines);
        return;
    }
}
/* Search for a symbol in the symbol table*/
SymbolEntry *get_entry(char *token, SymbolTable *Table)
{
    SymbolEntry *entry;
    entry = (SymbolEntry *)malloc(sizeof(SymbolEntry));

    for (entry = Table->entries; entry != NULL; entry = entry->next)
    {
        if (!strcmp(entry->key, token))
        {
            return entry;
        }
    }
    return NULL;
}

/* Get Opcode (if exsits) */
opcode whichOpcode(char *command)
{

    if (strcmp(command, "mov") == 0)
    {
        return mov;
    }
    if (strcmp(command, "cmp") == 0)
    {
        return cmp;
    }
    if (strcmp(command, "add") == 0)
    {
        return add;
    }
    if (strcmp(command, "sub") == 0)
    {
        return sub;
    }
    if (strcmp(command, "not") == 0)
    {
        return is_not;
    }
    if (strcmp(command, "clr") == 0)
    {
        return clr;
    }
    if (strcmp(command, "lea") == 0)
    {
        return lea;
    }
    if (strcmp(command, "inc") == 0)
    {
        return inc;
    }
    if (strcmp(command, "dec") == 0)
    {
        return dec;
    }
    if (strcmp(command, "jmp") == 0)
    {
        return jmp;
    }
    if (strcmp(command, "bne") == 0)
    {
        return bne;
    }
    if (strcmp(command, "red") == 0)
    {
        return red;
    }
    if (strcmp(command, "prn") == 0)
    {
        return prn;
    }
    if (strcmp(command, "jsr") == 0)
    {
        return jsr;
    }
    if (strcmp(command, "rts") == 0)
    {
        return rts;
    }
    if (strcmp(command, "hlt") == 0)
    {
        return hlt;
    }

    return no_opcode;
}

/* Get Register (if exsits) */
registers whichRegister(char *command)
{
    if (strcmp(command, "r0") == 0)
    {
        return r0;
    }
    if (strcmp(command, "r1") == 0)
    {
        return r1;
    }
    if (strcmp(command, "r2") == 0)
    {
        return r2;
    }
    if (strcmp(command, "r3") == 0)
    {
        return r3;
    }
    if (strcmp(command, "r4") == 0)
    {
        return r4;
    }
    if (strcmp(command, "r5") == 0)
    {
        return r5;
    }
    if (strcmp(command, "r6") == 0)
    {
        return r6;
    }
    if (strcmp(command, "r7") == 0)
    {
        return r7;
    }

    return no_register;
}

/* Get Instructions (if exsits)*/
char *whichInstruction(char *instruction)
{

    if (strcmp(instruction, ".data") == 0)
    {
        return "data";
    }
    if (strcmp(instruction, ".string") == 0)
    {
        return "string";
    }
    if (strcmp(instruction, ".entry") == 0)
    {
        return "entry";
    }
    if (strcmp(instruction, ".extren") == 0)
    {
        return "extren";
    }

    return "no_instruction";
}

/* Clean leading & ending spaces */
void clean_spaces(char *str)
{

    char *end;
    char *prev_str = str;
    char *out = (char *)(malloc(MAX_LINE_LENGTH));

    strcpy(out, str);

    /* Remove leading spaces */
    while (isspace((unsigned char)*out))
        out++;

    /* Check if it was all the spaces */
    if (*out == 0)
        return;

    /* Remove ending spaces */
    end = out + strlen(out) - 1;
    while (end > out && isspace((unsigned char)*end))
        end--;

    end[1] = '\0';

    strcpy(prev_str, out);
}

/* Function to free the memory allocated for the symbol table */
void freeSymbolTable(SymbolTable *symbol_table)
{
    SymbolEntry *current_entry = symbol_table->entries;
    SymbolEntry *next_entry;

    while (current_entry != NULL)
    {
        next_entry = current_entry->next;
        free(current_entry);
        current_entry = next_entry;
    }
}