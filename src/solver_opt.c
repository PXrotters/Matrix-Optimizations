/*
 * Tema 2 ASC
 * 2025 Spring
 */
#include "utils.h"
#include <string.h>

/*
 * Add your optimized implementation here
 */

static inline int min_int(int a, int b) { return a < b ? a : b; }

double* my_solver(int N, double *A, double *B, double *x_in) {
    const int blockSize = 32;  // Dimensiune bloc pentru cache L1

    // Alocam matricile si vectorii necesari
    double *transposeA = malloc((size_t)N * N * sizeof *transposeA);  // Aᵀ
    double *C = calloc((size_t)N * N, sizeof *C);  // C = B·Aᵀ
    double *transposeC = malloc((size_t)N * N * sizeof *transposeC);  // Cᵀ
    double *D = calloc((size_t)N * N, sizeof *D);  // D = Cᵀ·A
    double *x = malloc((size_t)N * sizeof *x);
    double *y = malloc((size_t)N * sizeof *y);
    double *result = malloc((size_t)N * sizeof *result);
    if (!transposeA||!C||!transposeC||!D||!x||!y||!result) { perror("malloc"); exit(1); }

    // Copiem vectorul initial si transpunem A -> transposeA
    memcpy(x, x_in, (size_t)N * sizeof *x);
    for (int i = 0; i < N; i++) {
        double *aRow = A + (size_t)i * N;
        for (int j = 0; j < N; j++) {
            transposeA[j*(size_t)N + i] = aRow[j];
        }
    }

    // Calculam bloc C = B * transposeA folosind blocuri si loop unrolling
    for (int iBlock = 0; iBlock < N; iBlock += blockSize)
    for (int kBlock = 0; kBlock < N; kBlock += blockSize)
    for (int jBlock = 0; jBlock < N; jBlock += blockSize) {
        int i_end = min_int(iBlock + blockSize, N),
            k_end = min_int(kBlock + blockSize, N),
            j_end = min_int(jBlock + blockSize, N);
        for (int i = iBlock; i < i_end; i++) {
            double *cRow = C + (size_t)i * N + jBlock;
            double *bRow = B + (size_t)i * N + kBlock;
            for (int k = kBlock; k < k_end; k++) {
                register double bval = bRow[k - kBlock];
                double *transposeAk = transposeA + (size_t)k * N + jBlock;
                int j = 0;
                // unrolling cate 4 elemente
                for (; j + 3 < j_end - jBlock; j += 4) {
                    cRow[j+0] += bval * transposeAk[j+0];
                    cRow[j+1] += bval * transposeAk[j+1];
                    cRow[j+2] += bval * transposeAk[j+2];
                    cRow[j+3] += bval * transposeAk[j+3];
                }
				// restul elementelor
                for (; j < j_end - jBlock; j++) {
                    cRow[j] += bval * transposeAk[j];
                }
            }
        }
    }

    // Transpunem C → transposeC
    for (int i = 0; i < N; i++) {
        double *Cr = C + (size_t)i * N;
        for (int j = 0; j < N; j++) {
            transposeC[j*(size_t)N + i] = Cr[j];
        }
    }

    // Calculam bloc D = transposeC * A similar
    for (int iBlock = 0; iBlock < N; iBlock += blockSize)
    for (int kBlock = 0; kBlock < N; kBlock += blockSize)
    for (int jBlock = 0; jBlock < N; jBlock += blockSize) {
        int i_end = min_int(iBlock + blockSize, N),
            k_end = min_int(kBlock + blockSize, N),
            j_end = min_int(jBlock + blockSize, N);
        for (int i = iBlock; i < i_end; i++) {
            double *dRow = D + (size_t)i * N + jBlock;
            double *transposeCi= transposeC + (size_t)i * N + kBlock;
            for (int k = kBlock; k < k_end; k++) {
                register double cval = transposeCi[k - kBlock];
                double *Ak = A + (size_t)k * N + jBlock;
                int j = 0;
				// unrolling cate 4 elemente
                for (; j + 3 < j_end - jBlock; j += 4) {
                    dRow[j+0] += cval * Ak[j+0];
                    dRow[j+1] += cval * Ak[j+1];
                    dRow[j+2] += cval * Ak[j+2];
                    dRow[j+3] += cval * Ak[j+3];
                }
				// restul elementelor
                for (; j < j_end - jBlock; j++) {
                    dRow[j] += cval * Ak[j];
                }
            }
        }
    }

    // Iteratia principala y = Cᵀ·x, x = C·y cu unrolling
    for (int iter = 0; iter < N; iter++) {
        // y = transposeC * x
        for (int i = 0; i < N; i++) {
            double *row = transposeC + (size_t)i * N;
            register double s0 = 0, s1 = 0, s2 = 0, s3 = 0;
            int k = 0;
			// unrolling cate 4 elemente
            for (; k + 3 < N; k += 4) {
                s0 += row[k+0] * x[k+0];
                s1 += row[k+1] * x[k+1];
                s2 += row[k+2] * x[k+2];
                s3 += row[k+3] * x[k+3];
            }
			// restul elementelor
            for (; k < N; k++) s0 += row[k] * x[k];
            y[i] = s0 + s1 + s2 + s3;
        }
        // x = C * y
        for (int i = 0; i < N; i++) {
            double *row = C + (size_t)i * N;
            register double s0 = 0, s1 = 0, s2 = 0, s3 = 0;
            int k = 0;
			// unrolling cate 4 elemente
            for (; k + 3 < N; k += 4) {
                s0 += row[k+0] * y[k+0];
                s1 += row[k+1] * y[k+1];
                s2 += row[k+2] * y[k+2];
                s3 += row[k+3] * y[k+3];
            }
			// restul elementelor
            for (; k < N; k++) s0 += row[k] * y[k];
            x[i] = s0 + s1 + s2 + s3;
        }
    }

    // Rezultul final result = D * x
    for (int i = 0; i < N; i++) {
        double *row = D + (size_t)i * N;
        register double s0 = 0, s1 = 0, s2 = 0, s3 = 0;
        int k = 0;
		// unrolling cate 4 elemente
        for (; k + 3 < N; k += 4) {
            s0 += row[k+0] * x[k+0];
            s1 += row[k+1] * x[k+1];
            s2 += row[k+2] * x[k+2];
            s3 += row[k+3] * x[k+3];
        }
		// restul elementelor
        for (; k < N; k++) s0 += row[k] * x[k];
        result[i] = s0 + s1 + s2 + s3;
    }

    // Eliberam memoria
    free(transposeA);
    free(C);
    free(transposeC);
    free(D);
    free(x);
    free(y);

    return result;
}
