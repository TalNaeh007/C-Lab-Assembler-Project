#include "header.h"
/* This file contains the first pass function */

/* Phase one function will go through each line

* DC and IC = 0
* read the first line
  It will first check for a labelName -
            if it has a labelName - rise a labelName flag
            check for a .data/.string/.struct directive for data storage.
                if it has directive - insert labelName to the Symbol Table with a tag (Data tag) and its value will be the value of DC(Data-Counter)
                                            if the labelName exist in the Symbol Table - Error.
                if it's not a directive for data storage -
                    if it's .extern or .entry directive
                        if its .extern directive - insert each labelName showing as operand of the directive to the Symbol Table with no value with external tag.
                        Read the Next line.
                    if it's NOT .extern or .entry directive -
                        insert the labelName definition to the Symbol Table with a code tag and its value will be the value of IC (Instruction Counter).
                            if the labelName is exists in the Symbol Table - Error.
                        Search for the opname in the OpTable
                            if it's not exists - Error "Wrong Operand"
            Analyze Operand of the directive
            Calculate L (Number of words taken by the binary code of the current directive).
            Generate binary code.
            Update IC (current IC + L)
            Read next line.
* If any errors were found in the source code
    Stop program.
* update value of data labelName type in the Symbol Table by adding the final value of IC.
* Identify the data type
     Encode the data type in the memory and update DC according to their size.
*/

extern bool containsExterns;
extern bool containsEntries;

/**
 * The main function of the first pass. Initializes the wordNode/symbol list and sets their respective values.
 * Prints an error message for each error found in the source code.
 * 
 * @param fptr - File pointer to be read from.
 *
 * @param DC - Data count 
 * @param IC - Instruction count
 */
