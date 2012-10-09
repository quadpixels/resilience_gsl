#include <gsl/gsl_math.h>
#include <gsl/gsl_cblas.h>
#include <gsl/triplicate.h>
#include <gsl/tommy.h>
#include "cblas.h"
#include "../real.h"
#include <setjmp.h>
#include <errno.h>

static volatile int fault_count_mm = 0;
static const int fault_limit_mm = 10;
static jmp_buf buf_mm;
static INDEX i, j, k;

static void gemm_handler(int sig, siginfo_t* si, void* unused) {
	puts(" >> Routine rollback (MM)");
	if(fault_count_mm < fault_limit_mm)
		siglongjmp(buf_mm, 1);
	else
		siglongjmp(buf, 1);
}

void
cblas_sgemm (const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA,
             const enum CBLAS_TRANSPOSE TransB, const int M, const int N,
             const int K, const float alpha, const float *A, const int lda,
             const float *B, const int ldb, const float beta, float *C,
             const int ldc)
{
#define BASE float
#include "source_gemm_r.h"
#undef BASE
}
