#include "header.h"

extern bool containsErrors;

/* ----- FUNCTION DECLARATIONS ----- */

void deleteSymbolTable(symbol*);
void deleteDataTable(data*);
void deleteWordTable(wordNode*);

void deleteSymbolTable(symbol* ptr)
{
    symbol *temp = NULL;
    while (ptr != NULL)
    {
        temp = ptr->next;
        free(ptr);
        ptr = temp;
    }
}

/* ----- FUNCTION DEFINITIONS ----- */

/**
 * Frees the memory allocated for the data table
 * @param ptr - Pointer to the data table
 */
void deleteDataTable(data* ptr)
{
    data *temp = NULL;
    while (ptr != NULL)
    {
        temp = ptr->next;
        free(ptr);
        ptr = temp;
    }
}
/**
 * Frees the memory allocated for the word table
 * @param wordNode - Pointer to the word table
 */
void deleteWordTable(wordNode* ptr)
{
    wordNode *temp = NULL;
    while (ptr != NULL)
    {
        temp = ptr->next;
        free(ptr);
        ptr = temp;
    }
}

/**
 * @brief Creates a new wordNode object with the corresponding op code groups
 * 
 * @param char *token - The token to be analyzed
 * @param line - Current line number
 * @param IC - Instruction count
 * @param label - Label of the token
 */
