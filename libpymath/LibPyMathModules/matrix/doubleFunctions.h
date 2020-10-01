/*
Copyright 2020 Toby Davis

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LIBPYMATHMODULES_DOUBLEFUNCTIONS_H
#define LIBPYMATHMODULES_DOUBLEFUNCTIONS_H

#include <libpymath/LibPyMathModules/internal.h>

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

void doubleMatrixDivMatrix(const double *a, const double *b, double *c, long long rows, long long cols, long long rowStrideA, long long colStrideA, long long rowStrideB, long long colStrideB, int threads) {
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

void doubleMatrixAddScalar(const double *a, const double b, double *c, long long rows, long long cols, long long rowStrideA, long long colStrideA, int threads) {
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

void doubleMatrixSubScalar(const double *a, const double b, double *c, long long rows, long long cols, long long rowStrideA, long long colStrideA, int threads) {
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

void doubleMatrixMulScalar(const double *a, const double b, double *c, long long rows, long long cols, long long rowStrideA, long long colStrideA, int threads) {
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

void doubleMatrixDivScalar(const double *a, const double b, double *c, long long rows, long long cols, long long rowStrideA, long long colStrideA, int threads) {
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

#endif // LIBPYMATHMODULES_DOBLEFUNCT
