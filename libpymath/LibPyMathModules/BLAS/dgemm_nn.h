#include "ulmblas.h"
#include <stdio.h>
#include <emmintrin.h>
#include <immintrin.h>

#ifndef _MSC_VER

#define MC  384
#define KC  384
#define NC  4096

#define MR  4
#define NR  4

//
//  Local buffers for storing panels from A, B and C
//

static double A_BUFFER[MC * KC] __attribute__ ((aligned (16)));
static double B_BUFFER[KC * NC] __attribute__ ((aligned (16)));
static double C_BUFFER[MR * NR] __attribute__ ((aligned (16)));

//
//  Packing complete panels from A (i.e. without padding)
//
static void
pack_MRxk(long k, const double *A, long incRowA, long incColA,
          double *buffer) {
    long i, j;

    for (j = 0; j < k; ++j) {
        for (i = 0; i < MR; ++i) {
            buffer[i] = A[i * incRowA];
        }
        buffer += MR;
        A += incColA;
    }
}

//
//  Packing panels from A with padding if required
//
static void
pack_A(long mc, long kc, const double *A, long incRowA, long incColA,
       double *buffer) {
    long mp = mc / MR;
    long _mr = mc % MR;

    long i, j;

    for (i = 0; i < mp; ++i) {
        pack_MRxk(kc, A, incRowA, incColA, buffer);
        buffer += kc * MR;
        A += MR * incRowA;
    }
    if (_mr > 0) {
        for (j = 0; j < kc; ++j) {
            for (i = 0; i < _mr; ++i) {
                buffer[i] = A[i * incRowA];
            }
            for (i = _mr; i < MR; ++i) {
                buffer[i] = 0.0;
            }
            buffer += MR;
            A += incColA;
        }
    }
}

//
//  Packing complete panels from B (i.e. without padding)
//
static void
pack_kxNR(long k, const double *B, long incRowB, long incColB,
          double *buffer) {
    long i, j;

    for (i = 0; i < k; ++i) {
        for (j = 0; j < NR; ++j) {
            buffer[j] = B[j * incColB];
        }
        buffer += NR;
        B += incRowB;
    }
}

//
//  Packing panels from B with padding if required
//
static void
pack_B(long kc, long nc, const double *B, long incRowB, long incColB,
       double *buffer) {
    long np = nc / NR;
    long _nr = nc % NR;

    long i, j;

    for (j = 0; j < np; ++j) {
        pack_kxNR(kc, B, incRowB, incColB, buffer);
        buffer += kc * NR;
        B += NR * incColB;
    }
    if (_nr > 0) {
        for (i = 0; i < kc; ++i) {
            for (j = 0; j < _nr; ++j) {
                buffer[j] = B[j * incColB];
            }
            for (j = _nr; j < NR; ++j) {
                buffer[j] = 0.0;
            }
            buffer += NR;
            B += incRowB;
        }
    }
}

