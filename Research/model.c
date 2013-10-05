/*struct stren {
    int strength;
    int delay;
};*/
int n = 4;
int* vars = malloc(sizeof(char) * n);
int* poss = malloc(sizeof(int) * n);
vars[0] = "IGF1RA";
vars[1] = "MEK";
vars[2] = "TRA";
vars[3] = "TGFBRA";
poss[0] = 2;
poss[1] = 2;
poss[2] = 2;
poss[3] = 2;

static int arrA[16] = {0, 1, 1, 2, 1, 2, 2, 2, 0, 0, 0, 1, 0, 1, 1, 2}//16 = product over poss
static int arrB[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}//holds delays
int main() {
    int maxDelay = maxDelay(poss, n);
    int molNum = n;
    model(arrA, arrB, maxDelay, vars, poss, molNum);
    //molecule strength array, delay array, maximum delay given, molecule variable names, possible strengths, number of vars
}

int model(int *arrA, int *arrB, int maxDelay, int *vars, int * poss, int molNums) {
    int numDelays = maxDelay;
    char* rules = malloc(sizeof(char) * 3);
    rules[0] = "";
    rules[1] = "";
    rules[2] = "";

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 16; j++) {
            if (arrA[j] = i) {
                

int maxDelay(int* poss, int n) {
    int max = 0;
    for (int i = 0; i < n; i++) {
        if (poss[i] > max) {
            max = poss[i];
        }
    }
    return max;
}
