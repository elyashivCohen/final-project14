
#ifndef PRE_ASM_H
#define PRE_ASM_H

#define MAX_MACRO_COUNT 50
#define START_OF_MACRO "mcr"
#define END_OF_MACRO "endmcr"

typedef struct Macro
{
    char name[MAX_LINE_LENGTH];
    char *lines;
} Macro;

void ignoreLeadingSpaces(char *line);
int findMacro(Macro *m, int size, char *name);
void addLinesToMacro(Macro *macro, const char *newLine);
int pre_asm(char *input_filename);

#endif