//
//  Micro kernel for multiplying panels from A and B.
//
static void
dgemm_micro_kernel(long kc,
                   double alpha, const double *A, const double *B,
                   double beta,
                   double *C, long incRowC, long incColC) {
    long kb = kc / 4;
    long kl = kc % 4;

//
//  Compute AB = A*B
//
    __asm__ volatile
    (
    "movq      %0,      %%rsi    \n\t"  // kb (32 bit) stored in %rsi
    "movq      %1,      %%rdi    \n\t"  // kl (32 bit) stored in %rdi
    "movq      %2,      %%rax    \n\t"  // Address of A stored in %rax
    "movq      %3,      %%rbx    \n\t"  // Address of B stored in %rbx
    "                            \n\t"
    "movaps    (%%rax), %%xmm0   \n\t"  // tmp0 = _mm_load_pd(A)
    "movaps  16(%%rax), %%xmm1   \n\t"  // tmp1 = _mm_load_pd(A+2)
    "movaps    (%%rbx), %%xmm2   \n\t"  // tmp2 = _mm_load_pd(B)
    "                            \n\t"
    "xorpd     %%xmm8,  %%xmm8   \n\t"  // ab_00_11 = _mm_setzero_pd()
    "xorpd     %%xmm9,  %%xmm9   \n\t"  // ab_20_31 = _mm_setzero_pd()
    "xorpd     %%xmm10, %%xmm10  \n\t"  // ab_01_10 = _mm_setzero_pd()
    "xorpd     %%xmm11, %%xmm11  \n\t"  // ab_21_30 = _mm_setzero_pd()
    "xorpd     %%xmm12, %%xmm12  \n\t"  // ab_02_13 = _mm_setzero_pd()
    "xorpd     %%xmm13, %%xmm13  \n\t"  // ab_22_33 = _mm_setzero_pd()
    "xorpd     %%xmm14, %%xmm14  \n\t"  // ab_03_12 = _mm_setzero_pd()
    "xorpd     %%xmm15, %%xmm15  \n\t"  // ab_23_32 = _mm_setzero_pd()
    "                            \n\t"
    "xorpd     %%xmm3,  %%xmm3   \n\t"  // tmp3 = _mm_setzero_pd
    "xorpd     %%xmm4,  %%xmm4   \n\t"  // tmp4 = _mm_setzero_pd
    "xorpd     %%xmm5,  %%xmm5   \n\t"  // tmp5 = _mm_setzero_pd
    "xorpd     %%xmm6,  %%xmm6   \n\t"  // tmp6 = _mm_setzero_pd
    "xorpd     %%xmm7,  %%xmm7   \n\t"  // tmp7 = _mm_setzero_pd
    "testq     %%rdi,   %%rdi    \n\t"  // if kl==0 writeback to AB
    "                            \n\t"
    "                            \n\t"
    "testq     %%rsi,   %%rsi    \n\t"  // if kb==0 handle remaining kl
    "je        .DCONSIDERLEFT%=  \n\t"  // update iterations
    "                            \n\t"
    ".DLOOP%=:                   \n\t"  // for l = kb,..,1 do
    "                            \n\t"
    "                            \n\t"  // 1. update
    "addpd     %%xmm3,  %%xmm12  \n\t"  // ab_02_13 = _mm_add_pd(ab_02_13, tmp3)
    "movaps  16(%%rbx), %%xmm3   \n\t"  // tmp3     = _mm_load_pd(B+2)
    "addpd     %%xmm6,  %%xmm13  \n\t"  // ab_22_33 = _mm_add_pd(ab_22_33, tmp6)
    "movaps    %%xmm2,  %%xmm6   \n\t"  // tmp6     = tmp2
    "pshufd $78,%%xmm2, %%xmm4   \n\t"  // tmp4     = _mm_shuffle_pd(tmp2, tmp2,
    "                            \n\t"  //                   _MM_SHUFFLE2(0, 1))
    "mulpd     %%xmm0,  %%xmm2   \n\t"  // tmp2     = _mm_mul_pd(tmp2, tmp0);
    "mulpd     %%xmm1,  %%xmm6   \n\t"  // tmp6     = _mm_mul_pd(tmp6, tmp1);
    "                            \n\t"
    "                            \n\t"
    "addpd     %%xmm5,  %%xmm14  \n\t"  // ab_03_12 = _mm_add_pd(ab_03_12, tmp5)
    "addpd     %%xmm7,  %%xmm15  \n\t"  // ab_23_32 = _mm_add_pd(ab_23_32, tmp7)
    "movaps    %%xmm4,  %%xmm7   \n\t"  // tmp7     = tmp4
    "mulpd     %%xmm0,  %%xmm4   \n\t"  // tmp4     = _mm_mul_pd(tmp4, tmp0)
    "mulpd     %%xmm1,  %%xmm7   \n\t"  // tmp7     = _mm_mul_pd(tmp7, tmp1)
    "                            \n\t"
    "                            \n\t"
    "addpd     %%xmm2,  %%xmm8   \n\t"  // ab_00_11 = _mm_add_pd(ab_00_11, tmp2)
    "movaps  32(%%rbx), %%xmm2   \n\t"  // tmp2     = _mm_load_pd(B+4)
    "addpd     %%xmm6,  %%xmm9   \n\t"  // ab_20_31 = _mm_add_pd(ab_20_31, tmp6)
    "movaps    %%xmm3,  %%xmm6   \n\t"  // tmp6     = tmp3
    "pshufd $78,%%xmm3, %%xmm5   \n\t"  // tmp5     = _mm_shuffle_pd(tmp3, tmp3,
    "                            \n\t"  //                   _MM_SHUFFLE2(0, 1))
    "mulpd     %%xmm0,  %%xmm3   \n\t"  // tmp3     = _mm_mul_pd(tmp3, tmp0)
    "mulpd     %%xmm1,  %%xmm6   \n\t"  // tmp6     = _mm_mul_pd(tmp6, tmp1)
    "                            \n\t"
    "                            \n\t"
    "addpd     %%xmm4,  %%xmm10  \n\t"  // ab_01_10 = _mm_add_pd(ab_01_10, tmp4)
    "addpd     %%xmm7,  %%xmm11  \n\t"  // ab_21_30 = _mm_add_pd(ab_21_30, tmp7)
    "movaps    %%xmm5,  %%xmm7   \n\t"  // tmp7     = tmp5
    "mulpd     %%xmm0,  %%xmm5   \n\t"  // tmp5     = _mm_mul_pd(tmp5, tmp0)
    "movaps  32(%%rax), %%xmm0   \n\t"  // tmp0     = _mm_load_pd(A+4)
    "mulpd     %%xmm1,  %%xmm7   \n\t"  // tmp7     = _mm_mul_pd(tmp7, tmp1)
    "movaps  48(%%rax), %%xmm1   \n\t"  // tmp1     = _mm_load_pd(A+6)
    "                            \n\t"
    "                            \n\t"
    "                            \n\t"
    "                            \n\t"  // 2. update
    "addpd     %%xmm3,  %%xmm12  \n\t"  // ab_02_13 = _mm_add_pd(ab_02_13, tmp3)
    "movaps  48(%%rbx), %%xmm3   \n\t"  // tmp3     = _mm_load_pd(B+6)
    "addpd     %%xmm6,  %%xmm13  \n\t"  // ab_22_33 = _mm_add_pd(ab_22_33, tmp6)
    "movaps    %%xmm2,  %%xmm6   \n\t"  // tmp6     = tmp2
    "pshufd $78,%%xmm2, %%xmm4   \n\t"  // tmp4     = _mm_shuffle_pd(tmp2, tmp2,
    "                            \n\t"  //                   _MM_SHUFFLE2(0, 1))
    "mulpd     %%xmm0,  %%xmm2   \n\t"  // tmp2     = _mm_mul_pd(tmp2, tmp0);
    "mulpd     %%xmm1,  %%xmm6   \n\t"  // tmp6     = _mm_mul_pd(tmp6, tmp1);
    "                            \n\t"
    "                            \n\t"
    "addpd     %%xmm5,  %%xmm14  \n\t"  // ab_03_12 = _mm_add_pd(ab_03_12, tmp5)
    "addpd     %%xmm7,  %%xmm15  \n\t"  // ab_23_32 = _mm_add_pd(ab_23_32, tmp7)
    "movaps    %%xmm4,  %%xmm7   \n\t"  // tmp7     = tmp4
    "mulpd     %%xmm0,  %%xmm4   \n\t"  // tmp4     = _mm_mul_pd(tmp4, tmp0)
    "mulpd     %%xmm1,  %%xmm7   \n\t"  // tmp7     = _mm_mul_pd(tmp7, tmp1)
    "                            \n\t"
    "                            \n\t"
    "addpd     %%xmm2,  %%xmm8   \n\t"  // ab_00_11 = _mm_add_pd(ab_00_11, tmp2)
    "movaps  64(%%rbx), %%xmm2   \n\t"  // tmp2     = _mm_load_pd(B+8)
    "addpd     %%xmm6,  %%xmm9   \n\t"  // ab_20_31 = _mm_add_pd(ab_20_31, tmp6)
    "movaps    %%xmm3,  %%xmm6   \n\t"  // tmp6     = tmp3
    "pshufd $78,%%xmm3, %%xmm5   \n\t"  // tmp5     = _mm_shuffle_pd(tmp3, tmp3,
    "                            \n\t"  //                   _MM_SHUFFLE2(0, 1))
    "mulpd     %%xmm0,  %%xmm3   \n\t"  // tmp3     = _mm_mul_pd(tmp3, tmp0)
    "mulpd     %%xmm1,  %%xmm6   \n\t"  // tmp6     = _mm_mul_pd(tmp6, tmp1)
    "                            \n\t"
    "                            \n\t"
    "addpd     %%xmm4,  %%xmm10  \n\t"  // ab_01_10 = _mm_add_pd(ab_01_10, tmp4)
    "addpd     %%xmm7,  %%xmm11  \n\t"  // ab_21_30 = _mm_add_pd(ab_21_30, tmp7)
    "movaps    %%xmm5,  %%xmm7   \n\t"  // tmp7     = tmp5
    "mulpd     %%xmm0,  %%xmm5   \n\t"  // tmp5     = _mm_mul_pd(tmp5, tmp0)
    "movaps  64(%%rax), %%xmm0   \n\t"  // tmp0     = _mm_load_pd(A+8)
    "mulpd     %%xmm1,  %%xmm7   \n\t"  // tmp7     = _mm_mul_pd(tmp7, tmp1)
    "movaps  80(%%rax), %%xmm1   \n\t"  // tmp1     = _mm_load_pd(A+10)
    "                            \n\t"
    "                            \n\t"
    "                            \n\t"  // 3. update
    "addpd     %%xmm3,  %%xmm12  \n\t"  // ab_02_13 = _mm_add_pd(ab_02_13, tmp3)
    "movaps  80(%%rbx), %%xmm3   \n\t"  // tmp3     = _mm_load_pd(B+10)
    "addpd     %%xmm6,  %%xmm13  \n\t"  // ab_22_33 = _mm_add_pd(ab_22_33, tmp6)
    "movaps    %%xmm2,  %%xmm6   \n\t"  // tmp6     = tmp2
    "pshufd $78,%%xmm2, %%xmm4   \n\t"  // tmp4     = _mm_shuffle_pd(tmp2, tmp2,
    "                            \n\t"  //                   _MM_SHUFFLE2(0, 1))
    "mulpd     %%xmm0,  %%xmm2   \n\t"  // tmp2     = _mm_mul_pd(tmp2, tmp0);
    "mulpd     %%xmm1,  %%xmm6   \n\t"  // tmp6     = _mm_mul_pd(tmp6, tmp1);
    "                            \n\t"
    "                            \n\t"
    "addpd     %%xmm5,  %%xmm14  \n\t"  // ab_03_12 = _mm_add_pd(ab_03_12, tmp5)
    "addpd     %%xmm7,  %%xmm15  \n\t"  // ab_23_32 = _mm_add_pd(ab_23_32, tmp7)
    "movaps    %%xmm4,  %%xmm7   \n\t"  // tmp7     = tmp4
    "mulpd     %%xmm0,  %%xmm4   \n\t"  // tmp4     = _mm_mul_pd(tmp4, tmp0)
    "mulpd     %%xmm1,  %%xmm7   \n\t"  // tmp7     = _mm_mul_pd(tmp7, tmp1)
    "                            \n\t"
    "                            \n\t"
    "addpd     %%xmm2,  %%xmm8   \n\t"  // ab_00_11 = _mm_add_pd(ab_00_11, tmp2)
    "movaps  96(%%rbx), %%xmm2   \n\t"  // tmp2     = _mm_load_pd(B+12)
    "addpd     %%xmm6,  %%xmm9   \n\t"  // ab_20_31 = _mm_add_pd(ab_20_31, tmp6)
    "movaps    %%xmm3,  %%xmm6   \n\t"  // tmp6     = tmp3
    "pshufd $78,%%xmm3, %%xmm5   \n\t"  // tmp5     = _mm_shuffle_pd(tmp3, tmp3,
    "                            \n\t"  //                   _MM_SHUFFLE2(0, 1))
    "mulpd     %%xmm0,  %%xmm3   \n\t"  // tmp3     = _mm_mul_pd(tmp3, tmp0)
    "mulpd     %%xmm1,  %%xmm6   \n\t"  // tmp6     = _mm_mul_pd(tmp6, tmp1)
    "                            \n\t"
    "                            \n\t"
    "addpd     %%xmm4,  %%xmm10  \n\t"  // ab_01_10 = _mm_add_pd(ab_01_10, tmp4)
    "addpd     %%xmm7,  %%xmm11  \n\t"  // ab_21_30 = _mm_add_pd(ab_21_30, tmp7)
    "movaps    %%xmm5,  %%xmm7   \n\t"  // tmp7     = tmp5
    "mulpd     %%xmm0,  %%xmm5   \n\t"  // tmp5     = _mm_mul_pd(tmp5, tmp0)
    "movaps  96(%%rax), %%xmm0   \n\t"  // tmp0     = _mm_load_pd(A+12)
    "mulpd     %%xmm1,  %%xmm7   \n\t"  // tmp7     = _mm_mul_pd(tmp7, tmp1)
    "movaps 112(%%rax), %%xmm1   \n\t"  // tmp1     = _mm_load_pd(A+14)
    "                            \n\t"
    "                            \n\t"
    "                            \n\t"  // 4. update
    "addpd     %%xmm3,  %%xmm12  \n\t"  // ab_02_13 = _mm_add_pd(ab_02_13, tmp3)
    "movaps 112(%%rbx), %%xmm3   \n\t"  // tmp3     = _mm_load_pd(B+14)
    "addpd     %%xmm6,  %%xmm13  \n\t"  // ab_22_33 = _mm_add_pd(ab_22_33, tmp6)
    "movaps    %%xmm2,  %%xmm6   \n\t"  // tmp6     = tmp2
    "pshufd $78,%%xmm2, %%xmm4   \n\t"  // tmp4     = _mm_shuffle_pd(tmp2, tmp2,
    "                            \n\t"  //                   _MM_SHUFFLE2(0, 1))
    "mulpd     %%xmm0,  %%xmm2   \n\t"  // tmp2     = _mm_mul_pd(tmp2, tmp0);
    "mulpd     %%xmm1,  %%xmm6   \n\t"  // tmp6     = _mm_mul_pd(tmp6, tmp1);
    "                            \n\t"
    "addq      $32*4,   %%rax    \n\t"  // A += 16;
    "                            \n\t"
    "addpd     %%xmm5,  %%xmm14  \n\t"  // ab_03_12 = _mm_add_pd(ab_03_12, tmp5)
    "addpd     %%xmm7,  %%xmm15  \n\t"  // ab_23_32 = _mm_add_pd(ab_23_32, tmp7)
    "movaps    %%xmm4,  %%xmm7   \n\t"  // tmp7     = tmp4
    "mulpd     %%xmm0,  %%xmm4   \n\t"  // tmp4     = _mm_mul_pd(tmp4, tmp0)
    "mulpd     %%xmm1,  %%xmm7   \n\t"  // tmp7     = _mm_mul_pd(tmp7, tmp1)
    "                            \n\t"
    "                            \n\t"
    "addpd     %%xmm2,  %%xmm8   \n\t"  // ab_00_11 = _mm_add_pd(ab_00_11, tmp2)
    "movaps 128(%%rbx), %%xmm2   \n\t"  // tmp2     = _mm_load_pd(B+16)
    "addpd     %%xmm6,  %%xmm9   \n\t"  // ab_20_31 = _mm_add_pd(ab_20_31, tmp6)
    "movaps    %%xmm3,  %%xmm6   \n\t"  // tmp6     = tmp3
    "pshufd $78,%%xmm3, %%xmm5   \n\t"  // tmp5     = _mm_shuffle_pd(tmp3, tmp3,
    "                            \n\t"  //                   _MM_SHUFFLE2(0, 1))
    "mulpd     %%xmm0,  %%xmm3   \n\t"  // tmp3     = _mm_mul_pd(tmp3, tmp0)
    "mulpd     %%xmm1,  %%xmm6   \n\t"  // tmp6     = _mm_mul_pd(tmp6, tmp1)
    "                            \n\t"
    "addq      $32*4,   %%rbx    \n\t"  // B += 16;
    "                            \n\t"
    "                            \n\t"
    "addpd     %%xmm4,  %%xmm10  \n\t"  // ab_01_10 = _mm_add_pd(ab_01_10, tmp4)
    "addpd     %%xmm7,  %%xmm11  \n\t"  // ab_21_30 = _mm_add_pd(ab_21_30, tmp7)
    "movaps    %%xmm5,  %%xmm7   \n\t"  // tmp7     = tmp5
    "mulpd     %%xmm0,  %%xmm5   \n\t"  // tmp5     = _mm_mul_pd(tmp5, tmp0)
    "movaps    (%%rax), %%xmm0   \n\t"  // tmp0     = _mm_load_pd(A+16)
    "mulpd     %%xmm1,  %%xmm7   \n\t"  // tmp7     = _mm_mul_pd(tmp7, tmp1)
    "movaps  16(%%rax), %%xmm1   \n\t"  // tmp1     = _mm_load_pd(A+18)
    "                            \n\t"
    "                            \n\t"
    "decq      %%rsi             \n\t"  // --l
    "jne       .DLOOP%=          \n\t"  // if l>= 1 go back
    "                            \n\t"
    "                            \n\t"
    ".DCONSIDERLEFT%=:           \n\t"
    "testq     %%rdi,   %%rdi    \n\t"  // if kl==0 writeback to AB
    "je        .DPOSTACCUMULATE%=\n\t"
    "                            \n\t"
    ".DLOOPLEFT%=:               \n\t"  // for l = kl,..,1 do
    "                            \n\t"
    "addpd     %%xmm3,  %%xmm12  \n\t"  // ab_02_13 = _mm_add_pd(ab_02_13, tmp3)
    "movapd  16(%%rbx), %%xmm3   \n\t"  // tmp3     = _mm_load_pd(B+2)
    "addpd     %%xmm6,  %%xmm13  \n\t"  // ab_22_33 = _mm_add_pd(ab_22_33, tmp6)
    "movapd    %%xmm2,  %%xmm6   \n\t"  // tmp6     = tmp2
    "pshufd $78,%%xmm2, %%xmm4   \n\t"  // tmp4     = _mm_shuffle_pd(tmp2, tmp2,
    "                            \n\t"  //                   _MM_SHUFFLE2(0, 1))
    "mulpd     %%xmm0,  %%xmm2   \n\t"  // tmp2     = _mm_mul_pd(tmp2, tmp0);
    "mulpd     %%xmm1,  %%xmm6   \n\t"  // tmp6     = _mm_mul_pd(tmp6, tmp1);
    "                            \n\t"
    "                            \n\t"
    "addpd     %%xmm5,  %%xmm14  \n\t"  // ab_03_12 = _mm_add_pd(ab_03_12, tmp5)
    "addpd     %%xmm7,  %%xmm15  \n\t"  // ab_23_32 = _mm_add_pd(ab_23_32, tmp7)
    "movapd    %%xmm4,  %%xmm7   \n\t"  // tmp7     = tmp4
    "mulpd     %%xmm0,  %%xmm4   \n\t"  // tmp4     = _mm_mul_pd(tmp4, tmp0)
    "mulpd     %%xmm1,  %%xmm7   \n\t"  // tmp7     = _mm_mul_pd(tmp7, tmp1)
    "                            \n\t"
    "                            \n\t"
    "addpd     %%xmm2,  %%xmm8   \n\t"  // ab_00_11 = _mm_add_pd(ab_00_11, tmp2)
    "movapd  32(%%rbx), %%xmm2   \n\t"  // tmp2     = _mm_load_pd(B+4)
    "addpd     %%xmm6,  %%xmm9   \n\t"  // ab_20_31 = _mm_add_pd(ab_20_31, tmp6)
    "movapd    %%xmm3,  %%xmm6   \n\t"  // tmp6     = tmp3
    "pshufd $78,%%xmm3, %%xmm5   \n\t"  // tmp5     = _mm_shuffle_pd(tmp3, tmp3,
    "                            \n\t"  //                   _MM_SHUFFLE2(0, 1))
    "mulpd     %%xmm0,  %%xmm3   \n\t"  // tmp3     = _mm_mul_pd(tmp3, tmp0)
    "mulpd     %%xmm1,  %%xmm6   \n\t"  // tmp6     = _mm_mul_pd(tmp6, tmp1)
    "                            \n\t"
    "                            \n\t"
    "addpd     %%xmm4,  %%xmm10  \n\t"  // ab_01_10 = _mm_add_pd(ab_01_10, tmp4)
    "addpd     %%xmm7,  %%xmm11  \n\t"  // ab_21_30 = _mm_add_pd(ab_21_30, tmp7)
    "movapd    %%xmm5,  %%xmm7   \n\t"  // tmp7     = tmp5
    "mulpd     %%xmm0,  %%xmm5   \n\t"  // tmp5     = _mm_mul_pd(tmp5, tmp0)
    "movapd  32(%%rax), %%xmm0   \n\t"  // tmp0     = _mm_load_pd(A+4)
    "mulpd     %%xmm1,  %%xmm7   \n\t"  // tmp7     = _mm_mul_pd(tmp7, tmp1)
    "movapd  48(%%rax), %%xmm1   \n\t"  // tmp1     = _mm_load_pd(A+6)
    "                            \n\t"
    "                            \n\t"
    "addq      $32,     %%rax    \n\t"  // A += 4;
    "addq      $32,     %%rbx    \n\t"  // B += 4;
    "                            \n\t"
    "decq      %%rdi             \n\t"  // --l
    "jne       .DLOOPLEFT%=      \n\t"  // if l>= 1 go back
    "                            \n\t"
    ".DPOSTACCUMULATE%=:         \n\t"  // Update remaining ab_*_* registers
    "                            \n\t"
    "addpd    %%xmm3,   %%xmm12  \n\t"  // ab_02_13 = _mm_add_pd(ab_02_13, tmp3)
    "addpd    %%xmm6,   %%xmm13  \n\t"  // ab_22_33 = _mm_add_pd(ab_22_33, tmp6)
    "                            \n\t"  //
    "addpd    %%xmm5,   %%xmm14  \n\t"  // ab_03_12 = _mm_add_pd(ab_03_12, tmp5)
    "addpd    %%xmm7,   %%xmm15  \n\t"  // ab_23_32 = _mm_add_pd(ab_23_32, tmp7)
    "                            \n\t"
    //
    //  Update C <- beta*C + alpha*AB
    //
    //
    "movsd  %4,                  %%xmm0 \n\t"  // load alpha
    "movsd  %5,                  %%xmm1 \n\t"  // load beta
    "movq   %6,                  %%rcx  \n\t"  // Address of C stored in %rcx

    "movq   %7,                  %%r8   \n\t"  // load incRowC
    "leaq   (,%%r8,8),           %%r8   \n\t"  //      incRowC *= sizeof(double)
    "movq   %8,                  %%r9   \n\t"  // load incColC
    "leaq   (,%%r9,8),           %%r9   \n\t"  //      incRowC *= sizeof(double)
    "                                   \n\t"
    "leaq (%%rcx,%%r9),          %%r10  \n\t"  // Store addr of C01 in %r10
    "leaq (%%rcx,%%r8,2),        %%rdx  \n\t"  // Store addr of C20 in %rdx
    "leaq (%%rdx,%%r9),          %%r11  \n\t"  // Store addr of C21 in %r11
    "                                   \n\t"
    "unpcklpd %%xmm0,            %%xmm0 \n\t"  // duplicate alpha
    "unpcklpd %%xmm1,            %%xmm1 \n\t"  // duplicate beta
    "                                   \n\t"
    "                                   \n\t"
    "movlpd (%%rcx),             %%xmm3 \n\t"  // load (C00,
    "movhpd (%%r10,%%r8),        %%xmm3 \n\t"  //       C11)
    "mulpd  %%xmm0,              %%xmm8 \n\t"  // scale ab_00_11 by alpha
    "mulpd  %%xmm1,              %%xmm3 \n\t"  // scale (C00, C11) by beta
    "addpd  %%xmm8,              %%xmm3 \n\t"  // add results

    "movlpd %%xmm3,        (%%rcx)       \n\t"  // write back (C00,
    "movhpd %%xmm3,        (%%r10,%%r8)  \n\t"  //             C11)
    "                                   \n\t"
    "movlpd (%%rdx),             %%xmm4 \n\t"  // load (C20,
    "movhpd (%%r11,%%r8),        %%xmm4 \n\t"  //       C31)
    "mulpd  %%xmm0,              %%xmm9 \n\t"  // scale ab_20_31 by alpha
    "mulpd  %%xmm1,              %%xmm4 \n\t"  // scale (C20, C31) by beta
    "addpd  %%xmm9,              %%xmm4 \n\t"  // add results
    "movlpd %%xmm4,        (%%rdx)       \n\t"  // write back (C20,
    "movhpd %%xmm4,        (%%r11,%%r8)  \n\t"  //             C31)
    "                                   \n\t"
    "                                   \n\t"
    "movlpd (%%r10),             %%xmm3 \n\t"  // load (C01,
    "movhpd (%%rcx,%%r8),        %%xmm3 \n\t"  //       C10)
    "mulpd  %%xmm0,              %%xmm10\n\t"  // scale ab_01_10 by alpha
    "mulpd  %%xmm1,              %%xmm3 \n\t"  // scale (C01, C10) by beta
    "addpd  %%xmm10,             %%xmm3 \n\t"  // add results
    "movlpd %%xmm3,        (%%r10)      \n\t"  // write back (C01,
    "movhpd %%xmm3,        (%%rcx,%%r8) \n\t"  //             C10)
    "                                   \n\t"
    "movlpd (%%r11),             %%xmm4 \n\t"  // load (C21,
    "movhpd (%%rdx,%%r8),        %%xmm4 \n\t"  //       C30)
    "mulpd  %%xmm0,              %%xmm11\n\t"  // scale ab_21_30 by alpha
    "mulpd  %%xmm1,              %%xmm4 \n\t"  // scale (C21, C30) by beta
    "addpd  %%xmm11,             %%xmm4 \n\t"  // add results
    "movlpd %%xmm4,        (%%r11)      \n\t"  // write back (C21,
    "movhpd %%xmm4,        (%%rdx,%%r8) \n\t"  //             C30)
    "                                   \n\t"
    "                                   \n\t"
    "leaq   (%%rcx,%%r9,2),      %%rcx  \n\t"  // Store addr of C02 in %rcx
    "leaq   (%%r10,%%r9,2),      %%r10  \n\t"  // Store addr of C03 in %r10
    "leaq   (%%rdx,%%r9,2),      %%rdx  \n\t"  // Store addr of C22 in $rdx
    "leaq   (%%r11,%%r9,2),      %%r11  \n\t"  // Store addr of C23 in %r11
    "                                   \n\t"
    "                                   \n\t"
    "movlpd (%%rcx),             %%xmm3 \n\t"  // load (C02,
    "movhpd (%%r10,%%r8),        %%xmm3 \n\t"  //       C13)
    "mulpd  %%xmm0,              %%xmm12\n\t"  // scale ab_02_13 by alpha
    "mulpd  %%xmm1,              %%xmm3 \n\t"  // scale (C02, C13) by beta
    "addpd  %%xmm12,             %%xmm3 \n\t"  // add results
    "movlpd %%xmm3,        (%%rcx)      \n\t"  // write back (C02,
    "movhpd %%xmm3,        (%%r10,%%r8) \n\t"  //             C13)
    "                                   \n\t"
    "movlpd (%%rdx),             %%xmm4 \n\t"  // load (C22,
    "movhpd (%%r11, %%r8),       %%xmm4 \n\t"  //       C33)
    "mulpd  %%xmm0,              %%xmm13\n\t"  // scale ab_22_33 by alpha
    "mulpd  %%xmm1,              %%xmm4 \n\t"  // scale (C22, C33) by beta
    "addpd  %%xmm13,             %%xmm4 \n\t"  // add results
    "movlpd %%xmm4,             (%%rdx) \n\t"  // write back (C22,
    "movhpd %%xmm4,        (%%r11,%%r8) \n\t"  //             C33)
    "                                   \n\t"
    "                                   \n\t"
    "movlpd (%%r10),             %%xmm3 \n\t"  // load (C03,
    "movhpd (%%rcx,%%r8),        %%xmm3 \n\t"  //       C12)
    "mulpd  %%xmm0,              %%xmm14\n\t"  // scale ab_03_12 by alpha
    "mulpd  %%xmm1,              %%xmm3 \n\t"  // scale (C03, C12) by beta
    "addpd  %%xmm14,             %%xmm3 \n\t"  // add results
    "movlpd %%xmm3,        (%%r10)      \n\t"  // write back (C03,
    "movhpd %%xmm3,        (%%rcx,%%r8) \n\t"  //             C12)
    "                                   \n\t"
    "movlpd (%%r11),             %%xmm4 \n\t"  // load (C23,
    "movhpd (%%rdx,%%r8),        %%xmm4 \n\t"  //       C32)
    "mulpd  %%xmm0,              %%xmm15\n\t"  // scale ab_23_32 by alpha
    "mulpd  %%xmm1,              %%xmm4 \n\t"  // scale (C23, C32) by beta
    "addpd  %%xmm15,             %%xmm4 \n\t"  // add results
    "movlpd %%xmm4,        (%%r11)      \n\t"  // write back (C23,
    "movhpd %%xmm4,        (%%rdx,%%r8) \n\t"  //             C32)
    : // output
    : // input
    "m" (kb),       // 0
    "m" (kl),       // 1
    "m" (A),        // 2
    "m" (B),        // 3
    "m" (alpha),    // 4
    "m" (beta),     // 5
    "m" (C),        // 6
    "m" (incRowC),  // 7
    "m" (incColC)   // 8
    : // register clobber list
    "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "r8", "r9", "r10", "r11",
    "xmm0", "xmm1", "xmm2", "xmm3",
    "xmm4", "xmm5", "xmm6", "xmm7",
    "xmm8", "xmm9", "xmm10", "xmm11",
    "xmm12", "xmm13", "xmm14", "xmm15"
    );
}

