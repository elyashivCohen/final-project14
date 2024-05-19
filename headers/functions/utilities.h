
#ifndef UTILITIES
#define UTILITIES

/* Define the symbol types in the symbol table */

void replaceTabs(char *str);
void writeToFile(FILE *file, char *lines);
opcode whichOpcode(char *command);
registers whichRegister(char *command);
char *whichInstruction(char *instruction);
void clean_spaces(char *str);
void freeSymbolTable(SymbolTable *symbol_table);
SymbolEntry *get_entry(char *token, SymbolTable *Table);
int get_line_number();
int get_entry_val(char *first_field, SymbolTable *Table);
opcodeType address_mathods(char *opName);
instructionType get_instruction_type(char *word);
opcode is_opcode(char *token);

#endif