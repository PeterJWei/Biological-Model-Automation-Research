/* esp2model-lib.h */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void freeData(void *data);
void printData(void * data);
int parse(char ***dictionary, char*word, int *count,int*diclen,void *data,int dFlag);
void printFile( char **dictionary, int *count, FILE *stream,void *data);
void loadFile(char *infileName, char ***dictionary, int *diclen,int*count, void* data, int dFlag);
void insertToDictionary(char *** dictionary, char * token, int *count, int*diclen, int dFlag);
void* workData(char ***dictionary,void *data,int*count);
