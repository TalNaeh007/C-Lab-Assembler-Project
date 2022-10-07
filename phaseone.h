#ifndef PHASEONE_H
#define PHASEONE_H
#include "header.h"
#include "operations.h"

extern bool containsErrors;

/**
 * Exits the program if memory allocation fails 
 * @param ptr - The pointer to be checked
 */
void checkMalloc(void * ptr)
{
    if(ptr == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(0);
    }
}

/**
 * Determines if the input is a directive, and if so, returns the proper enum
 * @param token - The string to be checked
 * @return Returns an enum int of the proper directive,
 * if there is no such directive - an error, if the string doesn't start with ".", - NONE
 */ 
int getDirective(char *token)
{
    
    if(*token == '.') /* The first char must be '.' if the string is a directive */
    {
        if(strcmp(token, DIRECTIVE_DATA_STR) == 0) /* If the token is a data directive */
        {
            return DATA_DIRECTIVE;
        }

        else
        if(strcmp(token, DIRECTIVE_STRING_STR) == 0) /* If the token is a string directive */
        {
            return STRING_DIRECTIVE;
        }
        
        if(strcmp(token, DIRECTIVE_STRUCT_STR) == 0) /* If the token is a struct directive */
        {
            return STRUCT_DIRECTIVE;
        }
        
        if(strcmp(token, DIRECTIVE_ENTRY_STR) == 0) /* If the token is an entry directive */
        {
            return ENTRY_DIRECTIVE;
        }
        
        if(strcmp(token, DIRECTIVE_EXTERN_STR) == 0) /* If the token is an extern directive */
        {
            return EXTERN_DIRECTIVE;
        }

        else /* If the token is an illegal directive */
        {
            return ERROR_DIRECTIVE;
        }
    }
    else /* If the token is not a directive */
    {
        return NONE_DIRECTIVE;
    }
}

/**
 * Determines if the token is a comment or not
 * @param token - The string to be checked
 * @return Returns true if the line is a comment, false otherwise
 */
int isComment(char *token)
{
    if(*token == ';')
        return true;
    else
        return false;
}

/**
 * @param token - The string to determine if it is an operation
 * @return Returns true if the string parameter is an operation
 */

int isOp(char *token)
{
    int i;
    for (i = 0; i < NUMBER_OF_VALID_COMMANDS ; i++)
    {
        if(strcmp(token, commands[i].name) == 0) /* Goes through the command names array */
        {
            return true;
        }
    }
    return false;
}

/**
 * @param token - The token to determine if it is a directive
 * @return - Returns true if the token is a directive, false otherwise
 */
int isDir(char *token)
{
    int i;

    for (i = 0; i < NUMBER_OF_DIRECTIVES ; i++) /* Goes through the directive names array */
    {
        if(strcmp(token, directives[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

/**
 * Determines if the token is an index 
 * @param token - The token to determine if it is an index
 * @return Returns true if the passed token is of an index type
 */
int isInd(char *token)
{
    int i;
    for (i = 0; i < strlen(token); i++) /* Goes through the token's characters */
    {
        if(token[i] == '.') /* If the token contains a '.' */
            return true;
    }
    return false;
}

/**
 * Determines if the token is a register
 * @param token - The string to determine if it is a register
 * @return Returns true if the string parameter is a register, false otherwise
 */
int isReg(char *token)
{
    int i;

    for (i = 0; i < NUMBER_OF_REGISTERS ; i++) /* Goes through the register names array */
    {
        if(strcmp(token, registers[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

/**
 * Checks for the validity of a labelName
 * @param token - The string to be check
 * @return - Returns the labelName if it is valid, invalid labelName otherwise 
 */
char *getLabel(char *token)
{
    char *ptr = token;

    if(token[strlen(token) - 1] == ':') /* If the last char is ':' */
    {
        if(isalpha(*token)) /* If the first letter is valid (A-Z) */
        {
            token[strlen(token) - 1] = 0; /* Set null terminator */
            while ((*ptr) != '\0')
            {
                if(!isalnum(*ptr)) /* If any of the chars is not alphanumeric*/
                {
                    containsErrors = true;
                    return "Invalid label name";
                }
                ptr++;/* Set ptr to the next char */
            }

            if(isOp(token)|isReg(token)|isDir(token)) /* If the token is an op/register/directive name,
                                                          return the reserved word error flag */
            {
                containsErrors = true;
                return "Invalid label name - Reserved word - Operation name/Register name/Directive name";
            }
            return token;
        }

        else /* If the first char is not valid */
        {
            containsErrors = true;
            return "Invalid label name";
        }
    }
    
    else
    {
        return NULL;
    }
}

#endif /* PHASEONE_H */
