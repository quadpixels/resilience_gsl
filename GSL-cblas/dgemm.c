#include <gsl/gsl_math.h>
#include <gsl/gsl_cblas.h>
#include <gsl/triplicate.h>
#include <gsl/tommy.h>
#include "cblas.h"
#include "../real.h"
#define forceinline __inline__ __attribute__((always_inline))

void
cblas_dgemm (const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA,
             const enum CBLAS_TRANSPOSE TransB, const int M, const int N,
             const int K, const double alpha, const double *A, const int lda,
             const double *B, const int ldb, const double beta, double *C,
             const int ldc)
{
//REAL_TRY(0) { /* Strangely, adding try-catch block here makes the compiler complain an error! */
#define BASE double
#include "source_gemm_r.h"
#undef BASE
//} REAL_CATCH(0) {} REAL_END(0);
}
