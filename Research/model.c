#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "test_cases.h"
#include "parsing.c"
//#define tests
/*struct stren {
    int strength;
    int delay;
};*/
int model(int *arrA, int *arrB, int arrSize, int maxDelay, char **vars, int *poss, int molNums, int systemMax, FILE* fp);
int maxDelay(int* arrB, int arrSize);
void printRules(char ***rules, int num, char* str, int systemMax);
void printesp(int **esp, int realsum, int realsumwoffset, int arrSize, FILE* fp);
int** truthTables(int sum, int arrSize, int limit, int *poss, int *arrA, int *arrB, int maxDelay, int realsum);
int sumPoss(int *poss, int molNums);
void printPreRules(char ***rules, int num, int maxDelay, int arrSize, int *arrB, int *arrA, char **vars, int limit, int *poss);
void printheader(char **vars, int limit, int *poss, int arrSize, int delay, int rs, FILE* fp);
int n = 5;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: %s filename", argv[0]);
    }
    int i;
    struct table* test = p(argv[1]);
    int MD, molNum;

////////////////////////////////////
//Open new file to write to
    char* fname = malloc(sizeof(char) * 1024);
    strcpy(fname, strtok(argv[1], "."));
    strcat(fname, ".pla");
    printf("%s", fname);
    FILE* fp = fopen(fname, "w");
////////////////////////////////////
#ifdef tests
    tables* t = tests();
#endif

    model(test -> arrA, test -> arrB, test -> arrSize, maxDelay(test -> arrB, test -> arrSize), test -> vars, test -> poss, test -> n, 3, fp);

#ifdef tests
    for (i = 0; i < 3; i++) {
        char** vars = t[i] -> vars;
        int* poss = t[i] -> poss;
        int* arrA = t[i] -> arrA;
        int* arrB = t[i] -> arrB;
        int n = t[i] -> n;
        int arrSize = t[i] -> arrSize;
        MD = maxDelay(arrB, arrSize);
        molNum = n;
        fprintf(fp,"\n\n////////////////////////////////////////////////////\n");
        fprintf(fp,"test number %d, molecule %s, %d inputs\n", i, vars[n - 1], n - 1);
        fprintf(fp,"////////////////////////////////////////////////////\n");
        model(arrA, arrB, arrSize, MD, vars, poss, molNum, 3);
    }
#endif
    //molecule strength array, delay array, maximum delay given, molecule variable names, possible strengths, number of vars
}

int model(int *arrA, int *arrB, int arrSize, int maxDelay, char **vars, int *poss, int molNums, int systemMax, FILE* fp) {
    //p();
    int i, j, k;
    int numDelays = maxDelay;
    int limit = molNums - 1;
//    int delay = maxDelay(arrB, arrSize);
    int num = poss[limit];
    //set delay trackers
    
    char*** rules = malloc(sizeof(char**) * num); //strings of rules
    for (i = 0; i < num; i++) {
        rules[i] = malloc(sizeof(char *) * maxDelay + 1);
    }
    for (i = 0; i < num; i++) {
        for (j = 0; j < maxDelay + 1; j++) {
            rules[i][j] = malloc(sizeof(char) * 10000);
            strcpy(rules[i][j], "");
        }
    }
#ifdef debug
    printPreRules(rules, num, maxDelay, arrSize, arrB, arrA, vars, limit, poss);
    printRules(rules, num, vars[limit], systemMax);
#endif
    int sum = sumPoss(poss, molNums);
    int realsum = (sum - poss[limit]) * (maxDelay + 1) + poss[limit];
    int realsumwoffset = realsum - poss[limit];
#ifdef debug
    fprintf(fp,"sum: %d, offset: %d, maxDelay: %d, realsum: %d\n", sum, poss[limit], maxDelay, realsum);
#endif
    int** esp = truthTables(sum, arrSize, limit, poss, arrA, arrB, maxDelay, realsum);
    printheader(vars, limit, poss, arrSize, maxDelay, realsumwoffset, fp);
    printesp(esp, realsum, realsumwoffset, arrSize, fp);
    fprintf(fp,"\n.e\n");
    free(rules);
}

void printheader(char **vars, int limit, int *poss, int arrSize, int delay, int rs, FILE* fp) {
    fprintf(fp,".lib");
    int i, j, k, stren;
    char* str;
    for (i = limit - 1; i >= 0; i--) {
        stren = poss[i];
        str = vars[i];
        for (j = 0; j < stren; j++) {
            for (k = 0; k < delay + 1; k++) {
                if (k == 0) {
                    fprintf(fp," %s_%d", str, j);
                }
                else {
                    fprintf(fp," %s_%d_%d", str, j, k);
                }
            }
        }
    }
    fprintf(fp,"\n.i %d\n", rs);
    fprintf(fp,".o %d\n", poss[limit]);
    fprintf(fp,".ob ");
    for (i = 0; i < poss[limit]; i++) {
        fprintf(fp," %s_%d", vars[limit], i);
    }
    fprintf(fp,"\n.p %d", arrSize);
}

