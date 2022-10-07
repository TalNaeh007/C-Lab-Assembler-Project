#include "header.h"

static symbol *head_symbol = NULL; /* A static pointer to the symbol list's head */


/**
 * Determines if a symbol with the same name exists in the symbol table
 * 
 * @param label - The label to be checked
 * @param line - The line number of the label
 * @return - Returns true if found a symbol with the same label, false otherwise 
 */
bool symbolExistence(char *label, int line)
{
    symbol *pSymbol = head_symbol;
    while(pSymbol)
    {
        if(pSymbol->labelType == EXTERN || pSymbol->labelType == ENTRY)
        {
            if(strcmp(pSymbol->entExtName, label) == 0)
            {
                containsErrors = true;
                printf("\nline %d: labelName %s is already defined\n", line, label);
                return true;
            }
            pSymbol = pSymbol->next;
            continue;
        }

        if(strcmp(pSymbol->labelName, label) == 0)
            {
                containsErrors = true;
                printf("Symbol already exists in line %d\n", line);
                return true;
            }
        pSymbol = pSymbol->next;
    }
        return false;
}

/**
 * Adds the symbol to the symbol list
 * 
 * @param label - The name of the symbol
 * @param address - The address of the symbol
 * @param external - True if the symbol is external, false otherwise
 * @param op - The opcode of the symbol
 * @param line - The line number of the symbol
 */
void addSymbol(char *label, int address, int external, int op, int line)
{
    symbol *pSymbol = NULL;    /* pSymbol symbol pointer */
    pSymbol = malloc(sizeof(symbol)); 
    checkMalloc(pSymbol); 
    pSymbol->labelName = malloc(sizeof(char) * MAX_LINE_LENGTH);  
    checkMalloc(pSymbol->labelName);

    symbolExistence(label, line);  /* If the symbol already exists in the list */

    strcpy(pSymbol->labelName, label); /* Copy the labelName to this symbol's node */
    pSymbol->external = external; /* Set the external flag */
    pSymbol->address = address;   /* Copy the value of DC */
    pSymbol->operation = op; /* Copy op type */
    pSymbol->next = NULL; /* Set next to NULL */

    if (head_symbol == NULL) /* If the list is empty */
    {
        head_symbol = pSymbol; /* Set the head to point to pSymbol */
    }
    else /* If the list is not empty */
    {
        symbol * ptrSymbol = head_symbol; /* A pointer to the head */
        while (ptrSymbol->next != NULL)
            ptrSymbol = ptrSymbol->next;
        ptrSymbol->next = pSymbol; /* Set the last node point to pSymbol */
    }
}

/**
 *  Updates the symbols' addresses with the IC counter when phase one ends
 *  @param IC - The IC counter
 */
void updateSymbols(int IC) 
{
    symbol *pSymbol = head_symbol;
    if(head_symbol == NULL)
        return;
     /* Create pointer to head */
    while (pSymbol != NULL) 
    { 
        if(pSymbol->operation == false)
            if(pSymbol->external == false)
                pSymbol->address += IC;
        
        pSymbol = pSymbol->next;
    }
}


symbol* searchLabel(char *label)
{
    symbol *ptr = head_symbol;
    while (ptr != NULL)
    {
        if (strcmp(ptr->labelName, label) == 0)
            return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

/**
 * @brief Gets the head of the Symbol List
 * 
 * @return symbol* - The head of the Symbol List 
 */
symbol* getHeadSymbol()
{
    return head_symbol;
}