//
//  Compute Y += alpha*X
//
static void
dgeaxpy(long m,
        long n,
        double alpha,
        const double *X,
        long incRowX,
        long incColX,
        double *Y,
        long incRowY,
        long incColY) {
    long i, j;


    if (alpha != 1.0) {
        for (j = 0; j < n; ++j) {
            for (i = 0; i < m; ++i) {
                Y[i * incRowY + j * incColY] += alpha * X[i * incRowX + j * incColX];
            }
        }
    } else {
        for (j = 0; j < n; ++j) {
            for (i = 0; i < m; ++i) {
                Y[i * incRowY + j * incColY] += X[i * incRowX + j * incColX];
            }
        }
    }
}

//
//  Compute X *= alpha
//
static void
dgescal(long m,
        long n,
        double alpha,
        double *X,
        long incRowX,
        long incColX) {
    long i, j;

    if (alpha != 0.0) {
        for (j = 0; j < n; ++j) {
            for (i = 0; i < m; ++i) {
                X[i * incRowX + j * incColX] *= alpha;
            }
        }
    } else {
        for (j = 0; j < n; ++j) {
            for (i = 0; i < m; ++i) {
                X[i * incRowX + j * incColX] = 0.0;
            }
        }
    }
}

//
//  Macro Kernel for the multiplication of blocks of A and B.  We assume that
//  these blocks were previously packed to buffers A_BUFFER and B_BUFFER.
//
static void
dgemm_macro_kernel(long mc,
                   long nc,
                   long kc,
                   double alpha,
                   double beta,
                   double *C,
                   long incRowC,
                   long incColC) {
    long mp = (mc + MR - 1) / MR;
    long np = (nc + NR - 1) / NR;

    long _mr = mc % MR;
    long _nr = nc % NR;

    long mr, nr;
    long i, j;

#   pragma omp parallel for
    for (j = 0; j < np; ++j) {
        nr = (j != np - 1 || _nr == 0) ? NR : _nr;

        for (i = 0; i < mp; ++i) {
            mr = (i != mp - 1 || _mr == 0) ? MR : _mr;

            if (mr == MR && nr == NR) {
                dgemm_micro_kernel(kc, alpha, &A_BUFFER[i * kc * MR], &B_BUFFER[j * kc * NR],
                                   beta,
                                   &C[i * MR * incRowC + j * NR * incColC],
                                   incRowC, incColC);
            } else {
                dgemm_micro_kernel(kc, alpha, &A_BUFFER[i * kc * MR], &B_BUFFER[j * kc * NR],
                                   0.0,
                                   C_BUFFER, 1, MR);
                dgescal(mr, nr, beta,
                        &C[i * MR * incRowC + j * NR * incColC], incRowC, incColC);
                dgeaxpy(mr, nr, 1.0, C_BUFFER, 1, MR,
                        &C[i * MR * incRowC + j * NR * incColC], incRowC, incColC);
            }
        }
    }
}

