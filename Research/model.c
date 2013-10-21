#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "test_cases.h"
/*struct stren {
    int strength;
    int delay;
};*/
int maxDelay(int* poss, int n, char** vars);
void printRules(char **rules, int num, char* str);

int n = 5;

int main() {
    int i;
    int MD, molNum;
    tables* t = tests();

    for (i = 0; i < 3; i++) {
        char** vars = t[i] -> vars;
        int* poss = t[i] -> poss;
        int* arrA = t[i] -> arrA;
        int* arrB = t[i] -> arrB;
        int n = t[i] -> n;
        int arrSize = t[i] -> arrSize;
        MD = maxDelay(poss, n, vars);
        molNum = n;
        printf("////////////////////////////////////////////////////\n");
        printf("test number %d, molecule %s, %d inputs\n", i, vars[n - 1], n - 1);
        printf("////////////////////////////////////////////////////\n");
        model(arrA, arrB, arrSize, MD, vars, poss, molNum);
    }
    //molecule strength array, delay array, maximum delay given, molecule variable names, possible strengths, number of vars
}

int model(int *arrA, int *arrB, int arrSize, int maxDelay, char **vars, int *poss, int molNums) {
    int i, j, k;
    int len;
    int numDelays = maxDelay;
    int limit = molNums - 1;
    int mod;
    char* str;
    char format[200];
    int track;
    int num = poss[limit];
    char** rules = malloc(sizeof(char*) * num); //strings of rules
    for (i = 0; i < num; i++) {
        rules[i] = malloc(sizeof(char) * 10000);
        strcpy(rules[i], "");
    }

    for (j = 0; j < arrSize; j++) {
        track = j;
        strcat(rules[arrA[j]], "(");
        for (i = limit - 1; i >= 1; i--) {
            mod = track % poss[i];
            str = vars[i];
            sprintf(format, "%s_%d  and  ", str, mod);
            strcat(rules[arrA[j]], format);
            track = track/poss[i];
        }
        mod = track % poss[0];
        str = vars[0];
        sprintf(format, "%s_%d)", str, mod);
        strcat(rules[arrA[j]], format);
        track = track/poss[0];

        strcat(rules[arrA[j]], " or\n");
    }
    for (k = 0; k < num; k++) {
        len = strlen(rules[k]);
        strcpy((rules[k] + len - 4), "\0");
    }
    printRules(rules, num, vars[limit]);
    free(rules);
}

void printRules(char **rules, int num, char* str) {
    int i;
    for (i = 0; i < num; i++) {
        printf("%s", str);
        printf("_%d:\n", i);
        printf("%s\n", rules[i]);
        free(rules[i]);
    }
}

int maxDelay(int* poss, int n, char** vars) {
    int i;
    int max = 0;
    for (i = 0; i < n; i++) {
        if (poss[i] > max && strcmp(vars[i], "ERK_ACT")) {
            max = poss[i];
        }
    }
    return max;
}
