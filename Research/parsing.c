#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define debug

struct arraylen {
    char **A;
    int len;
};

const char* f(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

int count(char* line)
{
    int count = 0;
    char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        count += 1;
    }
    return count;
}

struct arraylen* placevars(char* varname, char* line, char ** vars)
{
#ifdef debug
    printf("varname: %s\nline: %s\n", varname, line);
#endif
    int count = 0;
    int len = 0;
    int i = 0;
    char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        len = strlen(tok);
        char* new = malloc(sizeof(char) * len);
        strcpy(new, tok);
        vars[count] = new;
        count += 1;
        //free(new);
    }
    char **A = malloc(sizeof(char *) * (count + 1));
    for (i; i < count; i++) {
        A[i] = vars[i];
    }
    varname = strtok(varname, ",\n");
    A[count] = varname;
    //free(vars);
    struct arraylen* arraystruct = malloc(sizeof(struct arraylen));
    arraystruct -> A = A;
    arraystruct -> len = count + 1;
    return arraystruct;
}

int *varsposs(char *numline)
{
    char *tok;
    int* temp = malloc(sizeof(int)*100);
    int count = 0;
    for (tok = strtok(numline, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        temp[count] = atoi(tok);
        count++;
    }
    int* poss = malloc(sizeof(int) * count);
    int i;
    for (i = 0; i < count; i++) {
        poss[i] = temp[i];
    }
    return poss;
}

void makeArrays(int *A, int *B, FILE* stream) {
    char *tok;
    char *line = (char *)malloc(sizeof(char) * 1024);
    char *delay = malloc(sizeof(char) * 10);
    int stren;
    int first = 1;
    int count = 0;
    while (fgets(line, 1024, stream)) {
        first = 1;
        for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
        {
            strcpy(delay, "");
            if (first == 1) {
                first = 0;
//                printf("skipped %s\n", tok);
            }
            else {
//                printf("Scanning %s\n", tok);
                sscanf(tok, "%d%s", &stren, delay);
//                printf("Got %d strength, %s delay\n", stren, delay);
                A[count] = stren;
                if (!strcmp("d", delay)) {
//                    printf("Got 1 delay!\n");
                    B[count] = 1;
                }
                else if (!strcmp("dd", delay)) {
                    B[count] = 2;
//                    printf("Got 2 delay!\n");
                }
                else if (!strcmp("ddd", delay)) {
                    B[count] = 3;
//                    printf("Got 3 delay!\n");
                }
                else if (!strcmp("dddd", delay)) {
                    B[count] = 4;
                }
                else if (!strcmp("ddddd", delay)) {
                    B[count] = 5;
                }
                else if (!strcmp("dddddd", delay)) {
                    B[count] = 6;
                }
                else if (!strcmp("ddddddd", delay)) {
                    B[count] = 7;
                }
                else if (!strcmp("dddddddd", delay)) {
                    B[count] = 8;
                }
                else if (!strcmp("ddddddddd", delay)) {
                    B[count] = 9;
                }
                else if (!strcmp("dddddddddd", delay)) {
                    B[count] = 10;
                }
                else {
                    B[count] = 0;
                }
            count += 1;
            }
        }
    }
    //free(line);
    //free(delay);
}
        

struct table* p(char *filename)
{
    struct table* A = malloc(sizeof(struct table) * 1);   
    FILE* stream = fopen(filename, "r");
    if (stream == NULL) {
        printf("NULL!\n");
        return 0;
    }
/////////////////////////////////////////////////////
//Section 1: Create variable array
    char *varname = malloc(sizeof(char) * 400);
    char *line = (char *)malloc(sizeof(char) * 1024);
    size_t len = 0;
    fgets(varname, 400, stream);
    fgets(line, 1024, stream);
    char** varstemp = malloc(sizeof(char *) * (100));
    struct arraylen* arrstruct = placevars(varname, line, varstemp);
    char** vars = arrstruct -> A;
    len = arrstruct -> len;
    A -> n = len;
    //free(varname);
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////
//Section 2: Get the possible variable strengths
    char *numline = malloc(sizeof(char) * 1024);
    fgets(numline, 1024, stream);
    int *poss = varsposs(numline);
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////
//Section 3: Determine size of the array
    int counter = 1;
    int i;
    for (i = 0; i < len - 1; i++) {
        counter *= poss[i];
    }
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////
//Section 4: Make the strength and delay arrays
    int *arrA = malloc(sizeof(int) * counter);
    int *arrB = malloc(sizeof(int) * counter);
    fgets(numline, 1024, stream);

    makeArrays(arrA, arrB, stream);
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////
//Section 5: Make the table
    A -> vars = vars;
    A -> poss = poss;
    A -> n = len;
    A -> arrA = arrA;
    A -> arrB = arrB;
    A -> arrSize = counter;
/////////////////////////////////////////////////////
#ifdef debug
    for (i = 0; i < A -> n; i++) {
        printf("varname %d: %s\n", i, A -> vars[i]);
        printf("possibility: %d\n", poss[i]);
    }
    for (i = 0; i < A -> arrSize; i++) {
        printf("strength %d, delay %d\n", arrA[i], arrB[i]);
    }
#endif
    fclose(stream);
    return A;
}