//
//  Compute C <- beta*C + alpha*A*B
//
void
ULMBLAS(dgemm_nn)(long m,
                  long n,
                  long k,
                  double alpha,
                  const double *A,
                  long incRowA,
                  long incColA,
                  const double *B,
                  long incRowB,
                  long incColB,
                  double beta,
                  double *C,
                  long incRowC,
                  long incColC) {
    long mb = (m + MC - 1) / MC;
    long nb = (n + NC - 1) / NC;
    long kb = (k + KC - 1) / KC;

    long _mc = m % MC;
    long _nc = n % NC;
    long _kc = k % KC;

    long mc, nc, kc;
    long i, j, l;

    double _beta;

    if (alpha == 0.0 || k == 0) {
        dgescal(m, n, beta, C, incRowC, incColC);
        return;
    }

    for (j = 0; j < nb; ++j) {
        nc = (j != nb - 1 || _nc == 0) ? NC : _nc;

        for (l = 0; l < kb; ++l) {
            kc = (l != kb - 1 || _kc == 0) ? KC : _kc;
            _beta = (l == 0) ? beta : 1.0;

            pack_B(kc, nc,
                   &B[l * KC * incRowB + j * NC * incColB], incRowB, incColB,
                   B_BUFFER);

            for (i = 0; i < mb; ++i) {
                mc = (i != mb - 1 || _mc == 0) ? MC : _mc;

                pack_A(mc, kc,
                       &A[i * MC * incRowA + l * KC * incColA], incRowA, incColA,
                       A_BUFFER);

                dgemm_macro_kernel(mc, nc, kc, alpha, _beta,
                                   &C[i * MC * incRowC + j * NC * incColC],
                                   incRowC, incColC);
            }
        }
    }
}

