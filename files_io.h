#ifndef FILES_IO_H
#define FILES_IO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern bool boolean;
extern char* strdup(const char*);

#define MAX_LINE_LEN 80

/**
 * Copies the first argument's contents into the a new file named by the second file string parameter
 * @param char* src - the filename of the file to be copied
 * @param char* dst - the filename of the new file to be created
 */
void fileCopy(char*, char*);

/**
 * Opens a file and returns a file pointer to its' beginning in read mode
 * @param char* name - the filename of the new file to be created
 * @return - Returns a file pointer to its' beginning in reading mode. Upon failure returns a NULL pointer
 */
FILE* openFileRead(char*);

/**
 * Opens a new file and returns a file pointer to its' beginning in write mode
 * @param char* name - the filename of the new file to be created
 * @return - Returns a file pointer to its' beginning in write mode. Upon failure returns a NULL pointer
 */
FILE* openFileWrite(char*);

/**
 * Checks if the end of the file was reached
 * @param FILE* fp - the file pointer to be checked
 * @return - Returns true if EOF was reached. Otherwise returns false
 */
bool checkEOF(FILE*);

/**
 * Renames the temp file to the original file name with an ".am" extension
 * @param char* original - The filename to be named as
 * @param char* temp - The filename of the temp file to be renamed
 */
void renameMacroFile(char*, char*);

/* -------- FUNCTION DEFINITIONS -------- */

FILE *openFileRead(char* name)
{
    FILE *fp;
    fp = fopen(name, "r");

    if(!fp)
    {
        printf("Error reading file");
        return NULL;
    }
    else 
        return fp;
}

FILE *openFileWrite(char* name)
{
    FILE *fp;
    fp = fopen(name, "r+");

    if(!fp)
    {
        printf("Error reading file");
        return NULL;
    }
    else
        return fp;
}

void fileCopy(char* src, char* dst)
{
    FILE* source;
    FILE* dest;
    char c;
    
    source = openFileRead(src);
    dest = openFileWrite(dst);

    if(!source || !dest)
    {
        printf("Error opening file: %s\n", src);
        return;
    }

    while((c = fgetc(source)) != EOF)
        {
            fputc(c, dest);
        }
    fclose(source);
    fclose(dest);    
}

bool checkEOF(FILE *source)
{
    if(fgetc(source) == EOF)
        return true;

    fseek(source, -1, SEEK_CUR);
    return false;
}

void renameMacroFile(char* original, char* temp)
{
    char *tempName = (char*)strdup(original);
    tempName = strtok(tempName, ".");
    tempName = strcat(tempName, ".am");
    rename(temp, tempName);
    free(tempName);
}

void create_ob_file(char*, int, int);

/**
 * If no errors were found, create the .ob file
 * 
 * @param tempObFileName - The name of the temp .ob file 
 * @param IC - Instruction count
 * @param DC - Data count
 */
void create_ob_file(char* tempObFileName, int IC, int DC)
{
    unsigned short int *toPrint;
    char *base32;
    char *base32value;
    FILE *fptr; /* Might need to move to main */
    wordNode *p; /* A wordNode pointer to navigate throught the list, might need malloc? */
    data *pd;
    char* obFileName = malloc(sizeof(char)*(strlen(tempObFileName) + 3*sizeof(char))); /* Preparing .ob filename */
    strcpy(obFileName, tempObFileName);
    strcat(obFileName, ".ob");

    fptr = fopen(obFileName, "w"); /* Opening .ob file */

    fprintf(fptr, "%s %s\n", decimalToBase32(IC), decimalToBase32(DC)); /* Writing IC/DC to the .ob file */

    p = getHeadWord();
    pd = getHeadData();
    
    /* char * base32; */ /* FOR TESTS */
    while (p) 
    {
        toPrint = (unsigned short int *) (&(p->word.first));
        /*Assign toPrint with corresponding word*/
       switch (p->addressingMethod) 
       {
            case NONE:
                toPrint = (unsigned short int *) (&(p->word.first));
            case IMMEDIATE_TYPE:
                toPrint = (unsigned short int *) (&(p->word.immediate));
            case DIRECT_TYPE:
                toPrint = (unsigned short int *) (&(p->word.immediate));
            case INDEX_TYPE:
                toPrint = (unsigned short int *) (&(p->word.index));
            case REGISTER_TYPE:
                toPrint = (unsigned short int *) (&(p->word.reg));
        }
        base32 = decimalToBase32(p->address);
        base32value = decimalToBase32(*toPrint);
        fprintf(fptr, "%s\t%s\n", base32, base32value);

        p = p->next;
    }
    free(p);

    printf("\n");
    while (pd) /* DATA PRINT */
    { 
        toPrint = (unsigned short int *) (&(pd->value));
        /*Assign toPrint with corresponding word*/

        base32 = decimalToBase32(pd->address);
        base32value = decimalToBase32(*toPrint);
        fprintf(fptr, "%s\t%s\n", base32, base32value);
        pd = pd->next;
        }

    free(pd);
    fclose(fptr);
    /* p might need free */
}

void create_ext_file(char*);

/**
 * If an extern has been found, create the .ext file
 * 
 * @param tempExtFileName - The name of the temp .ext file 
 */
void create_ext_file(char* tempExtFileName)
{
    FILE *fptr; /* Might need to move to main */
    wordNode *pw;
    
    char* ExtFileName = malloc(sizeof(char)*(strlen(tempExtFileName) + 5*sizeof(char))); /* Preparing .ext filename */

    strcpy(ExtFileName, tempExtFileName);
    strcat(ExtFileName, ".ext");
    pw = getHeadWord();
    fptr = fopen(ExtFileName, "w");

    while(pw){
        if(pw->isExternal == true){
            fprintf(fptr, "%s\t%s\n", pw->externalLabelName, decimalToBase32(pw->address));
        }
        pw = pw->next;
    }

    free(ExtFileName);
    fclose(fptr);
}

/* Creates a .ent file with the entry points  --- MOVE TO FILE_IO --- */
void create_ent_file(char*);

/**
 * If an entry has been found, create the .ent file
 * 
 * @param tempEntFileName - The name of the temp .ent file 
 */
void create_ent_file(char* tempEntFileName)
{
    FILE *fptr; /* Might need to move to main */
    entry *p; /* An entry pointer to navigate through the list */
    char* EntFileName = malloc(sizeof(char)*(strlen(tempEntFileName) + 5*sizeof(char))); /* Preparing .ent filename */
    strcpy(EntFileName, tempEntFileName);
    strcat(EntFileName, ".ent");

    fptr = fopen(EntFileName, "w");

    p = getHeadEntry();

    while(p != NULL) /* Iterate through the list */
    {
        fprintf(fptr, "%s\t%s\n", p->labelName, decimalToBase32(p->address));
        p = p->next;
    }

    free(EntFileName);
    fclose(fptr);
    
}

#endif /* FILES_IO_H */

