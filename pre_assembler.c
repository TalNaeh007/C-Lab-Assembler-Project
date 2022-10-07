#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "pre_assembler.h"

/* --- FUNCTION DEFINITIONS --- */

void preAssembler(char* fileName)
{
    char macroName[20] = "";
    char macroData[5*MAX_LINE_LEN];

    printf("--------- Processing %s ---------\n", fileName);
	initialize_table();
    extractMacros(macroName, macroData, fileName, "tmp.txt");
    delete_table();
}

void getMacroName(char* ptr1, char* mName)
{
    int i, j;
    
    for(i = 0; !(isspace(ptr1[i])); i++) {}/* Finds the first space after the macro's appearance */
    for(j = 0; ptr1[i+1] != '\n' && ptr1[i+1] != EOF; i++, j++) /* Getting the macro's name */
        mName[j] = ptr1[i+1];   

    ptr1 = deleteSpaces(mName); /* White space deletion */
    for(i = 0; i < strlen(ptr1); i++)
        mName[i] = ptr1[i];
    mName[i] = '\0';   
}

void getMacroData(FILE* fpmacro, char *source, char* mData)

{
    while ((source = getLine(fpmacro)) && !(strstr(source, "endmacro"))) /* Getting macro's data */
                {
                    strcat(mData, source);
                }                         
}

char *skipSpaces(char* line)
{
    int i = 0;
    char c;
    while(isspace(c = line[i]))
    {
        i++;
    }
    return (line + i);
}

char* deleteSpaces(char* str)
{
    int i;
    char c;
    for(i = 0; (isspace(c = str[i])); i++) {}

    return (str + i);
}

char *getLine(FILE* f1)
{
    fgets(curLine, MAX_LINE_LEN, f1);
    return curLine;
}

void initialize_table()
{
    int i;
    for(i = 0; i < MAX_TABLE_SIZE; i++)
        hashTable[i] = NULL;
}

void print_table()
{
    int i;
    for(i = 0; i < MAX_TABLE_SIZE; i++)
    {
        if(hashTable[i] == NULL)
            continue;
    }
        putchar('\n');
}

macro* createMacro(char* name, char* data)
{
    int i;
    macro* mac = (macro*)malloc(sizeof(macro));
    checkMalloc(mac);
    
    for(i = 0; i < strlen(name); i++) /* Copies the name into the macro's name field */
        mac->name[i] = name[i];
        mac->name[i] = '\0';

    for(i = 0; i < strlen(data); i++) /* Copies the data into the symbol's data field */
        mac->data[i] = data[i];
        mac->data[i] = '\0';

    return mac;    
}

unsigned int hash(char* name) /* Sums the ASCII values of the characters in the name
                                 and returns the modulo quotient (by MAX_TABLE_SIZE) as the index  */
{
    unsigned int  i, length = strlen(name), hash_value = 0; 
     
    for(i = 0; i < length; i++)
    {
        hash_value += name[i]; /* Sums the ASCII values of the characters */
    }
    return hash_value % MAX_TABLE_SIZE;
}

bool insertMacro(macro *mac)
{
    unsigned int index;

    if(mac == NULL)
        return false;

    index = hash(mac->name); /* Compute hash function */

    if(hashTable[index] != NULL) /* Check if table's index is available */
        return false;

    hashTable[index] = mac;
    return true;    
        
}

void delete_table()
{
    int i;
    for(i = 0; i < MAX_TABLE_SIZE; i++)
        {
            if(hashTable[i] != NULL)
                free(hashTable[i]);
        }
}

macro* hashTableLookup(char *name)
{
    unsigned int index;
    index = hash(name);

    if(index == MAX_TABLE_SIZE + 1 || hashTable[index] == NULL) /* If the index is out of bounds or the table's index is NULL */
        return NULL;
    if((strcmp(hashTable[index]->name, name) == 0)) /* If the name is the same as the symbol's name */
        return hashTable[index];

    return NULL;
}

void extractMacros(char* macroName, char* macroData, char* original, char* tempFile)
{
    char *line, *lineCopy;
    FILE *source, *temp;
    unsigned int lineCount = 0;
    temp = fopen(tempFile, "w");
    source = openFileWrite(original);
	
    while ((line = getLine(source)))
    {
        lineCount++;

        if((line == NULL) | !(strcmp(line, "\n"))) /* Skips the line if it is blank */
        {
			fputc('\n', temp);
            checkEOF(source);
            
            continue;
        }

		lineCopy = (char*)strdup(line);
		if(strstr(line, "macro "))
		{
			inMacro = true;

			if(containsMacro(source, macroName, macroData, line))
            {   
				continue;
            }
		}
		
		strtok(lineCopy, " \n"); /* Prep macroName */
		lineCopy = deleteSpaces(lineCopy);

		if(hashTableLookup(lineCopy))
		{
            fprintf(temp, "%s", (hashTable[hash(lineCopy)])->data);
            continue;
       	}
		
		if(!(strstr(line, "macro ")))	
			fprintf(temp, "%s", line);	
        
		else
    
        inMacro = false;
        
        if(checkEOF(source))   
            break;
    } /* END OF WHILE */
	free(lineCopy);
    fclose(source);
    fclose(temp);

    /* Renames the temp file to the original file name */
    renameMacroFile(original, tempFile);	
}

char* getFirstWord(char* str)
{
    if((str == NULL) | (str[0] == '\0'))
        return " ";

    str = skipSpaces(str);
    str = strtok(str, " \n");
	str[strlen(str)] = '\0'; /* Planting a null terminator at the end of the string */
    return str;
}

bool containsMacro(FILE *source, char *name, char *data, char *line)
{
    char *mName, *mData;

    inMacro = true;
    hasMacro = true;
            
    line = deleteSpaces(line); 
    skipSpaces(line); /* Line cleanup */

    getMacroName(line, name);
            
    if(hashTableLookup(name) == NULL)
    {
        getMacroData(source, line, data);
        mName = (char*)strdup(name); /* Copies the macro name into an editable clone */
        mData = (char*)strdup(data); /* Copies the macro data into an editable clone */
        insertMacro(createMacro(mName, mData));
        memset(name, 0, MAX_NAME_SIZE);
        memset(data, 0, strlen(data) - 1);
       
        inMacro = false;
        return true;
    }
    return false;
}