#else

#define MC  384
#define KC  384
#define NC  4096

#define MR  4
#define NR  4

//
//  Local buffers for storing panels from A, B and C
//
static double __declspec(align(16)) A_BUFFER[MC * KC];
static double __declspec(align(16)) B_BUFFER[KC * NC];
static double __declspec(align(16)) C_BUFFER[MR * NR];

//
//  Packing complete panels from A (i.e. without padding)
//
static void
pack_MRxk(long int k, const double *A, long int incRowA, long int incColA,
          double *buffer) {
    long int i, j;

    for (j = 0; j < k; ++j) {
        for (i = 0; i < MR; ++i) {
            buffer[i] = A[i * incRowA];
        }
        buffer += MR;
        A += incColA;
    }
}

//
//  Packing panels from A with padding if required
//
static void
pack_A(long int mc, long int kc, const double *A, long int incRowA, long int incColA,
       double *buffer) {
    long int mp = mc / MR;
    long int _mr = mc % MR;

    long int i, j;

    for (i = 0; i < mp; ++i) {
        pack_MRxk(kc, A, incRowA, incColA, buffer);
        buffer += kc * MR;
        A += MR * incRowA;
    }
    if (_mr > 0) {
        for (j = 0; j < kc; ++j) {
            for (i = 0; i < _mr; ++i) {
                buffer[i] = A[i * incRowA];
            }
            for (i = _mr; i < MR; ++i) {
                buffer[i] = 0.0;
            }
            buffer += MR;
            A += incColA;
        }
    }
}

