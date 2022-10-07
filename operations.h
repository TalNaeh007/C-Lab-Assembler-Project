#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "commands.h"
#include "structs.h"
#include "header.h"
#include "phaseone.h"
#include <math.h>

#define BEGIN_INSTRUCTION_ADDRESS 100 /* Memory beginning address */
#define POSITION 1 /* Position of the immediate value in the instruction */
#define MAX_BASE32_DIGITS 3 /* Maximum number of digits in base32 to represent any address */

/* An array that represents each special base-32 symbol as its index number */
char symbols[32]={'!','@','#','$','%','^','&','*','<','>','a','b','c','d',
'e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};

/* A static pointer to the beginning of the wordNode list */
static wordNode *head = NULL;

/* ----- FUNCTION DECLARATIONS  ----- */

int isOp(char *token);
int isReg(char *token);
int isDir(char *token);
int isInd(char * token);

void createWords(char * token, int line, int *IC, char * label);
void addWord(wordNode * node);
int isValidLabelName(char *token, int line);
wordNode* getMissingLabel();
wordNode* getHeadWord();

void updateAddresses();
char *decimalToBase32(int num);


/* ----- FUNCTION DEFINITIONS  ----- */

/**
 * Goes through the wordNode list and updates all of the words with their corresponding addresses
 */
void updateAddresses()
{
    int address = BEGIN_INSTRUCTION_ADDRESS; /* The address of the first instruction */
    wordNode *ptr = head; /* List's head */
    if(ptr == NULL)
        return;
    if(!ptr) /* If reached the end of the list */
        return;

    while (ptr)
    {
        ptr->address = address++; /* Update the address of the word and increment address */
        ptr = ptr->next;
    }
}


/**
 * Searches for a word with a missing labelName and returns a pointer to it if found
 * @return - Returns a wordNode pointer to w word with missing labels if found one,
 * or NULL if no such word was found
 */
wordNode* getMissingLabel()
{
    wordNode *p = head; /* A pointer to the list's head */

    while (p)
    {
        if(p->missingLabelName) /* Iterating through the list */
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}



/**
 * Gets the wordNode's list's head
 * @return wordNode* - Returns the head of the wordNode list
 */
wordNode* getHeadWord()
{
    wordNode * tmp;
    tmp = head;
    return tmp;
}

/**
 * Adds the node parameter to the end of the list
 * @param node - The wordNode to be added to the list
 */
void addWord(wordNode * node)
{
    
    if(!head) /* If the list is empty, initialize the head to the passed node */
    {
        head = node;
    }
        
    else /* If the list is not empty, go to the end of it and add the node */
    {
        wordNode *p = head;

        while(p->next)
            p = p->next;

        p->next = node;
    }
}

/**
 * Converts a decimal number to base32
 * @param int num - The decimal number to be converted to base-32
 * @return - Returns a string of the base-32 representation of the passed decimal number
 */
char *decimalToBase32(int num) /* NEEDS REFACTORING - EXPLAIN moZ var */
{
    int i = 0, j = 0, count = 0, moZ = 0, position = POSITION;
    unsigned int abs = num;
    char temp;
    char bin[10]; /* A temporary array to hold the binary representation of the decimal number */
    char *base32Num = malloc(MAX_BASE32_DIGITS * sizeof(char));
    checkMalloc(base32Num);

    while(i < 10)
    {
        bin[i] = abs % 2 + '0';
        abs = abs/2;
        i++;
    }

    for(; j < 5 ; j++)
    {
        temp = (bin[9 - j]);
        bin[9 - j] = bin[j];
        bin[j] = (char)temp;
    }

    for(i = 9; i > -1; i--)
    {
        moZ += pow(2, count) * (bin[i]-'0');
        count++;

        if(count == 5)
        {
            base32Num[position] = symbols[moZ];
            position--;
            count = 0;
            moZ = 0;
        }
    }
    base32Num[2] = '\0';

    return base32Num;
}

/**
 * Gets the next relevant index token from the given string
 * 
 * @param str - The string to be searched
 * @return - Returns the next relevant index token from the given string
 */
char *cutIndexToken(char *str)
{
    int i = 0;
    char *substring = malloc(sizeof(char) * MAX_LINE_LENGTH);

    while(str[i] != '.') 
    {
        substring[i] = str[i];
        i++;
    }
    substring[i] = '\0';
    return substring;
}

int isValidLabelName(char *token, int line) 
{
    char *p;
    p = token;
    /* Check if the first char is a letter (valid labelName) */
    if(isalpha(*p)) 
    {
        /* Checks for invalid chars */
        while (*p != '\0') 
        {
            if((isalnum(*p)) == 0) 
            {
                printf("Invalid character in line %d\n", line);
                return false;
            }
            p++;
        }
        return true;
    }
    return false;
}

#endif /* OPERATIONS_H */
