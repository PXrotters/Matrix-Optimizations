/*
 * Tema 2 ASC
 * 2024 Spring
 */
#include "utils.h"
#include <cblas.h>

/* 
 * Add your BLAS implementation here
 */
double* my_solver(int N, double *A, double *B, double *x) {
    // Alocam matricile si vectorii necesari
    double *C = calloc((size_t)N * N, sizeof *C);
    double *D = calloc((size_t)N * N, sizeof *D);
    double *y = malloc((size_t)N * sizeof *y);
    double *result = malloc((size_t)N * sizeof *result);
    if (!C || !D || !y || !result) {
        perror("malloc");
        exit(1);
    }

    // Calculam C = B * Aᵀ
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, N, N, N, 1.0, B, N, A, N, 0.0, C, N);

    // Calculam D = Cᵀ * A
    cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans, N, N, N, 1.0, C, N, A, N, 0.0, D, N);

    // Relaxare iterativa: x ← C·(Cᵀ·x)
    for (int iter = 0; iter < N; iter++) {
        // y = Cᵀ * x
        cblas_dgemv(CblasRowMajor, CblasTrans, N, N, 1.0, C, N, x, 1, 0.0, y, 1);
        // x = C * y
        cblas_dgemv(CblasRowMajor, CblasNoTrans, N, N, 1.0, C, N, y, 1, 0.0, x, 1);
    }

    // Calcul final: result = D * x
    cblas_dgemv(CblasRowMajor, CblasNoTrans, N, N, 1.0, D, N, x, 1, 0.0, result, 1);

    // Eliberam memoria
    free(C);
    free(D);
    free(y);

    return result;
}
