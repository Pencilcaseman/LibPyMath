//
// Created by penci on 04/09/2020.
//

#ifndef LIBPYMATHMODULES_DOUBLEFUNCTIONS_H
#define LIBPYMATHMODULES_DOUBLEFUNCTIONS_H

#ifdef _OPENMP
#include <omp.h>
#endif

#define internalGet(i, j, r, c) ((j) * (c) + (i) * (r))

void doubleMatrixAddMatrix(const double *a, const double *b, double *c, long long rows, long long cols, long long rowStrideA, long long colStrideA, long long rowStrideB, long long colStrideB, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] + b[internalGet(i, j, rowStrideB, colStrideB)];
                c[j + i * cols + 1] = a[internalGet(i, j, rowStrideA, colStrideA) + 1] + b[internalGet(i, j, rowStrideB, colStrideB) + 1];
                c[j + i * cols + 2] = a[internalGet(i, j, rowStrideA, colStrideA) + 2] + b[internalGet(i, j, rowStrideB, colStrideB) + 2];
                c[j + i * cols + 3] = a[internalGet(i, j, rowStrideA, colStrideA) + 3] + b[internalGet(i, j, rowStrideB, colStrideB) + 3];
            }

            for (; j < cols; j++) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] + b[internalGet(i, j, rowStrideB, colStrideB)];
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, b, c, rows, cols, rowStrideA, colStrideA, rowStrideB, colStrideB)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] + b[internalGet(i, j, rowStrideB, colStrideB)];
                c[j + i * cols + 1] = a[internalGet(i, j, rowStrideA, colStrideA) + 1] + b[internalGet(i, j, rowStrideB, colStrideB) + 1];
                c[j + i * cols + 2] = a[internalGet(i, j, rowStrideA, colStrideA) + 2] + b[internalGet(i, j, rowStrideB, colStrideB) + 2];
                c[j + i * cols + 3] = a[internalGet(i, j, rowStrideA, colStrideA) + 3] + b[internalGet(i, j, rowStrideB, colStrideB) + 3];
            }

            for (; j < cols; j++) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] + b[internalGet(i, j, rowStrideB, colStrideB)];
            }
        }
    }
}

void doubleMatrixSubMatrix(const double *a, const double *b, double *c, long long rows, long long cols, long long rowStrideA, long long colStrideA, long long rowStrideB, long long colStrideB, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] - b[internalGet(i, j, rowStrideB, colStrideB)];
                c[j + i * cols + 1] = a[internalGet(i, j, rowStrideA, colStrideA) + 1] - b[internalGet(i, j, rowStrideB, colStrideB) + 1];
                c[j + i * cols + 2] = a[internalGet(i, j, rowStrideA, colStrideA) + 2] - b[internalGet(i, j, rowStrideB, colStrideB) + 2];
                c[j + i * cols + 3] = a[internalGet(i, j, rowStrideA, colStrideA) + 3] - b[internalGet(i, j, rowStrideB, colStrideB) + 3];
            }

            for (; j < cols; j++) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] - b[internalGet(i, j, rowStrideB, colStrideB)];
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, b, c, rows, cols, rowStrideA, colStrideA, rowStrideB, colStrideB)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] - b[internalGet(i, j, rowStrideB, colStrideB)];
                c[j + i * cols + 1] = a[internalGet(i, j, rowStrideA, colStrideA) + 1] - b[internalGet(i, j, rowStrideB, colStrideB) + 1];
                c[j + i * cols + 2] = a[internalGet(i, j, rowStrideA, colStrideA) + 2] - b[internalGet(i, j, rowStrideB, colStrideB) + 2];
                c[j + i * cols + 3] = a[internalGet(i, j, rowStrideA, colStrideA) + 3] - b[internalGet(i, j, rowStrideB, colStrideB) + 3];
            }

            for (; j < cols; j++) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] - b[internalGet(i, j, rowStrideB, colStrideB)];
            }
        }
    }
}

void doubleMatrixMulMatrix(const double *a, const double *b, double *c, long long rows, long long cols, long long rowStrideA, long long colStrideA, long long rowStrideB, long long colStrideB, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] * b[internalGet(i, j, rowStrideB, colStrideB)];
                c[j + i * cols + 1] = a[internalGet(i, j, rowStrideA, colStrideA) + 1] * b[internalGet(i, j, rowStrideB, colStrideB) + 1];
                c[j + i * cols + 2] = a[internalGet(i, j, rowStrideA, colStrideA) + 2] * b[internalGet(i, j, rowStrideB, colStrideB) + 2];
                c[j + i * cols + 3] = a[internalGet(i, j, rowStrideA, colStrideA) + 3] * b[internalGet(i, j, rowStrideB, colStrideB) + 3];
            }

            for (; j < cols; j++) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] * b[internalGet(i, j, rowStrideB, colStrideB)];
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, b, c, rows, cols, rowStrideA, colStrideA, rowStrideB, colStrideB)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] * b[internalGet(i, j, rowStrideB, colStrideB)];
                c[j + i * cols + 1] = a[internalGet(i, j, rowStrideA, colStrideA) + 1] * b[internalGet(i, j, rowStrideB, colStrideB) + 1];
                c[j + i * cols + 2] = a[internalGet(i, j, rowStrideA, colStrideA) + 2] * b[internalGet(i, j, rowStrideB, colStrideB) + 2];
                c[j + i * cols + 3] = a[internalGet(i, j, rowStrideA, colStrideA) + 3] * b[internalGet(i, j, rowStrideB, colStrideB) + 3];
            }

            for (; j < cols; j++) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] * b[internalGet(i, j, rowStrideB, colStrideB)];
            }
        }
    }
}

void doubleMatrixDivMatrix(const double *a, const double *b, double *c, long long rows, long long cols, long long rowStrideA, long long colStrideA, long long rowStrideB, long long colStrideB, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] / b[internalGet(i, j, rowStrideB, colStrideB)];
                c[j + i * cols + 1] = a[internalGet(i, j, rowStrideA, colStrideA) / 1] + b[internalGet(i, j, rowStrideB, colStrideB) + 1];
                c[j + i * cols + 2] = a[internalGet(i, j, rowStrideA, colStrideA) / 2] + b[internalGet(i, j, rowStrideB, colStrideB) + 2];
                c[j + i * cols + 3] = a[internalGet(i, j, rowStrideA, colStrideA) / 3] + b[internalGet(i, j, rowStrideB, colStrideB) + 3];
            }

            for (; j < cols; j++) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] / b[internalGet(i, j, rowStrideB, colStrideB)];
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, b, c, rows, cols, rowStrideA, colStrideA, rowStrideB, colStrideB)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] / b[internalGet(i, j, rowStrideB, colStrideB)];
                c[j + i * cols + 1] = a[internalGet(i, j, rowStrideA, colStrideA) + 1] / b[internalGet(i, j, rowStrideB, colStrideB) + 1];
                c[j + i * cols + 2] = a[internalGet(i, j, rowStrideA, colStrideA) + 2] / b[internalGet(i, j, rowStrideB, colStrideB) + 2];
                c[j + i * cols + 3] = a[internalGet(i, j, rowStrideA, colStrideA) + 3] / b[internalGet(i, j, rowStrideB, colStrideB) + 3];
            }

            for (; j < cols; j++) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] / b[internalGet(i, j, rowStrideB, colStrideB)];
            }
        }
    }
}

#endif // LIBPYMATHMODULES_DOBLEFUNCT