//
//  Packing complete panels from B (i.e. without padding)
//
static void
pack_kxNR(long int k, const double *B, long int incRowB, long int incColB,
          double *buffer) {
    long int i, j;

    for (i = 0; i < k; ++i) {
        for (j = 0; j < NR; ++j) {
            buffer[j] = B[j * incColB];
        }
        buffer += NR;
        B += incRowB;
    }
}

//
//  Packing panels from B with padding if required
//
static void
pack_B(long int kc, long int nc, const double *B, long int incRowB, long int incColB,
       double *buffer) {
    long int np = nc / NR;
    long int _nr = nc % NR;

    long int i, j;

    for (j = 0; j < np; ++j) {
        pack_kxNR(kc, B, incRowB, incColB, buffer);
        buffer += kc * NR;
        B += NR * incColB;
    }
    if (_nr > 0) {
        for (i = 0; i < kc; ++i) {
            for (j = 0; j < _nr; ++j) {
                buffer[j] = B[j * incColB];
            }
            for (j = _nr; j < NR; ++j) {
                buffer[j] = 0.0;
            }
            buffer += NR;
            B += incRowB;
        }
    }
}

//
//  Micro kernel for multiplying panels from A and B.
//
static void
dgemm_micro_kernel(long kc,
                   double alpha, const double *A, const double *B,
                   double beta,
                   double *C, long incRowC, long incColC) {
    double __declspec(align(16)) AB[MR * NR];

    long int i, j, l;

//
//  Compute AB = A*B
//
    register __m128d ab_00_11, ab_20_31;
    register __m128d ab_01_10, ab_21_30;
    register __m128d ab_02_13, ab_22_33;
    register __m128d ab_03_12, ab_23_32;

    register __m128d tmp0, tmp1, tmp2, tmp3;
    register __m128d tmp4, tmp5, tmp6, tmp7;

    tmp0 = _mm_load_pd(A);                                      // (1)
    tmp1 = _mm_load_pd(A + 2);                                    // (2)
    tmp2 = _mm_load_pd(B);                                      // (3)

    ab_00_11 = _mm_setzero_pd();
    ab_20_31 = _mm_setzero_pd();
    ab_01_10 = _mm_setzero_pd();
    ab_21_30 = _mm_setzero_pd();
    ab_02_13 = _mm_setzero_pd();
    ab_22_33 = _mm_setzero_pd();
    ab_03_12 = _mm_setzero_pd();
    ab_23_32 = _mm_setzero_pd();

    for (l = 0; l < kc; ++l) {
        tmp3 = _mm_load_pd(B + 2);

        tmp4 = _mm_shuffle_pd(tmp2, tmp2, _MM_SHUFFLE2(0, 1));
        tmp5 = _mm_shuffle_pd(tmp3, tmp3, _MM_SHUFFLE2(0, 1));

        tmp6 = tmp2;
        tmp2 = _mm_mul_pd(tmp2, tmp0);
        tmp6 = _mm_mul_pd(tmp6, tmp1);
        ab_00_11 = _mm_add_pd(ab_00_11, tmp2);
        ab_20_31 = _mm_add_pd(ab_20_31, tmp6);

        tmp7 = tmp4;
        tmp4 = _mm_mul_pd(tmp4, tmp0);
        tmp7 = _mm_mul_pd(tmp7, tmp1);
        ab_01_10 = _mm_add_pd(ab_01_10, tmp4);
        ab_21_30 = _mm_add_pd(ab_21_30, tmp7);

        tmp2 = _mm_load_pd(B + 4);                                // (6)
        tmp6 = tmp3;
        tmp3 = _mm_mul_pd(tmp3, tmp0);
        tmp6 = _mm_mul_pd(tmp6, tmp1);
        ab_02_13 = _mm_add_pd(ab_02_13, tmp3);
        ab_22_33 = _mm_add_pd(ab_22_33, tmp6);

        tmp7 = tmp5;
        tmp5 = _mm_mul_pd(tmp5, tmp0);
        tmp0 = _mm_load_pd(A + 4);                                // (4)
        tmp7 = _mm_mul_pd(tmp7, tmp1);
        tmp1 = _mm_load_pd(A + 6);                                // (5)
        ab_03_12 = _mm_add_pd(ab_03_12, tmp5);
        ab_23_32 = _mm_add_pd(ab_23_32, tmp7);

        A += 4;
        B += 4;
    }

    _mm_storel_pd(&AB[0 + 0 * 4], ab_00_11);
    _mm_storeh_pd(&AB[1 + 0 * 4], ab_01_10);
    _mm_storel_pd(&AB[2 + 0 * 4], ab_20_31);
    _mm_storeh_pd(&AB[3 + 0 * 4], ab_21_30);

    _mm_storel_pd(&AB[0 + 1 * 4], ab_01_10);
    _mm_storeh_pd(&AB[1 + 1 * 4], ab_00_11);
    _mm_storel_pd(&AB[2 + 1 * 4], ab_21_30);
    _mm_storeh_pd(&AB[3 + 1 * 4], ab_20_31);

    _mm_storel_pd(&AB[0 + 2 * 4], ab_02_13);
    _mm_storeh_pd(&AB[1 + 2 * 4], ab_03_12);
    _mm_storel_pd(&AB[2 + 2 * 4], ab_22_33);
    _mm_storeh_pd(&AB[3 + 2 * 4], ab_23_32);

    _mm_storel_pd(&AB[0 + 3 * 4], ab_03_12);
    _mm_storeh_pd(&AB[1 + 3 * 4], ab_02_13);
    _mm_storel_pd(&AB[2 + 3 * 4], ab_23_32);
    _mm_storeh_pd(&AB[3 + 3 * 4], ab_22_33);

//
//  Update C <- beta*C
//
    if (beta == 0.0) {
        for (j = 0; j < NR; ++j) {
            for (i = 0; i < MR; ++i) {
                C[i * incRowC + j * incColC] = 0.0;
            }
        }
    } else if (beta != 1.0) {
        for (j = 0; j < NR; ++j) {
            for (i = 0; i < MR; ++i) {
                C[i * incRowC + j * incColC] *= beta;
            }
        }
    }

//
//  Update C <- C + alpha*AB (note: the case alpha==0.0 was already treated in
//                                  the above layer dgemm_nn)
//
    if (alpha == 1.0) {
        for (j = 0; j < NR; ++j) {
            for (i = 0; i < MR; ++i) {
                C[i * incRowC + j * incColC] += AB[i + j * MR];
            }
        }
    } else {
        for (j = 0; j < NR; ++j) {
            for (i = 0; i < MR; ++i) {
                C[i * incRowC + j * incColC] += alpha * AB[i + j * MR];
            }
        }
    }
}

