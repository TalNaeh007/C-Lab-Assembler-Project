#ifndef MAIN_H
#define MAIN_H
#include "phasetwo.c"
#include "operations.h"
#include "pre_assembler.c"


#define MAX_FILENAME_LEN 20

/* --- MAIN --- */

int main (int argc, char *argv[])
{
    char *fileName, *amFileName; /* String variables for filenames and addresses */
    int DC = 0, IC = 0, fileNameCounter = 1; /* Counters for IC, DC and file amount counter */

    FILE *fp, *fptr;
    containsErrors = false, containsEntries = false, containsExterns = false; /* Reset the flags */

    while(fileNameCounter < argc) /* As long as there are files to process */
    {
    /* File naming preparation and opening */
    fileName = malloc(MAX_FILENAME_LEN * sizeof(char)+3);
    checkMalloc(fileName);
    strcpy(fileName, argv[fileNameCounter]);
    strcat(fileName, ".as");

    fptr = fopen(fileName, "r"); /* Open the next file in read mode */

    if(fptr == NULL) /* If the file could not be located, go to the next file */
    {
        printf("\nCould not locate the file: %s \n", fileName);
        free(fileName);
        fileNameCounter++;
        containsErrors = false;
        continue;
    }
    fclose(fptr);

    preAssembler(fileName); /* Pre-assembler, macros expansion */
    /* File naming preparation and opening */
    
    
    amFileName = malloc(MAX_FILENAME_LEN * sizeof(char)+3);
    checkMalloc(amFileName);
    strcpy(amFileName, argv[fileNameCounter]);
    strcat(amFileName, ".am");

    fp = fopen(amFileName, "r");


    phaseOne(fp, &DC, &IC); /* First phase of the assembler */
    
    if(containsErrors == true) /* If there were errors in the file, skip to the next file */
    {
        fclose(fp);
        free(fileName);
        fileNameCounter++;
        continue;
    }

    /* If there were no errors, update the addresses and continue to the next phase */
    updateSymbols(IC);
    updateAddresses();
    updateDataListAddress(IC);

    phaseTwo(); /* Second phase of the assembler */
    
    fclose(fp); 
    /* NEED TO CHECK WHY CONDITIONS DON'T WORK - FIX */
    if(containsErrors == false)
        create_ob_file(argv[fileNameCounter], IC, DC);
    if(containsEntries == true)
        create_ent_file(argv[fileNameCounter]);
    if(containsExterns == true)
        create_ext_file(argv[fileNameCounter]);

    /* Memory cleanup */
    deleteSymbolTable(getHeadSymbol());
    deleteDataTable(getHeadData());
    deleteWordTable(getHeadWord());
    

    free(fileName);
    free(amFileName);

    containsErrors = false; /* Reset the error flag */
    fileNameCounter++; /* Go to the next file */
    }

    return 0;
}

#endif
