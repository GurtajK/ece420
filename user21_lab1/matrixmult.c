#include "lab1_IO.h"
#include "timer.h"
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#include <stdio.h>

int** A;
int** B;
int** C;
int n, k;
double m, sqrtk;

void* calculate_submatrix(void* rank) {
    int r = (int)rank;
    int irank = r % (int)sqrtk;
    int jrank = r / (int)sqrtk;
    int i_end = (irank+1)*m;
    int j_end = (jrank+1)*m;

    for (int i = irank*m; i < i_end; i++) {
        for (int j = jrank*m; j < j_end; j++) {
            for (int r = 0; r < n; r++) {
                C[i][j] += A[i][r]*B[r][j];
            }
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    // validate input
    if (argc != 2) {
        printf("Usage: ./demo <thread_count>\n");
        return -1;
    }

    k = atoi(argv[1]);
    if (k < 1) {
        printf("ERROR!\nNumber of threads must be a positive perfect square integer.\n");
        return -1;
    }

    sqrtk = pow(k, 0.5);
    if (sqrtk != (int)sqrtk) {
        printf("ERROR!\nNumber of threads must be a perfect square.\n");
        return -1;
    }

    Lab1_loadinput(&A, &B, &n);
    
    m = n/sqrtk;
    if (m != (int)m) {
        printf("ERROR!\nNumber of threads must perfectly divide n^2. (n = %d, k = %d)\n", n, k);
        return -1;
    }
    
    // allocate result matrix
    C = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
    {
      C[i] = calloc(n, sizeof(int));
    }

    pthread_t tids[k];

    double start;
    GET_TIME(start);

    for (int i = 0; i < k; i++) {
        pthread_create(&tids[i], NULL, calculate_submatrix, (void *) i);
    }
    for (int i = 0; i < k; i++) {
        pthread_join(tids[i], NULL);
    }

    double end; GET_TIME(end);
    Lab1_saveoutput(C, &n, end-start);
    printf("Thread Count: %d, Time: %f\n", k, end-start);

    for (int i = 0; i < n; i++)
    {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);
}