//
//  Compute Y += alpha*X
//
static void
dgeaxpy(long int m,
        long int n,
        double alpha,
        const double *X,
        long int incRowX,
        long int incColX,
        double *Y,
        long int incRowY,
        long int incColY) {
    long int i, j;


    if (alpha != 1.0) {
        for (j = 0; j < n; ++j) {
            for (i = 0; i < m; ++i) {
                Y[i * incRowY + j * incColY] += alpha * X[i * incRowX + j * incColX];
            }
        }
    } else {
        for (j = 0; j < n; ++j) {
            for (i = 0; i < m; ++i) {
                Y[i * incRowY + j * incColY] += X[i * incRowX + j * incColX];
            }
        }
    }
}

//
//  Compute X *= alpha
//
static void
dgescal(long int m,
        long int n,
        double alpha,
        double *X,
        long int incRowX,
        long int incColX) {
    long int i, j;

    if (alpha != 0.0) {
        for (j = 0; j < n; ++j) {
            for (i = 0; i < m; ++i) {
                X[i * incRowX + j * incColX] *= alpha;
            }
        }
    } else {
        for (j = 0; j < n; ++j) {
            for (i = 0; i < m; ++i) {
                X[i * incRowX + j * incColX] = 0.0;
            }
        }
    }
}

