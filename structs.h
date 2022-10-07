#ifndef STRUCTS_H
#define STRUCTS_H

/* Definition of the symbol type */
typedef struct symbol
{
    char *labelName;
    char *entExtName;
    int labelType;
    int address;
    short int value; /* ADDED - for data types. Else - set to NULL*/
    int operation; 
    int external;
    struct symbol * next;
} symbol;

/* Definition of the data label type */
typedef struct data 
{
    int address;
    short int value;
    struct data * next; 
    int line;
} data;

/* Definition of the entry label type */
typedef struct entry
{
    char *labelName;
    int address;
    int addressInserted;
    int line;
    struct entry *next;
} entry;


/* Definition of the first word type */
typedef struct firstWord
{
    unsigned int era: 2; /* ERA bits */
    unsigned int destination: 2; /* Destination operand bits */
    unsigned int source: 2; /* source operand bits */
    unsigned int opcode: 4;  /* Opcode bits */
    char * opcodeName;
} firstWord;

/* Definition of immediate word type:
  Used to store either an immediate number or a memory address */
typedef struct ImmediateWord
{
    unsigned int era: 2; /* ERA bits */
    unsigned int number: 8; /* Bits used to hold the value/address */
} ImmediateWord;

/* Definition of index register word type */
typedef struct IndexWord
{
    unsigned int era: 2; /* era bits */
    unsigned int sourceAddressingMethod: 2; /* Indexing addressing addressingMethod bits */
    unsigned int destinationAddressingMethod: 2; /* Field number in the of the index */
    unsigned int opcode : 4;
} IndexWord;


typedef struct RegisterWord/* Definition of register word type */
{
    unsigned int era: 2; /* ERA bits */
    unsigned int destination: 4; /* Destination register bits */
    unsigned int source: 4; /* Source register bits */
} RegisterWord;

/* Word union, used to store the different types of words of a specific line */
typedef union WordTypes
{   data data;
    firstWord first;  /* firsWord type */
    ImmediateWord immediate; /* Immediate word type */
    IndexWord index; /* Index word type */
    RegisterWord reg; /* Register word type */
} WordTypes;


/* Definition of the wordNode type */
typedef struct wordNode
{
    WordTypes word;
    int address;
    int addressingMethod; /* Addressing addressingMethod - which word is it? */
    int line;
    char *missingLabelName; /* Missing labelName */
    char *externalLabelName; /* Label of an external */
    bool isExternal;     /* External flag */
    struct wordNode *next;    /*Pointer to next*/
} wordNode;

typedef struct opcode
{
    char *name; /* The command string */
    int code;  /* The opcode */
    int groupNum; /* The command group it belongs to */
} opcode;

/* A static array for storing opcode information */
static opcode commands[NUMBER_OF_VALID_COMMANDS] = {{"mov", 0,  0}, {"cmp", 1, 0}, {"add", 2, 0},
                                                    {"sub", 3,  0}, {"not", 4, 1}, {"clr", 5, 1}, {"lea", 6, 0}, {"inc", 7, 1},
                                                    {"dec", 8,  1}, {"jmp", 9, 1}, {"bne", 10, 1}, {"get", 11, 1}, {"prn", 12, 1},
                                                    {"jsr", 13, 1}, {"rts", 14, 2}, {"hlt", 15, 2}};


enum labelType {DATA, EXTERN, ENTRY, STRING, STRUCT, NON_LABEL}; /* Label types */

#endif