void printPreRules(char ***rules, int num, int maxDelay, int arrSize, int *arrB, int *arrA, char **vars, int limit, int *poss) {
    int i, j, k;
    int track, del;
    int mod, len;
    char* str;
    char format[200];
    for (j = 0; j < arrSize; j++) {
        track = j;
        del = arrB[j];
        strcat(rules[arrA[j]][0], "(");
        for (i = limit - 1; i >= 1; i--) {
            for (k = 0; k < del + 1; k++) {
                mod = track % poss[i];
                str = vars[i];
                if (k != 0) {
                    sprintf(format, "%s_%d_del%d  and  ", str, mod, k);
                }
                else {
                    sprintf(format, "%s_%d  and  ", str, mod, k);
                }
                strcat(rules[arrA[j]][0], format);
                track = track/poss[i];
            }
        }
        mod = track % poss[0];
        str = vars[0];
        sprintf(format, "%s_%d)", str, mod);
        strcat(rules[arrA[j]][0], format);
        track = track/poss[0];

        strcat(rules[arrA[j]][0], " or\n");
    }
    for (k = 0; k < num; k++) {
        len = strlen(rules[k][0]);
        strcpy((rules[k][0] + len - 4), "\0");
    }
}

void printesp(int **esp, int realsum, int realsumwoffset, int arrSize, FILE* fp) {
    int i, j;
    for (i = 0; i < arrSize; i++) {
        for (j = 0; j < realsum; j++) {
            if (j == realsumwoffset) {
                fprintf(fp,"    ");
            }
            else if (j == 0) {
                fprintf(fp,"\n");
            }
            fprintf(fp,"%d", esp[i][j]);
        }
    }
}

int** truthTables(int sum, int arrSize, int limit, int *poss, int *arrA, int *arrB, int maxDelay, int realsum) {
    int **A = calloc(sizeof(int *), arrSize);
    int i, j, k, mod;
    int offset, track, index, strength;
    //int realsum = (sum - poss[limit]) * (maxDelay + 1) + poss[limit];
    for (i = 0; i < arrSize; i++) {
        A[i] = calloc(sizeof(int), realsum);
    }
    for (j = 0; j < arrSize; j++) {
        offset = 0;
        track = j;
        int del = arrB[j];
        //fprintf(fp,"index for j = %d\n", j);
        for (i = limit - 1; i >= 0; i--) {
            for (k = 0; k < del + 1; k++) {
                mod = track % poss[i];
                //fprintf(fp,"mod = %d, poss[i] = %d\n", mod, poss[i]);
                index = offset + mod*(maxDelay + 1) + k;
                //fprintf(fp,"%d\n", index);
                A[j][index] = 1;
                track = track/poss[i];
            }
            offset += poss[i] * (maxDelay + 1);
        }
        strength = arrA[j];
        A[j][offset + strength] = 1;
    }
    return A;
}


int sumPoss(int *poss, int molNums) {
    int i;
    int sum = 0;
    for (i = 0; i < molNums; i++) {
        sum += poss[i];
    }
    return sum;
}

void printRules(char ***rules, int num, char* str, int systemMax) {
    int i, j;
    char *finals = malloc(sizeof(char) * 10000);
    for (i = 0; i < num; i++) {
        printf("%s_%d = %s_%d_final\n", str, i, str, i);
    }
    for (i = 0; i < num; i++) {
        printf("%s", str);
        printf("_%d_tmp = \n", i);
        printf("%s\n", rules[i][0]);
    }
    for (i = 0; i < num; i++) {
        for (j = 1; j < systemMax + 1; j++) {
            if (j != 1) {
                printf("%s_%d_del%d = %s_%d_del%d\n", str, i, j, str, i, j - 1);
            }
            else {
                printf("%s_%d_del%d = %s_%d\n", str, i, j, str, i);
            }
        }
//        free(rules[i]);
    }
    for (i = 0; i < num; i++) {
        printf("%s_%d_final =\n(", str, i);
        for (j = 0; j < num; j++) {
            printf("~%s_%d_tmp & ", str, j);
        }
        printf("%s)", str);
        printf(" & %s_%d_tmp\n\n", str, i);
    }
}

int maxDelay(int* arrB, int arrSize) {
    int i;
    int max = 0;
    for (i = 0; i < arrSize; i++) {
        if (arrB[i] > max) {
            max = arrB[i];
        }
    }
    return max;
}
