/*
--- PRE-ASSEMBLER ALGO---

1. Open the file and get the first line. (If == EOF, return)
2. Get the first word of the line. (If it was found in the macro table, delete its name and replace it with its data, go back to #1)
3. If the first word is "macro", find its name and insert it into the table. (If it already exists, print an error message)
  (If it's not "macro", skip to #5)
4. When found "endmacro", delete it from the file and continue
5. Otherwise write the line to the output file.
5. Go back to #1
*/

#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H


#include <stdlib.h>
#include "files_io.h"
#include "commands.h"

#define MAX_TABLE_SIZE 50
#define MAX_NAME_SIZE 20
#define MAX_LINE_LEN 80

static int inMacro = false;
static int hasMacro = false;
static char curLine [MAX_LINE_LEN]; /* A static array that holds the current line */

/**
 * Main pre-assembler function. Locates macros in the file, expands their data and creates the .am file.
 * @param char *filename - The filename of the file to be pre-assembled.
 */
void preAssembler(char*);
/* --- TEXT PROCESSING FUNCTIONS --- */

/**
 * Gets the first word from a given string
 * @param char *str - the string to get the first word from
 * @return - Returns a pointer to first word from a given string 
 */
char* getFirstWord(char*);

/**
 * Gets the next line from a given file
 * @param FILE* f1 - the file to get the next line from
 * @return - Returns a pointer to first word from a given line in the file, or NULL if EOF
 */
char* getLine(FILE*);

/**
 * Skips spaces and returns a pointer to the next non-white space character
 * @param char* line - the string to get the next non-white space character from
 * @return - Returns a pointer to the next non-white space character
 */
char* skipSpaces(char*);

/**
 * Returns the given string without white-spaces at the beginning
 * @param char* str - the string to delete the non-white space characters from
 * @return - Returns a pointer to the string with no white-spaces at the beginning
 */
char* deleteSpaces(char*);

/**
 * Copies the macro's name into the 2nd char pointer
 * @param char* ptr1 - the string to copy the macro's name from
 * @param char* mName - the string to copy the macro's name into
 */
void getMacroName(char*, char*);

/**
 * Copies the macro's data into the 2nd char pointer
 * @param FILE* fpmacro - the file to get the data from
 * @param char* source - the string to copy the macro's data from
 * @param char* mData - the string to copy the macro's data into
 */
void getMacroData(FILE*, char*, char*);

/* --- END OF TEXT PROCESSING FUNCTIONS --- */

/* Imports all the macro names and their data to the macro table and creates a .am file with the expanded macros */

/**
 * Imports all the macro names and their data to the macro table and creates a .am file with the expanded macros
 * @param char* macroName - A string to hold the macro's name
 * @param char* macroData - A string to hold the macro's data
 * @param char* original - A string to hold the original file name
 * @param char* tempFile - A string to hold the expanded file name
 */
void extractMacros(char*, char*, char*, char*);

/**
 * Returns true if found the string "macro " in the line
 * @param FILE* source- The file to get the line from
 * @param char* name - A string to hold the macro's name
 * @param char* data - A string to hold the macro's data
 * @param char* line - A string to hold the next line
 */
bool containsMacro(FILE*, char*, char*, char*);


/* A symbol object to hold the macros */
typedef struct macro
{
    char name[MAX_NAME_SIZE]; /* Macro's name */
    char data[5*MAX_LINE_LEN]; /* Macro's data */

}macro;

/* --- HASH TABLE --- */

/**
 * Main hash function, returns the symbol's index
 * @param char* name - the string to create the index from
 * @return - Returns the macro's index number
 */
unsigned int hash(char*);

/**
 * Initializes the static table to NULL pointers
 */
void initialize_table();

/**
 * Prints any none-NULL objects in the table
 */
void print_table();

/**
 * Creates a new symbol object and returns a pointer to it 
 * @param char* name - Macro's name
 * @param char* data - Macro's data
 * @return - Returns a pointer to the new macro object
 */
macro* createMacro(char*, char*);

/**
 * Inserts a new macro into the table
 * @param macro* mac - A pointer to the macro to be inserted
 * @return - * Returns true if the macro has been successfuly inserted into the table. Returns false otherwise 
 */
bool insertMacro(macro*);

/**
 * Frees up the table's memory
 */
void delete_table();

/**
 * Searches for a macro in the table
 * @param char* name - The macro's name to search for
 * @return - Returns a pointer to the symbol if it exists in the table, returns NULL otherwise
 */
macro* hashTableLookup(char*);

static macro* hashTable[MAX_TABLE_SIZE]; /* The static table */

#endif
