#ifndef PHASETWO_H
#define PHASETWO_H
/*
 ---- ALGO DESCRIPTION --- 
    1. Initialize IC = 0
    2. Open the file and get the first/next line. (If == EOF, return)
    3. If the first field is a symbol, skip it
    4. If it's a .data, .string, .struct, .extern direcives, go back to #2
    5. If it's not a .entry directive, go to #7
    6. If it's a .entry directive, mark the matching label as an entry point and go back to #2.
    7. Complete the binary code for the 2nd word, according to the delivery method.
            If an operand is a symbol, look it up in the symbol table.
    8. IC = IC + Line_Count
    9. Go back to #2
    10. If an error has been found (use a flag), stop the program.
    11. Create and write to the output files. Machine code file (.ob object),
        .ext external file for external symbols, and .ent entry file for entry points.
 */

/* --- FUNCTIONS --- */

#include "phaseone.c"

/**
 * Main function of the second pass. Updates any missing labels and writes the output files.
 */
void phaseTwo();

bool containsErrors = false;
bool containsEntries = false;
bool containsExterns = false;

void phaseTwo()
{

    symbol *symbolPtr = NULL;
    wordNode *wordPtr = NULL;
    entry *entryPtr = NULL;
    while ((wordPtr = getMissingLabel()))
    {
        symbolPtr = searchLabel(wordPtr->missingLabelName);
        /* If no such symbol was found, print an error */
        if(!symbolPtr)
        {
            containsErrors = true;
            printf("\nSymbol was not found");
            return;
        }

        else
        {
            if(symbolPtr->external == true)
            {
                containsExterns = true;
                wordPtr->word.immediate.era = true;
                wordPtr->word.immediate.number = false;
                wordPtr->isExternal = true;
                wordPtr->externalLabelName = symbolPtr->labelName;
            }

            else if (symbolPtr->external == false)
            {
                wordPtr->word.immediate.era = 2;
                wordPtr->word.immediate.number = symbolPtr->address;
                wordPtr->word.immediate.number += BEGIN_INSTRUCTION_ADDRESS;
            }
            wordPtr->missingLabelName = NULL;
        }
    }
    
    while((entryPtr = getNotAddressedEntries()))
    {
        symbolPtr = searchLabel(entryPtr->labelName); /* Search for symbols with the same label */
        
        if(!symbolPtr) /* If there is no such symbol, add an error */
        {
            entryPtr->addressInserted = true;
        }

            /* If a symbol was found, assign its address to the entry node */
        else
        {
            entryPtr->address = symbolPtr->address;
            entryPtr->address += BEGIN_INSTRUCTION_ADDRESS;
            entryPtr->addressInserted = true;
        }
    }
}
#endif /* PHASETWO_H */
