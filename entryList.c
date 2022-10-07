/*entryList.c functions*/
#include "header.h"

static entry *head_entry = NULL;

/**
 * Get the head of the entry list
 * @return entry* - Returns a pointer to the head of the entry list
 */
entry *getHeadEntry()
{
    entry *pEntry = head_entry;
    return pEntry;
}

/**
 * Checks if the entry LabelName already exists in the list
 * 
 * @param label  - The label to be checked
 * @return - Returns true if found an entry with the same label, false otherwise
 */
bool checkEntryLabelName(char *label) 
{
    entry *p2Entry = head_entry;
    while (p2Entry)
    {
        if(strcmp(p2Entry->labelName, label) == 0)
        {
            return true;
        }

        else
        {
            p2Entry = p2Entry->next;
        }
    }
    return false;  /* A node with the same labelName doesn't exist in the list */
}

/**
 * Adds a label to the entry list
 * 
 * @param label - The label to be added to the entry list
 * @param line - The line number of the label
 */
void addEntry(char *label, int line)
{
    entry *pEntry = NULL;
    if((checkEntryLabelName(label)) == true) /* If the entry already exists */
    {
        printf("line %d : This entry name is already being used\n", line);
        return;
    }

    pEntry = malloc(sizeof(entry));
    checkMalloc(pEntry);
    pEntry->labelName = malloc(sizeof(char) * MAX_LINE_LENGTH);

    strcpy(pEntry->labelName, label);
    pEntry->line = line;
    pEntry->address = 0;
    pEntry->addressInserted = false;
    pEntry->next = NULL;

    if(head_entry == NULL)
    {
        head_entry = pEntry;
    }
    else /*while not reaching to the last node - point to last node*/
    {
        entry *entryP = head_entry;
        while (entryP->next != NULL)
            entryP = entryP->next;
        entryP->next = pEntry;
    }
}

/**
 * Get the entry words without addresses
 * @return - Returns a pointer to the entry words without addresses
 */ 
entry *getNotAddressedEntries()
{
    entry *pEntry = head_entry;

    while (pEntry)
    {
        if(pEntry->addressInserted == 0)  /* If addressInserted is false */
        {
            return pEntry;
        }
        else
        {
            pEntry = pEntry->next;
        }
    }
    return NULL;
}