//
//  Macro Kernel for the multiplication of blocks of A and B.  We assume that
//  these blocks were previously packed to buffers A_BUFFER and B_BUFFER.
//
static void
dgemm_macro_kernel(long int mc,
                   long int nc,
                   long int kc,
                   double alpha,
                   double beta,
                   double *C,
                   long int incRowC,
                   long int incColC) {
    long int mp = (mc + MR - 1) / MR;
    long int np = (nc + NR - 1) / NR;

    long int _mr = mc % MR;
    long int _nr = nc % NR;

    long int mr, nr;
    long int i, j;

#   pragma omp parallel for private(j, nr, i, mr) shared(np, _nr, mp, _mr, kc, alpha, A_BUFFER, B_BUFFER, C_BUFFER) default(none)
    for (j = 0; j < np; ++j) {
        nr = (j != np - 1 || _nr == 0) ? NR : _nr;

        for (i = 0; i < mp; ++i) {
            mr = (i != mp - 1 || _mr == 0) ? MR : _mr;

            if (mr == MR && nr == NR) {
                dgemm_micro_kernel(kc, alpha, &A_BUFFER[i * kc * MR], &B_BUFFER[j * kc * NR],
                                   beta,
                                   &C[i * MR * incRowC + j * NR * incColC],
                                   incRowC, incColC);
            } else {
                dgemm_micro_kernel(kc, alpha, &A_BUFFER[i * kc * MR], &B_BUFFER[j * kc * NR],
                                   0.0,
                                   C_BUFFER, 1, MR);
                dgescal(mr, nr, beta,
                        &C[i * MR * incRowC + j * NR * incColC], incRowC, incColC);
                dgeaxpy(mr, nr, 1.0, C_BUFFER, 1, MR,
                        &C[i * MR * incRowC + j * NR * incColC], incRowC, incColC);
            }
        }
    }
}

//
//  Compute C <- beta*C + alpha*A*B
//
void
ULMBLAS(dgemm_nn)(long int m,
                  long int n,
                  long int k,
                  double alpha,
                  const double *A,
                  long int incRowA,
                  long int incColA,
                  const double *B,
                  long int incRowB,
                  long int incColB,
                  double beta,
                  double *C,
                  long int incRowC,
                  long int incColC) {
    long int mb = (m + MC - 1) / MC;
    long int nb = (n + NC - 1) / NC;
    long int kb = (k + KC - 1) / KC;

    long int _mc = m % MC;
    long int _nc = n % NC;
    long int _kc = k % KC;

    long int mc, nc, kc;
    long int i, j, l;

    double _beta;

    if (alpha == 0.0 || k == 0) {
        dgescal(m, n, beta, C, incRowC, incColC);
        return;
    }

    for (j = 0; j < nb; ++j) {
        nc = (j != nb - 1 || _nc == 0) ? NC : _nc;

        for (l = 0; l < kb; ++l) {
            kc = (l != kb - 1 || _kc == 0) ? KC : _kc;
            _beta = (l == 0) ? beta : 1.0;

            pack_B(kc, nc,
                   &B[l * KC * incRowB + j * NC * incColB], incRowB, incColB,
                   B_BUFFER);

            for (i = 0; i < mb; ++i) {
                mc = (i != mb - 1 || _mc == 0) ? MC : _mc;

                pack_A(mc, kc,
                       &A[i * MC * incRowA + l * KC * incColA], incRowA, incColA,
                       A_BUFFER);

                dgemm_macro_kernel(mc, nc, kc, alpha, _beta,
                                   &C[i * MC * incRowC + j * NC * incColC],
                                   incRowC, incColC);
            }
        }
    }
}

#endif
