#ifndef LIBPYMATHMODULES_DOUBLEFUNCTIONS_H
#define LIBPYMATHMODULES_DOUBLEFUNCTIONS_H

#include <libpymath/src/internal.h>

void doubleMatrixAddMatrix(const double *a, const double *b, double *c, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, const long int rowStrideB, const long int colStrideB, int threads) {
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

#		pragma omp parallel for private(i, j) shared(a, b, c, rows, cols, rowStrideA, colStrideA, rowStrideB, colStrideB) default(none)
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

void doubleMatrixSubMatrix(const double *a, const double *b, double *c, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, const long int rowStrideB, const long int colStrideB, int threads) {
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

#		pragma omp parallel for private(i, j) shared(a, b, c, rows, cols, rowStrideA, colStrideA, rowStrideB, colStrideB) default(none)
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

void doubleMatrixMulMatrix(const double *a, const double *b, double *c, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, const long int rowStrideB, const long int colStrideB, int threads) {
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

#		pragma omp parallel for private(i, j) shared(a, b, c, rows, cols, rowStrideA, colStrideA, rowStrideB, colStrideB) default(none)
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

void doubleMatrixDivMatrix(const double *a, const double *b, double *c, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, const long int rowStrideB, const long int colStrideB, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
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
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, b, c, rows, cols, rowStrideA, colStrideA, rowStrideB, colStrideB) default(none)
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

void doubleMatrixAddScalar(const double *a, const double b, double *c, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] + b;
                c[j + i * cols + 1] = a[internalGet(i, j, rowStrideA, colStrideA) + 1] + b;
                c[j + i * cols + 2] = a[internalGet(i, j, rowStrideA, colStrideA) + 2] + b;
                c[j + i * cols + 3] = a[internalGet(i, j, rowStrideA, colStrideA) + 3] + b;
            }

            for (; j < cols; j++) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] + b;
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, b, c, rows, cols, rowStrideA, colStrideA) default(none)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] + b;
                c[j + i * cols + 1] = a[internalGet(i, j, rowStrideA, colStrideA) + 1] + b;
                c[j + i * cols + 2] = a[internalGet(i, j, rowStrideA, colStrideA) + 2] + b;
                c[j + i * cols + 3] = a[internalGet(i, j, rowStrideA, colStrideA) + 3] + b;
            }

            for (; j < cols; j++) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] + b;
            }
        }
    }
}

void doubleMatrixSubScalar(const double *a, const double b, double *c, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] - b;
                c[j + i * cols + 1] = a[internalGet(i, j, rowStrideA, colStrideA) + 1] - b;
                c[j + i * cols + 2] = a[internalGet(i, j, rowStrideA, colStrideA) + 2] - b;
                c[j + i * cols + 3] = a[internalGet(i, j, rowStrideA, colStrideA) + 3] - b;
            }

            for (; j < cols; j++) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] - b;
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, b, c, rows, cols, rowStrideA, colStrideA) default(none)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] - b;
                c[j + i * cols + 1] = a[internalGet(i, j, rowStrideA, colStrideA) + 1] - b;
                c[j + i * cols + 2] = a[internalGet(i, j, rowStrideA, colStrideA) + 2] - b;
                c[j + i * cols + 3] = a[internalGet(i, j, rowStrideA, colStrideA) + 3] - b;
            }

            for (; j < cols; j++) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] - b;
            }
        }
    }
}

void doubleMatrixMulScalar(const double *a, const double b, double *c, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] * b;
                c[j + i * cols + 1] = a[internalGet(i, j, rowStrideA, colStrideA) + 1] * b;
                c[j + i * cols + 2] = a[internalGet(i, j, rowStrideA, colStrideA) + 2] * b;
                c[j + i * cols + 3] = a[internalGet(i, j, rowStrideA, colStrideA) + 3] * b;
            }

            for (; j < cols; j++) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] * b;
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, b, c, rows, cols, rowStrideA, colStrideA) default(none)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] * b;
                c[j + i * cols + 1] = a[internalGet(i, j, rowStrideA, colStrideA) + 1] * b;
                c[j + i * cols + 2] = a[internalGet(i, j, rowStrideA, colStrideA) + 2] * b;
                c[j + i * cols + 3] = a[internalGet(i, j, rowStrideA, colStrideA) + 3] * b;
            }

            for (; j < cols; j++) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] * b;
            }
        }
    }
}

