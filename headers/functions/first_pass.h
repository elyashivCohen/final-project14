#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#define IC_START_ADDRESS 100
#define MAX_NAME_LENGTH 31

int validate_source_method(int line_number, opcode op, int addressing_method);
int validate_destination_method(int line_number, opcode op, int addressing_method);
int get_addressing_method(int line_number, char *operand);
int get_num_of_opreands(int line_number, char *rest_of_line);
int get_opcode_opreands(opcode op);
void add_new_code(int **code, int *size, int value);
int is_reserved(char str[MAX_NAME_LENGTH]);
int is_valid_number(int line_number, char *value);
int is_valid_string_name(int line_number, char *str);
void increase_datas(SymbolTable *st, int IC);
int is_label(char *str);
void add_new_entry(SymbolTable *st, char *symbol_name, SymbolType type, int symbol_value);
int get_symbol_value(SymbolTable *st, char *symbol_name);
int check_symbol(int line_number, SymbolTable *st, char *symbol_name);
int first_pass(char *input_filename, SymbolTable *st, int **binary_code);

#endif