void phaseOne(FILE * fptr, int *DC, int *IC)
{
    char line[MAX_LINE_LENGTH]; /* A line buffer to store each line */
    int lineNumber = 0; /* Count lines */

    while (fgets(line, MAX_LINE_LENGTH, fptr)) /* Read the next line until EOF is reached */
    {
        int directiveType; /* Directive's number */
        char *currentToken = NULL; /* Token buffer to store each currentToken */
        char *labelName = NULL; /* Label buffer to store each labelName's name */
        lineNumber++; /* Increment line counter by one in every new iteration */
        currentToken = strtok(line, "\t \n"); /* Tokenize the line by spaces and tabs */

        if(currentToken == NULL)/* Skips the line if it is blank */
        {
            continue;
        }

        if(isComment(currentToken)) /* Skips the line if it is a comment */
        {
            continue;
        }

        if((labelName = getLabel(currentToken))) /* If labelName is not NULL */
        {
            /* Handling errors if getLabel returned any of the error string flags */
            if(strcmp(labelName, "Invalid label name\n") == 0)
            {
                containsErrors = true;
                printf("Invalid label name at line %d\n", lineNumber);/*prints error*/
                labelName = NULL; /*set labelName to null*/
            } 
            else if(strcmp(labelName, "Invalid label name - Reserved word - Operation name/Register name/Directive name\n") == 0)
            {   
                /* containsErrors = true; */ /* FIX */
                printf("line %d : Label name cannot be used\n", lineNumber);
            }

            currentToken = strtok(NULL, "\t \n"); /* Move on to the next currentToken */

            if(currentToken == NULL) /* If the next currentToken is NULL, print an error and go to next line */
            {
                containsErrors = true;
                printf("line %d : add an expression after label name\n", lineNumber);
                continue;
            }
        }

        if((directiveType = getDirective(currentToken))) /* If directive is found */
        {
            if(directiveType == ENTRY_DIRECTIVE) /*if directiveType is entry*/
            {
                if(labelName != NULL) 
                {
                    containsErrors = true;
                    printf("line %d: Label name before entry directive type has no meaning and won't be "
                           "calculated\n",lineNumber); /*if we have a labelName before entry directiveType*/
                }

                if(!(currentToken = strtok(NULL, "\t \n"))) /*continue*/
                {
                    containsErrors = true;
                    printf("line %d : Entry directive type must have a label name \n", lineNumber);
                    continue;
                }

                addEntry(currentToken, lineNumber); 
                containsEntries = true;
                if((currentToken = strtok(NULL, "\n")) ) /* If there's a more text in the current line */
                {
                    containsErrors = true;
                    printf("Entry directiveType can only take one parameter in line %d \n",
                           lineNumber); /* Print an error and continue */
                    continue;
                } 
                
                else /* If the next currentToken is null */
                {
                    continue;
                }
            }
            else if(directiveType == EXTERN_DIRECTIVE) /* If the directiveType is extern */
            {
                if(labelName != NULL) 
                {   
                    containsErrors = true;
                    printf("line %d: labelName \"%s\" before entry directiveType is meaningless and will be ignored\n",
                           lineNumber, labelName);
                }

                if(!(currentToken = strtok(NULL, "\t \n"))) /* Next token */
                {
                    containsErrors = true;
                    printf("Extern directiveType must have a labelName parameter in line %d\n", lineNumber);
                    continue;
                }

                addSymbol(currentToken, 0, true, false, lineNumber); /* Add the extern symbol to the symbol table */
                containsExterns = true;

                if((currentToken = strtok(NULL, "\n"))) /* If the next word is not null */
                {
                    containsErrors = true;
                    printf("Extern directiveType can only take one parameter in line %d\n", lineNumber);
                    continue;
                } 
                
                else  /* Next currentToken is null */
                {
                    continue;
                }
            }

            else if(directiveType == DATA_DIRECTIVE || directiveType == STRING_DIRECTIVE || directiveType == STRUCT_DIRECTIVE)
                /* If the directiveType is a data/string/struct directiveType */
            {

                if(directiveType == DATA_DIRECTIVE)
                {
                    if(labelName != NULL) /* if label name is not null add to symbol list */
                    {
                        addSymbol(labelName, *DC, false, false, lineNumber);
                        breakToTokens(currentToken, lineNumber, DC); /*splits string to tokens of int*/
                    } 
                    
                    else if(labelName == NULL)/* If labelName was not found */
                    {
                        breakToTokens(currentToken, lineNumber, DC);
                    }
                }

                else if(directiveType == STRING_DIRECTIVE)  /* If the directiveType is a string directiveType */
                {
                    currentToken = strtok(NULL, "\n"); /* Get the next currentToken */
                    if(currentToken == NULL) /* If the next currentToken is empty */
                    {
                        containsErrors = true;
                        printf("Must specify a string in line %d\n", lineNumber); /* Print an error and continue to the next line */
                        continue;
                    }
                    if(labelName != NULL) /* If a labelName was found, add to the symbol list */
                    {
                        addSymbol(labelName, *DC, false, false, lineNumber);
                        addStr(currentToken, lineNumber, DC); /* add string*/
                        continue;
                    } 
                    
                    else if(labelName == NULL) /* If no labelName was found */
                    {
                        addStr(currentToken, lineNumber, DC); /* Add to data count*/
                        continue;
                    }
                }

                else if(directiveType == STRUCT_DIRECTIVE) /*if directiveType is struct directiveType*/
                {
                    if(labelName != NULL) /*if labelName exists*/
                    {
                        addSymbol(labelName, *DC, false, false, lineNumber);
                        breakToTokens(currentToken, lineNumber, DC); /* Splits the string to int tokens */

                    }

                    else if(labelName == NULL) 
                    {
                        breakToTokens(currentToken, lineNumber, DC); /* Split the string to int tokens */
                    }
                }
            }

            else if(directiveType == ERROR_DIRECTIVE)
            {
                containsErrors = true;
                printf("Line %d : Invalid directive type \n", lineNumber);
                continue;
            }
        }

        else /* If the currentToken is not a directiveType */
        {
            /* Create a relevant wordNode */
            createWords(currentToken, lineNumber, IC, labelName);
        }
    }
    
    if(lineNumber == 0) /* If the file is empty */
    {
        printf("No input in file - FILE IS EMPTY.\n");
    }
}