void doubleMatrixDivScalar(const double *a, const double b, double *c, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] / b;
                c[j + i * cols + 1] = a[internalGet(i, j, rowStrideA, colStrideA) + 1] / b;
                c[j + i * cols + 2] = a[internalGet(i, j, rowStrideA, colStrideA) + 2] / b;
                c[j + i * cols + 3] = a[internalGet(i, j, rowStrideA, colStrideA) + 3] / b;
            }

            for (; j < cols; j++) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] / b;
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, b, c, rows, cols, rowStrideA, colStrideA) default(none)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] / b;
                c[j + i * cols + 1] = a[internalGet(i, j, rowStrideA, colStrideA) + 1] / b;
                c[j + i * cols + 2] = a[internalGet(i, j, rowStrideA, colStrideA) + 2] / b;
                c[j + i * cols + 3] = a[internalGet(i, j, rowStrideA, colStrideA) + 3] / b;
            }

            for (; j < cols; j++) {
                c[j + i * cols] = a[internalGet(i, j, rowStrideA, colStrideA)] / b;
            }
        }
    }
}

void doubleMatrixFillScalar(double *a, const double scalar, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = scalar;
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = scalar;
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = scalar;
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = scalar;
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = scalar;
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, scalar, rows, cols, rowStrideA, colStrideA) default(none)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = scalar;
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = scalar;
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = scalar;
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = scalar;
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = scalar;
            }
        }
    }
}

void doubleMatrixFillAscending(double *a, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                const double fill = (double) (j + i * cols);
                a[internalGet(i, j, rowStrideA, colStrideA)] = fill;
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = fill + 1;
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = fill + 2;
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = fill + 3;
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = (double) (j + i * cols);
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, rows, cols, rowStrideA, colStrideA) default(none)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                const double fill = (double) (j + i * cols);
                a[internalGet(i, j, rowStrideA, colStrideA)] = fill;
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = fill + 1;
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = fill + 2;
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = fill + 3;
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = (double) (j + i * cols);
            }
        }
    }
}

void doubleMatrixFillDescending(double *a, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, int threads) {
    const double max = (double) (rows * cols) - 1;

    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                const double fill = (double) (j + i * cols);
                a[internalGet(i, j, rowStrideA, colStrideA)] = max - fill;
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = max - (fill + 1);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = max - (fill + 2);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = max - (fill + 3);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = (double) (j + i * cols);
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, rows, cols, rowStrideA, colStrideA) default(none)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                const double fill = (double) (j + i * cols);
                a[internalGet(i, j, rowStrideA, colStrideA)] = max - fill;
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = max - (fill + 1);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = max - (fill + 2);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = max - (fill + 3);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = (double) (j + i * cols);
            }
        }
    }
}

void doubleMatrixFillRandomRange(double *a, const double min, const double max, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                const double fill = (double) (j + i * cols);
                a[internalGet(i, j, rowStrideA, colStrideA)] = randomRange(min, max);
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = randomRange(min, max);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = randomRange(min, max);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = randomRange(min, max);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = randomRange(min, max);
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, min, max, rows, cols, rowStrideA, colStrideA) default(none)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                const double fill = (double) (j + i * cols);
                a[internalGet(i, j, rowStrideA, colStrideA)] = randomRange(min, max);
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = randomRange(min, max);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = randomRange(min, max);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = randomRange(min, max);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = randomRange(min, max);
            }
        }
    }
}

#define SIGMOID(x) (1 / (1 + exp((-(x)))))
#define TANH(x) (tanh((x)))
#define RELU(x) ((x) > 0 ? (x) : 0)
#define LEAKY_RELU(x) ((x) > 0 ? (x) : ((x) * 0.2))

#define D_SIGMOID(x) ((x) * (1 - (x)))
#define D_TANH(x) (1 / (cosh((x)) * cosh((x))))
#define D_RELU(x) ((x) > 0 ? 1 : 0)
#define D_LEAKY_RELU(x) ((x) > 0 ? 1 : 0.2)

void doubleMatrixMapSigmoid(double *a, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA)]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA) + 1]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA) + 2]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA) + 3]);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA)]);
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, rows, cols, rowStrideA, colStrideA) default(none)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA)]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA) + 1]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA) + 2]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA) + 3]);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA)]);
            }
        }
    }
}