void createWords(char *token, int line, int *IC, char *label)
{
    int i, opGroup, registerNum, foundCommand = false;
    char *TokenToCopy = NULL;
    int totalICIncrements = 0; /* A counter for the IC increments */

    wordNode *first = NULL, *second = NULL, *third = NULL, *fourth = NULL; /* A pointer for each word */

    for(i = 0; i < NUMBER_OF_VALID_COMMANDS; i++) 
    {
        if(strcmp(token, commands[i].name) == 0)  /* If the matched with a valid command */
        {
            foundCommand = true;
            opGroup = commands[i].groupNum; /* Set the corresponding op group */

            /* Create the first word */
            first = malloc(sizeof(wordNode));

            checkMalloc(first);

            first->word.first.opcodeName = commands[i].name;
            first->addressingMethod = NONE;
            first->isExternal = false;
            first->line = line;
            first->word.first.opcode = commands[i].code;
            first->word.first.source = 0;
            first->word.first.destination = 0;
            first->word.first.era = 0;
            first->next = NULL;
            first->missingLabelName = NULL;
            first->externalLabelName = NULL;
            totalICIncrements++; /*Increment by one*/
            continue;
        }
    }

    if(foundCommand == false) /* If the command wasn't found in commands list */ 
    {
        containsErrors = true;
        if((token = strtok(NULL, "\n ,\t"))) /* Check for syntax errors */
        {
            containsErrors = true;
            printf("\nline %d: %s is not a valid command\n", line, token);
        }
        return;
    }


    if((token = strtok(NULL, "\n ,\t"))) /* Check for syntax errors */
    {
        if(opGroup == 0)  /* If the command belongs to the first group */
        {
            if(*token == '#')  /* If the first token is immediate number type */
            {   
                /* Converting the string to a decimal number */
                char *ptr = token;
                char *strtolPtrErrorIndicator;
                int num = strtol(ptr, &strtolPtrErrorIndicator, 10);
                ptr++;

                if(first->word.first.opcode == OP_LEA) /* If the command is lea, print an error and return */
                {
                    containsErrors = true;
                    printf("Immediate addressing method cannot be use with the command in line %d as first operand\n", line);
                    free(first);
                    return;
                }

                else if(*strtolPtrErrorIndicator != 0) /* If strtol fails */
                {
                    printf("Invalid number in line %d\n", line);
                    free(first);
                    return;
                }
                else if(num > MAX_IMI_NUM || num < MIN_IMI_NUM) /*number is not in range*/
                {
                    containsErrors = true;
                    printf("Immediate number is out of range in line %d - immediate numbers must be in the range"
                           " of -128 to 127\n", line);
                    free(first);
                    return;
                }

                else /* If the syntax is correct */
                {
                    /* Update the first word and create the second word */
                    first->word.first.source = IMMEDIATE_TYPE;
                    second = malloc(sizeof(wordNode));
                    second->addressingMethod = IMMEDIATE_TYPE;
                    second->isExternal = false;
                    second->line = line;
                    second->missingLabelName = malloc(sizeof(char) * MAX_LINE_LENGTH);
                    strcpy(second->missingLabelName, token);
                    second->word.immediate.number = num;
                    second->word.immediate.era = 0;
                    second->next = NULL;
                    second->externalLabelName = NULL;
                    totalICIncrements++;
                }
            }

            /* First token is an index type */
            else if(isInd(token))
            {
                char LastCharToken = token[strlen(token) - 1];
                int j = LastCharToken - '0';

                first->word.first.source = INDEX_TYPE;
                first->word.first.era = 0;
                first->addressingMethod = INDEX_TYPE;

                /* Create the second word */
                second = malloc(sizeof(wordNode));
                second->addressingMethod = DIRECT_TYPE;
                second->isExternal = false;
                second->line = line;
                TokenToCopy = cutIndexToken(token);
                second->missingLabelName = TokenToCopy;
                second->word.immediate.number = 0;
                second->next = NULL;
                second->externalLabelName = NULL;
                totalICIncrements++;

                /* Create the third word */
                third = malloc(sizeof(wordNode));
                third->addressingMethod = IMMEDIATE_TYPE;
                third->line = line;
                third->isExternal = false;
                third->word.immediate.number = j;
                third->word.immediate.era = 0;
                third->next = NULL;
                third->missingLabelName = NULL;
                third->externalLabelName = NULL;
                totalICIncrements++;

                /* Create the fourth word */
                fourth = malloc(sizeof(wordNode));
                fourth->addressingMethod = DIRECT_TYPE;
                fourth->line = line;
                fourth->isExternal = false;
                fourth->word.immediate.number = 0;
                fourth->next = NULL;
                fourth->missingLabelName = NULL;
                fourth->externalLabelName = NULL;
                totalICIncrements++;
            }

            /* First token is a register */
            else if(isReg(token))
            {
                if(first->word.first.opcode == OP_LEA) /* If the command is lea, add error and return */
                {
                    containsErrors = true;
                    printf("Register addressing method type cannot be used with the command in line %d as first operand\n", line);
                    free(first);
                    return;
                }
                registerNum = (token[1] - '0');

                first->word.first.source = REGISTER_TYPE;
                /* Create the second word */
                second = malloc(sizeof(wordNode));
                second->addressingMethod = REGISTER_TYPE;
                second->isExternal = false;
                second->line = line;
                second->word.reg.source = registerNum;
                second->word.reg.destination = 0;
                second->word.reg.era = 0;
                second->next = NULL;
                second->missingLabelName = NULL;
                second->externalLabelName = NULL;
                totalICIncrements++;
            }


            else /* If the first token is a label name*/
            {
                if(isValidLabelName(token,line) == 0)
                {
                    printf("Label must start with an alphabet char in line %d\n", line);
                    free(first);
                    return;
                }
                else
                {
                    first->word.first.source = DIRECT_TYPE;
                    /* Create the second word */
                    second = malloc(sizeof(wordNode));
                    second->addressingMethod = DIRECT_TYPE;
                    second->isExternal = false;
                    second->line = line;
                    second->missingLabelName = malloc(sizeof(char) * MAX_LINE_LENGTH);
                    strcpy(second->missingLabelName, token);
                    second->word.immediate.number = 0;
                    second->word.immediate.era = 0;
                    second->next = NULL;
                    second->externalLabelName = NULL;
                    totalICIncrements++;
                }
            }


            if((token = strtok(NULL, "\n ,\t"))) /* Get the second token */
            {
                if(*token == '#')
                {
                    int num;
                    char *ptr = token;
                    char *strtolPtrErrorIndicator;
                    ptr++;

                    num = strtol(ptr, &strtolPtrErrorIndicator, 10);

                    if(first->word.first.opcode != (OP_CMP))
                    {
                        containsErrors = true;
                        printf("Immediate addressing method type cannot be used with this command in line %d as second operand\n", line);
                        free(first);
                        free(second);
                        return;
                    }

                    if(*strtolPtrErrorIndicator != 0) /* If strtol fails */
                    {
                        printf("Invalid number in line %d\n", line);
                        free(first);
                        free(second);
                        return;
                    }

                    if(num > MAX_IMI_NUM || num < MIN_IMI_NUM) /* Number is not in range*/
                    {
                        containsErrors = true;
                        printf("Immediate number is out of range in line %d\n", line);
                        free(first);
                        return;
                    }

                    else /* If the syntax is correct */
                    {
                        first->word.first.destination = IMMEDIATE_TYPE;

                        /* Create the third word */
                        third = malloc(sizeof(wordNode));
                        third->addressingMethod = IMMEDIATE_TYPE;
                        third->isExternal = false;
                        third->line = line;

                        if(first->word.first.source == INDEX_TYPE)
                        {
                            ;
                        }
                        else
                        {
                            third->word.immediate.number = num;
                        }

                        third->word.immediate.era = 0;
                        third->next = NULL;
                        third->missingLabelName = NULL;
                        third->externalLabelName = NULL;
                        totalICIncrements++;
                    }
                }

                else if(isReg(token))
                {
                    if(second->addressingMethod == REGISTER_TYPE)
                    {
                        first->word.first.destination = REGISTER_TYPE;
                        second->word.reg.destination = (token[1] - '0');
                    }
                    else if(first->word.first.source == INDEX_TYPE)
                    {
                        first->word.first.destination = REGISTER_TYPE;
                        fourth->addressingMethod = REGISTER_TYPE;
                        fourth->word.reg.destination = (token[1] - '0');
                    }

                    else
                    {
                        first->word.first.destination = REGISTER_TYPE;
                        third = malloc(sizeof(wordNode));
                        third->addressingMethod = REGISTER_TYPE;
                        third->isExternal = false;
                        third->line = line;
                        third->word.reg.source = 0;
                        third->word.reg.destination = (token[1] - '0');
                        third->word.reg.era = 0;
                        third->next = NULL;
                        third->missingLabelName = NULL;
                        third->externalLabelName = NULL;
                        totalICIncrements++;
                    }

                }

                else if(isInd(token))
                {
                    fourth->address = *IC;
                }

                else
                {
                    char *ptr = token;
                    if(isValidLabelName(ptr,line))
                    {
                        if(first->word.first.source == REGISTER_TYPE)
                        {
                            first->word.first.destination = DIRECT_TYPE;
                            /*create the third word*/
                            third = malloc(sizeof(wordNode));
                            third->isExternal = false;
                            third->missingLabelName = malloc(sizeof(char) * MAX_LINE_LENGTH);
                            strcpy(third->missingLabelName, token);
                            third->addressingMethod = DIRECT_TYPE;
                            third->line = line;
                            third->word.immediate.number = 0;
                            third->word.immediate.era = 0;
                            third->next = NULL;
                            third->externalLabelName = NULL;
                        }

                        else if(first->word.first.source == INDEX_TYPE)
                        {
                            first->word.first.destination = DIRECT_TYPE;
                            fourth->addressingMethod = DIRECT_TYPE;
                            fourth->missingLabelName = malloc(sizeof(char) * MAX_LINE_LENGTH);
                            checkMalloc(fourth->missingLabelName);
                            strcpy(fourth->missingLabelName, token);
                            fourth->line = line;
                            totalICIncrements++;
                        }
                    }
                }
            }

            else /* Less than 2 arguments */
            {
                containsErrors = true;
                printf("The command in line %d must take 2 parameters\n", line);
                free(first);
                free(second);
                free(third);
                free(fourth);
                return;
            }
        }

        else if(opGroup == 1)  /* Command belongs to the second group */
        {
            if(*token == '#')  /* Token is an immediate number */
            {
                signed int num;
                char *ptr = token;
                char *strtolPtrErrorIndicator;
                ptr++;

                num = strtol(ptr, &strtolPtrErrorIndicator, 10); 

                if(*strtolPtrErrorIndicator != 0)  /* strtol fails*/
                {
                    printf("Invalid number in line %d\n", line);
                    free(first);
                    return;
                }

                else if(first->word.first.opcode != OP_PRN)
                {
                    containsErrors = true;
                    printf("The command in line %d can't take an immediate number as a first operand\n", line);
                    free(first);
                    return;
                }

                else if(num > MAX_IMI_NUM || num < MIN_IMI_NUM)
                {
                    containsErrors = true;
                    printf("Immediate number is out of range in line %d - immediate numbers must be in the range"
                           " of -128 to 127\n", line);
                    free(first);
                    return;
                }
                    /* If the syntax is correct */
                else
                {
                    first->word.first.destination = IMMEDIATE_TYPE;
                    /*create the second word*/
                    second = malloc(sizeof(wordNode));
                    second->addressingMethod = IMMEDIATE_TYPE;
                    second->isExternal = false;
                    second->line = line;
                    second->word.immediate.number = num;
                    second->word.immediate.era = 0;
                    second->next = NULL;
                    second->missingLabelName = NULL;
                    second->externalLabelName = NULL;
                    totalICIncrements++;
                }
            }

            else if(isReg(token)) /* Token is a register */
                {
                if(first->word.first.opcode != (OP_JMP | OP_JSR | OP_BNE))
                {
                    first->word.first.destination = REGISTER_TYPE;
                    second = malloc(sizeof(wordNode));
                    second->addressingMethod = REGISTER_TYPE;
                    second->isExternal = false;
                    second->line = line;
                    second->word.reg.source = 0;
                    second->word.reg.destination = (token[1] - '0');;
                    second->word.reg.era = 0;
                    second->next = NULL;
                    second->missingLabelName = NULL;
                    second->externalLabelName = NULL;
                    totalICIncrements++;
                }
                else
                {
                    containsErrors = true;
                    printf("Register cannot be assigned as operand in line %d\n", line);
                    free(first);
                }
            }

            else if(isInd(token)) /* Token is an index */
            {
                char LastCharToken = token[strlen(token) - 1];
                int j = LastCharToken - '0';

                first->word.first.source = INDEX_TYPE;
                first->word.first.era = 0;

                /* Create the second word */
                second = malloc(sizeof(wordNode));
                second->addressingMethod = DIRECT_TYPE;
                second->isExternal = false;
                second->line = line;
                second->word.immediate.number = 0;
                second->missingLabelName = NULL;
                second->externalLabelName = NULL;
                second->next = NULL;
                totalICIncrements++;

                /* Create the third word */
                third = malloc(sizeof(wordNode));
                third->addressingMethod = IMMEDIATE_TYPE;
                third->line = line;
                third->isExternal = false;
                third->word.immediate.number = j;
                third->word.immediate.era = 0;
                third->next = NULL;
                third->missingLabelName = NULL;
                third->externalLabelName = NULL;
                totalICIncrements++;


            }
            else  /* If it is a label name */
            {
                if(isValidLabelName(token,line))
                {
                    first->word.first.destination = DIRECT_TYPE;
                    /*create second word*/
                    second = malloc(sizeof(wordNode));
                    second->line = line;
                    second->addressingMethod = DIRECT_TYPE;
                    second->isExternal = false;
                    second->missingLabelName = malloc(sizeof(char) * MAX_LINE_LENGTH);
                    strcpy(second->missingLabelName, token);
                    second->word.immediate.number = 0;
                    second->word.immediate.era = 0;
                    second->next = NULL;
                    second->externalLabelName = NULL;
                    totalICIncrements++;
                }

                else
                {
                    containsErrors = true;
                    printf("Labels must start with a letter in line %d\n", line);
                    free(first);
                    free(second);
                    return;
                }
            }

            if((token = strtok(NULL, "\n ,\t")))
            {
                containsErrors = true;
                printf("Line %d : there are too many arguments for this command\n", line);
                free(first);
                free(second);
                free(third);
                return;
            }
        }

        else if(opGroup == 2)  /*command is in third group*/
        {
            containsErrors = true;
            printf("Command in line %d does not use arguments\n", line);
            free(first);
            return;
        }
    }

    else
    {
        if(first->word.first.opcode != OP_RTS && first->word.first.opcode != OP_HLT)
        {
            containsErrors = true;
            printf("Command in line %d must take arguments \n", line);
            free(first);
            return;
        }
    }
    if(label) /* Add it to the symbols table */
    {
        addSymbol(label, *IC, false, true, line);
    }

    /* Add the words to the list */
    if(first)
    {
        addWord(first);
    }
    if(second)
    {
        addWord(second);
    }
    if(third)
    {
        addWord(third);
    }
    if(fourth)
    {
        addWord(fourth);
    }

    *IC += totalICIncrements; /* Add the sum of increments to IC */
}
