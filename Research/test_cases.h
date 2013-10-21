#include <stdio.h>
#include <stdlib.h>
struct table {
    char** vars;
    int* poss;
    int n;
    int* arrA;
    int* arrB;
    int arrSize;
};
typedef struct table* tables;

static int arrAt[27] = {0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 2, 1, 1, 1, 1, 1, 1};
static int arrBt[27] = {0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1, 3, 0, 0, 1, 0, 0, 0, 0, 1, 3, 0, 0, 1};

static int arrA5[16] = {0, 1, 1, 2, 1, 2, 2, 2, 0, 0, 0, 1, 0, 1, 1, 2};//16 = product over poss
static int arrB5[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};//holds delays

static int arrA6[72] = {0, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
static int arrB6[72] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
tables* tests() {
////////////
// test 1 //
////////////
    int i = 3;
    int j, k;
    tables* tests = malloc(sizeof(tables) * i);
    for (k = 0; k < i; k++) {
        tests[k] = malloc(sizeof(struct table));
    }

    int n = 4;
    tests[0] -> n = n;
    tests[0] -> vars = malloc(sizeof(char*) * n);
    tests[0] -> poss = malloc(sizeof(int) * n);
    int arrSize = 27;
    tests[0] -> arrSize = 27;
    tests[0] -> arrA = malloc(sizeof(int) * arrSize);
    tests[0] -> arrB = malloc(sizeof(int) * arrSize);
    tests[0] -> vars[0] = "ERK_CURRENT";
    tests[0] -> vars[1] = "DUSP_MKP";
    tests[0] -> vars[2] = "ERK_ACT";
    tests[0] -> vars[3] = "ERK";
    tests[0] -> poss[0] = 3;
    tests[0] -> poss[1] = 3;
    tests[0] -> poss[2] = 3;
    tests[0] -> poss[3] = 3;
    for (j = 0; j < 27; j++) {
        tests[0] -> arrA[j] = arrAt[j];
        tests[0] -> arrB[j] = arrBt[j];
    }

////////////
// test 2 //
////////////
    n = 5;
    tests[1] -> n = n;
    tests[1] -> vars = malloc(sizeof(char*) * n);
    tests[1] -> poss = malloc(sizeof(int) * n);
    tests[1] -> vars[0] = "IGF1RA";
    tests[1] -> vars[1] = "MEK";
    tests[1] -> vars[2] = "TRA";
    tests[1] -> vars[3] = "TGFBRA";
    tests[1] -> vars[4] = "ERK_ACT";
    tests[1] -> poss[0] = 2;
    tests[1] -> poss[1] = 2;
    tests[1] -> poss[2] = 2;
    tests[1] -> poss[3] = 2;
    tests[1] -> poss[4] = 3;
    arrSize = 16;
    tests[1] -> arrSize = arrSize;
    tests[1] -> arrA = malloc(sizeof(int) * arrSize);
    tests[1] -> arrB = malloc(sizeof(int) * arrSize);
    for (j = 0; j < 16; j++) {
        tests[1] -> arrA[j] = arrA5[j];
        tests[1] -> arrB[j] = arrB5[j];
    }
////////////
// test 3 //
////////////
    n = 6;
    tests[2] -> n = n;
    tests[2] -> vars = malloc(sizeof(char*) * n);
    tests[2] -> poss = malloc(sizeof(int) * n);
    tests[2] -> vars[0] = "ROS";
    tests[2] -> vars[1] = "ASNOS_C";
    tests[2] -> vars[2] = "TRA";
    tests[2] -> vars[3] = "MKK36";
    tests[2] -> vars[4] = "IRA";
    tests[2] -> vars[5] = "P38_MAPK_ACT";
    tests[2] -> poss[0] = 3;
    tests[2] -> poss[1] = 3;
    tests[2] -> poss[2] = 2;
    tests[2] -> poss[3] = 2;
    tests[2] -> poss[4] = 2;
    tests[2] -> poss[5] = 3;
    arrSize = 72;
    tests[2] -> arrSize = arrSize;
    tests[2] -> arrA = malloc(sizeof(int) * arrSize);
    tests[2] -> arrB = malloc(sizeof(int) * arrSize);
    for (j = 0; j < arrSize; j++) {
        tests[2] -> arrA[j] = arrA6[j];
        tests[2] -> arrB[j] = arrB6[j];
    }

    return tests;
}