void doubleMatrixMapTanh(double *a, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = TANH(a[internalGet(i, j, rowStrideA, colStrideA)]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = TANH(a[internalGet(i, j, rowStrideA, colStrideA) + 1]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = TANH(a[internalGet(i, j, rowStrideA, colStrideA) + 2]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = TANH(a[internalGet(i, j, rowStrideA, colStrideA) + 3]);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = TANH(a[internalGet(i, j, rowStrideA, colStrideA)]);
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, rows, cols, rowStrideA, colStrideA) default(none)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = TANH(a[internalGet(i, j, rowStrideA, colStrideA)]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = TANH(a[internalGet(i, j, rowStrideA, colStrideA) + 1]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = TANH(a[internalGet(i, j, rowStrideA, colStrideA) + 2]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = TANH(a[internalGet(i, j, rowStrideA, colStrideA) + 3]);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = TANH(a[internalGet(i, j, rowStrideA, colStrideA)]);
            }
        }
    }
}

void doubleMatrixMapRELU(double *a, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = RELU(a[internalGet(i, j, rowStrideA, colStrideA)]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 1]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 2]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 3]);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = RELU(a[internalGet(i, j, rowStrideA, colStrideA)]);
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, rows, cols, rowStrideA, colStrideA) default(none)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = RELU(a[internalGet(i, j, rowStrideA, colStrideA)]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 1]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 2]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 3]);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = RELU(a[internalGet(i, j, rowStrideA, colStrideA)]);
            }
        }
    }
}

void doubleMatrixMapLeakyRELU(double *a, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA)]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 1]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 2]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 3]);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA)]);
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, rows, cols, rowStrideA, colStrideA) default(none)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA)]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 1]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 2]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 3]);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA)]);
            }
        }
    }
}

void doubleMatrixMapSigmoidDerivative(double *a, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = D_SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA)]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = D_SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA) + 1]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = D_SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA) + 2]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = D_SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA) + 3]);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = D_SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA)]);
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, rows, cols, rowStrideA, colStrideA) default(none)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = D_SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA)]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = D_SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA) + 1]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = D_SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA) + 2]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = D_SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA) + 3]);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = D_SIGMOID(a[internalGet(i, j, rowStrideA, colStrideA)]);
            }
        }
    }
}

void doubleMatrixMapTanhDerivative(double *a, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = D_TANH(a[internalGet(i, j, rowStrideA, colStrideA)]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = D_TANH(a[internalGet(i, j, rowStrideA, colStrideA) + 1]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = D_TANH(a[internalGet(i, j, rowStrideA, colStrideA) + 2]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = D_TANH(a[internalGet(i, j, rowStrideA, colStrideA) + 3]);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = D_TANH(a[internalGet(i, j, rowStrideA, colStrideA)]);
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, rows, cols, rowStrideA, colStrideA) default(none)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = D_TANH(a[internalGet(i, j, rowStrideA, colStrideA)]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = D_TANH(a[internalGet(i, j, rowStrideA, colStrideA) + 1]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = D_TANH(a[internalGet(i, j, rowStrideA, colStrideA) + 2]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = D_TANH(a[internalGet(i, j, rowStrideA, colStrideA) + 3]);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = D_TANH(a[internalGet(i, j, rowStrideA, colStrideA)]);
            }
        }
    }
}

void doubleMatrixMapRELUDerivative(double *a, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = D_RELU(a[internalGet(i, j, rowStrideA, colStrideA)]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = D_RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 1]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = D_RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 2]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = D_RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 3]);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = D_RELU(a[internalGet(i, j, rowStrideA, colStrideA)]);
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, rows, cols, rowStrideA, colStrideA) default(none)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = D_RELU(a[internalGet(i, j, rowStrideA, colStrideA)]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = D_RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 1]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = D_RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 2]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = D_RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 3]);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = D_RELU(a[internalGet(i, j, rowStrideA, colStrideA)]);
            }
        }
    }
}

void doubleMatrixMapLeakyRELUDerivative(double *a, const long int rows, long long cols, const long int rowStrideA, const long int colStrideA, int threads) {
    if (rows * cols < 90000) {
        long long i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = D_LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA)]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = D_LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 1]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = D_LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 2]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = D_LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 3]);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = D_LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA)]);
            }
        }
    } else {
        long long i, j;
#       ifdef _OPENMP
        omp_set_num_threads(threads);
#       endif

#		pragma omp parallel for private(i, j) shared(a, rows, cols, rowStrideA, colStrideA) default(none)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols - 3; j += 4) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = D_LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA)]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 1] = D_LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 1]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 2] = D_LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 2]);
                a[internalGet(i, j, rowStrideA, colStrideA) + 3] = D_LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA) + 3]);
            }

            for (; j < cols; j++) {
                a[internalGet(i, j, rowStrideA, colStrideA)] = D_LEAKY_RELU(a[internalGet(i, j, rowStrideA, colStrideA)]);
            }
        }
    }
}

#endif // LIBPYMATHMODULES_DOBLEFUNCT
