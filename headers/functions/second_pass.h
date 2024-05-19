#ifndef SECOND_PASS_H
#define SECOND_PASS_H

int second_pass(char *input_filename, SymbolTable *st, int **arr);
void parse_label(char label[MAX_LINE_LENGTH], char *token, SymbolTable *Table, int **arr);
int prase_args(SymbolTable *Table, char *token, int **arr, opcodeType type);
int parse_opcode(char *token, SymbolTable *Table, int **arr);
opcode is_opcode(char *opName);
int is_register(char *reg);
int is_index_add(char *arg, char **firstPart, char **secondPart);
int mark_as_entry(char *cur_feild, SymbolTable *Table);
SymbolEntry *is_a_label(char *first_field, SymbolTable *Table);
int ignoreEmptyLine(char *string);
#endif