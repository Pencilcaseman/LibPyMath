#include "ulmblas.h"
#include <stdio.h>

//
//  Declaration for the BLAS error message function used and required by
//  BLAS level 2 and level 3 functions.
//
void
ULMBLAS(xerbla)(const char rout[6], const long info) {
    fprintf(stderr, "Parameter %li to routine %s was incorrect\n", info, rout);
}

void
F77BLAS(xerbla)(const char rout[6], const long *info) {
    ULMBLAS(xerbla)(rout, *info);
}
