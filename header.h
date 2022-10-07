#ifndef HEADER_H
#define HEADER_H
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "commands.h"
#include "operations.h"
#include "structs.h"
#include "phaseone.h"
#include "symbolList.c"
#include "dataList.c"
#include "entryList.c"
#include "operations.c"
#include "files_io.h"


/*phaseOne.c functions*/

void phaseOne(FILE * fp,int *DC,int *IC);

/* utilsphaseOne.c */
int getDirective(char * token);
int isComment(char * token);
char *getLabel(char * token);
void checkMalloc(void * ptr);


/* symbolList.c functions */
void addSymbol(char * label, int address, int external, int op, int line);
bool symbolExistence(char *label, int line);
void updateSymbols(int IC);
void updateDataListAddress(int IC);
symbol *searchLabel(char * label);


/* dataList.c functions */
void addStr(char *string, int line_counter, int *DC);
void addChar(char ch, int line);
void breakToTokens(char *token, int line_number, int *DC);
void addNum(char *number, int line, int *DC);
data *getHeadData();


/* entryList.c functions */
void addEntry(char *label, int line);
bool checkEntryLabelName(char *label);
entry *getHeadEntry();
entry *getNotAddressedEntries();


#endif
