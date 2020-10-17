#include <libpymath/src/internal.h>
#include "ulmblas.h"
#include <stdio.h>

//
//  Declaration for the BLAS error message function used and required by
//  BLAS level 2 and level 3 functions.
//
void
ULMBLAS(xerbla)(const char rout[6], const long info) {
    // fprintf(stderr, "Parameter %li to routine %s was incorrect\n", info, rout);

    switch (info) {
        case 1: pythonPrint("Parameter 1 to routine DGEMM was incorrect\n");
        case 2: pythonPrint("Parameter 2 to routine DGEMM was incorrect\n");
        case 3: pythonPrint("Parameter 3 to routine DGEMM was incorrect\n");
        case 4: pythonPrint("Parameter 4 to routine DGEMM was incorrect\n");
        case 5: pythonPrint("Parameter 5 to routine DGEMM was incorrect\n");
        case 8: pythonPrint("Parameter 8 to routine DGEMM was incorrect\n");
        case 10: pythonPrint("Parameter 10 to routine DGEMM was incorrect\n");
        case 13: pythonPrint("Parameter 13 to routine DGEMM was incorrect\n");
        default: pythonPrint("Unknown DGEMM error\n");
    }
}

void
F77BLAS(xerbla)(const char rout[6], const long *info) {
    ULMBLAS(xerbla)(rout, *info);
}
