#include "constants.h"

typedef struct Macro
{
    char name[MAX_LINE_LENGTH];
    char *lines;
} Macro;

typedef enum SymbolType
{
    mdefine,
    code,
    data,
    external,
    entry
} SymbolType;

typedef enum
{
    LABEL,
    OPCODE,
    DATA_INS,
    STR_INS,
    EXT_INS,
    ENT_INS,
    DEF_INS,
    EROOR
} instructionType;

typedef enum
{
    ZERO_OPRAND,
    ONE_OPRAND,
    TWO_OPRAND
} opcodeType;

/* Define a structure for a symbol table entry */
typedef struct SymbolEntry
{
    char key[MAX_LINE_LENGTH];
    SymbolType type;
    int value;
    struct SymbolEntry *next;
} SymbolEntry;

/* Define a structure for the symbol table */
typedef struct SymbolTable
{
    SymbolEntry *entries;
    int count;
} SymbolTable;

typedef enum opcode
{
    mov,
    cmp,
    add,
    sub,
    is_not,
    clr,
    lea,
    inc,
    dec,
    jmp,
    bne,
    red,
    prn,
    jsr,
    rts,
    hlt,
    no_opcode = -1
} opcode;

typedef enum registers
{
    r0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    no_register
} registers;

typedef enum
{
    False,
    True
} Bool;