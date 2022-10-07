#ifndef COMMANDS_H
#define COMMANDS_H


#define NUMBER_OF_VALID_COMMANDS 16 /* Total number of the assembler's commands */
#define NUMBER_OF_REGISTERS 8
#define NUMBER_OF_DIRECTIVES 5 /* Total number of the assembler's directives */
#define MIN_NUM -512
#define MAX_NUM 511
#define MIN_IMI_NUM -128
#define MAX_IMI_NUM 127

/* DIRECTIVES */
#define DIRECTIVE_DATA_STR ".data"
#define DIRECTIVE_STRING_STR ".string"
#define DIRECTIVE_STRUCT_STR ".struct"
#define DIRECTIVE_ENTRY_STR ".entry"
#define DIRECTIVE_EXTERN_STR ".extern"

#define MAX_LINE_LENGTH 80


/* Opcodes for the assembler's commands */
#define OP_CMP 1
#define OP_LEA 6
#define OP_JMP 9
#define OP_BNE 10
#define OP_PRN 12
#define OP_JSR 13
#define OP_RTS 14
#define OP_HLT 15

/* An array to store the registers names */
static char* registers[8] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/* An array to store the directives names */
static char* directives[5] = {"data", "string", "struct", "entry", "extern"};

/* Directive enum */
enum directive {NONE_DIRECTIVE, DATA_DIRECTIVE,STRUCT_DIRECTIVE, STRING_DIRECTIVE,
                ENTRY_DIRECTIVE, EXTERN_DIRECTIVE, ERROR_DIRECTIVE};

/* Addressing method enum */
enum addressing_method {NONE = -1, IMMEDIATE_TYPE = 0, DIRECT_TYPE = 1, INDEX_TYPE = 2, REGISTER_TYPE = 3};

/* Boolean type for easier use */
typedef enum boolean
{
    false = 0, true = 1
}bool;

#endif
