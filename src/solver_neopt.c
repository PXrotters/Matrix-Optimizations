/*
 * Tema 2 ASC
 * 2025 Spring
 */
#include "utils.h"

/*
 * Add your unoptimized implementation here
 */
double* my_solver(int N, double *A, double *B, double *x_in) {
	// Alocam matricile si vectorii necesari
    double *C = calloc(N * N, sizeof *C);  // C = B * Aᵀ
    double *D = calloc(N * N, sizeof *D);  // D = Cᵀ * A
    double *x = malloc(N * sizeof *x);  // x curent
    double *y = malloc(N * sizeof *y);  // y intermediar
    double *result = malloc( N       * sizeof *result); // y final
    if (!C || !D || !x || !y || !result) {
        perror("malloc");
        exit(1);
    }

    // Copiem vectorul initial
    for (int i = 0; i < N; i++)
        x[i] = x_in[i];

    // Calculam C = B * Aᵀ prin cele 3 foruri
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                // elementul (i,j) din B*Aᵀ: B[i][k] * Aᵀ[k][j]  = B[i][k] * A[j][k]
                sum += B[i*N + k] * A[j*N + k];
            }
            C[i*N + j] = sum;
        }
    }

    // Calculam D = Cᵀ * A prin cele 3 foruri
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                // D[i][j] = Cᵀ[i][k] * A[k][j] = C[k][i] * A[k][j]
                sum += C[k*N + i] * A[k*N + j];
            }
            D[i*N + j] = sum;
        }
    }

    // Repetam de N ori: y = Cᵀ * x;   x = C * y;
    for (int iter = 0; iter < N; iter++) {
        // y = Cᵀ * x
        for (int i = 0; i < N; i++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                // Cᵀ[i][k] = C[k][i]
                sum += C[k*N + i] * x[k];
            }
            y[i] = sum;
        }
        // x = C * y
        for (int i = 0; i < N; i++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += C[i*N + k] * y[k];
            }
            x[i] = sum;
        }
    }

    // Produsul final y = D * x
    for (int i = 0; i < N; i++) {
        double sum = 0.0;
        for (int k = 0; k < N; k++) {
            sum += D[i*N + k] * x[k];
        }
        result[i] = sum;
    }

    // Eliberam memoria alocata
    free(C);
    free(D);
    free(x);
    free(y);

    return result;
}
