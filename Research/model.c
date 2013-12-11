#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "test_cases.h"
#include "parsing.c"

/*struct stren {
    int strength;
    int delay;
};*/
int model(int *arrA, int *arrB, int arrSize, int maxDelay, char **vars, int *poss, int molNums, int systemMax);
int maxDelay(int* arrB, int arrSize);
void printRules(char ***rules, int num, char* str, int systemMax);

int n = 5;

int main() {
    int i;
    struct table* test = p();
    int MD, molNum;
    tables* t = tests();

    printf("/////////////////////\n");
    printf("test number 0\n");
    printf("/////////////////////\n");
    model(test -> arrA, test -> arrB, test -> arrSize, maxDelay(test -> arrB, test -> arrSize), test -> vars, test -> poss, test -> n, 3);

    for (i = 0; i < 3; i++) {
        char** vars = t[i] -> vars;
        int* poss = t[i] -> poss;
        int* arrA = t[i] -> arrA;
        int* arrB = t[i] -> arrB;
        int n = t[i] -> n;
        int arrSize = t[i] -> arrSize;
        MD = maxDelay(arrB, arrSize);
        molNum = n;
        printf("////////////////////////////////////////////////////\n");
        printf("test number %d, molecule %s, %d inputs\n", i, vars[n - 1], n - 1);
        printf("////////////////////////////////////////////////////\n");
        model(arrA, arrB, arrSize, MD, vars, poss, molNum, 3);
    }
    //molecule strength array, delay array, maximum delay given, molecule variable names, possible strengths, number of vars
}

int model(int *arrA, int *arrB, int arrSize, int maxDelay, char **vars, int *poss, int molNums, int systemMax) {
    //p();
    int i, j, k;
    int len;
    int numDelays = maxDelay;
    int limit = molNums - 1;
    int mod;
    char* str;
    char format[200];
    int track;
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

    for (j = 0; j < arrSize; j++) {
        track = j;
        int del = arrB[j];
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
    printRules(rules, num, vars[limit], systemMax);
    free(rules);
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
