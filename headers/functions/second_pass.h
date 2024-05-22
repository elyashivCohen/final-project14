#ifndef SECOND_PASS_H
#define SECOND_PASS_H

/*
##########################################################################################################
##########################################################################################################
#################################### second_pass.c General Overview: #######################################
##########################################################################################################
##########################################################################################################

In the second pass we complete the code words from the first pass by coding the commands (starting from the
second word) to binary code.
If there are instructions of .entry/.extern , the function will create entry/extern files.

##########################################################################################################
##########################################################################################################
##########################################################################################################
*/
int second_pass(char *input_filename, SymbolTable *st, int **arr);
/* @ Function: second_pass
   @ Arguments: The function gets char pinter to name of the file and pinter to symbol table and double pointer to binary code array.
   @ Description: The function receives the name of the input file from the command line and opens the file produced in the second pass
   according to format (am). reads line by line and analyzes the line using a function parsing_a_line. If there are instructions
   of .entry, the function will create entry files
*/

void parsing_a_line(instructionType ins, char *cur_feild, char *prev_word, SymbolTable *st, int **arr);
/* @ Function: parsing_a_line
   @ Arguments: the function gets instructionType of previous word, char pointer to current word and
   char pointer to previous word, pinter to symbol table, double pointer to binary code array.
   @ Description: the function gets parse of the instruction previous word and withdraws them according to:
   marking symbol table value as .entry, or parse the last of the line as the first word is label or opcode commandץ
*/

void parse_label(char label[MAX_LINE_LENGTH], char *token, SymbolTable *Table, int **arr);
/* @ Function: parse_label
   @ Arguments: The function gets char array - label name, char pointer that hold the current location in the line,
   and pinter to symbol table and double pointer to binary code array.
   @ Description: The function uses the saved position of the word and receives the argument of the label using the
   strtok function, and classifies its operations using the function get_instrction_type
*/
int parse_opcode(char *token, SymbolTable *Table, int **arr);
/* @ Function: parse_opcode
   @ Arguments: the function gets pinter to symbol table, char pinter to the opcode comennd, double pointer to bin code array.
   @ Description: the function parse the opcode Through the address_mathods (func the return how many args in which opcode)
   and call parse_args as per the amount of the args of any opcode command.
*/

int prase_args(SymbolTable *Table, char *token, int **arr, opcodeType type);
/* @ Function: parse_args
   @ Arguments: the function gets pinter to symbol table, char pinter to the curr location, double pointer to bin code array,
   and the amount of oprands.
   @ Description: the function parse the argumnts of opcode acoording the amount of opcodeType - amount of args of any opcode.
   using args_classify.
*/
void args_classify(char *args, SymbolTable *Table, int **arr);
/* @ Function: args_classify
   @ Arguments: the function gets pinter to symbol table, char pinter to the curr location, double pointer to bin code array.
   @ Description: the function parse the argumnts of opcode acoording to their type: "#, address of index, label, register"
   and calculate and insert to the word code array.
*/
opcode is_opcode(char *opName);

int is_register(char *reg);

int is_index_add(char *arg, char **firstPart, char **secondPart);

int mark_as_entry(char *cur_feild, SymbolTable *Table);

SymbolEntry *is_a_label(char *first_field, SymbolTable *Table);

int ignoreEmptyLine(char *string);

int print_to_entry_file(SymbolTable *Table);

int print_to_ext_file(int line, char *key);

Bool is_extern(SymbolEntry *entry, SymbolTable *Table);

#endif