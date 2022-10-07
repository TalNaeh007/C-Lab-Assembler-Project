#include "header.h"


static data *head_data = NULL; /* A static pointer to the head of the data list */

/**
 * Get the head of the data list
 * @return - Returns a pointer to the head of the data list
 */
data * getHeadData()
{
    data *pData = head_data;
    return pData;
}
/**
 * Add a char to the data list
 * @param ch - The char to be added to the list
 * @param line - The line number of the char
 */
void addChar(char ch, int line)
{
    data *pData = malloc(sizeof(data)); /* pData data node */
    pData->value = ch; /* Sets the char to the node's value */
    pData->line = line; 
    pData->next = NULL; 

    if(head_data == NULL) /* If the list is empty, set the node as the head */
    {
        head_data = pData; 
    }

    else /* If the data list is not empty, add the node to the end of the list */
    {
        data *p = head_data; 
        while (p->next) /* Iterate through the list */
            p = p->next;
        p->next = pData;
    }
}

/**
 * Add a string to the data list
 * 
 * @param string - The char string to be added to the list
 * @param line_counter - The line number of the char/string
 */
void addStr(char *string, int line_counter, int *DC) /* Adds a string to the data list */
{
    if((*string == '"') && (string[strlen(string) - 1] == '"')) /* Check quotation marks at the beginning and at the end of the string */
    {
        string[strlen(string) - 1] = '\0'; /* Change the right quotation mark to null terminator */
        string++; /* Fix the offset so the string pointer will point to the first char after the left quotation mark */
    }

    else  /* If there are no quotation marks at the beginning and at the end of the string, print an error and return */
    {
        printf("Strings must have quotation marks at the beginning and at the end of the string : line %d\n", line_counter);
        return;
    }

    while (*string != '\0') /* If haven't reached null terminator, add to symbol list */
    {
        addChar(*string, line_counter); /* Add the character */
        string++; /* Fix the offset */
        (*DC)++; /* Increment data count */
    }
    addChar(0, line_counter); /* Add 0 as the null terminator */
    (*DC)++; 
}


void addNum(char *number, int line, int *DC) /* Add a number to the data list */
{
    char *ptr = NULL; /*A pointer for error checking */
    data *pData = NULL; /* Data node */
    int num;

    num = strtol(number, &ptr, 10); /* Convert the string to a decimal number and assigned it num */
    if(*ptr != '\0') /* If strtok didn't return a null terminator */
    {
        if(*number == '"')
        {
            addStr(number, line, DC);
            return;
        }

        else
        {
            containsErrors = true;
            printf("The number is invalid : line %d\n", line); /* Prints error and exit function */
            return;
        }
    }

    if((num < MIN_NUM) || (num > MAX_NUM)) /* If number is out of the 10-bit range */
    {
        containsErrors = true;
        printf("Number is out of range in line %d", line); /* Print error and exit function */
        return;
    }

    pData = malloc(sizeof(data)); /* Data pointer */
    checkMalloc(pData);

    pData->next = NULL;
    pData->line = line; /* Set line number */
    pData->value = (short int)num;

    if(head_data == NULL)
    {
        head_data = pData; /* Set the node as the head */
    }

    else
    {
        data *p = head_data; /* Set the other pointer as the head */
        while (p->next) /* Set pointer p to point to the last node */
            p = p->next;
        p->next = pData; /* Set pData as the last node */
    }
    (*DC)++; /* Increment data count by one */
}
 
 /**
  * Splits the passed string to int tokens. If string is not null - loop through the tokens and add them to the data list.
  * 
  * @param token - The string to be split
  * @param line_number - The line number of the string
  * @param DC - Data count pointer
  */
void breakToTokens(char *token, int line_number, int *DC)
{
    if((token = strtok(NULL, "\n ,\t") )== NULL) /* If the token is null */
    {
        containsErrors = true;
        printf("Error in line %d", line_number); /* Print error */
        return;
    }

    else
    {
        addNum(token, line_number, DC); /* Send the first token to addNum - EXPLAIN */
        while ((token = strtok(NULL, "\n ,\t"))) /* Send the rest of the tokens to addNum - EXPLAIN */
        {
            addNum(token, line_number, DC);
        }
    }
}

/**
 * Update address words contained in the data list - EXPLAIN
 * 
 * @param IC - Instruction count
 */
void updateDataListAddress(int IC)
{
    data *p = NULL; /* Get the head of the data list */
    p = head_data;
    if(p == NULL)
        return;
    IC += BEGIN_INSTRUCTION_ADDRESS; /* Update IC */
    if(p == NULL) /* If the list is empty */
        return;

    while (p) /* Iterate through the list */
    {
        p->address = IC;
        IC++; /* Increment the instruction count */
        p = p->next;
    }
}

