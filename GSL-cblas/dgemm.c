#include <gsl/gsl_math.h>
#include <gsl/gsl_cblas.h>
#include <gsl/triplicate.h>
#include <gsl/tommy.h>
#include "cblas.h"
#include "../real.h"
#include <setjmp.h>
#include <errno.h>
#include <signal.h>
#define forceinline __inline__ __attribute__((always_inline))

static volatile int fault_count_mm = 0;
static const int fault_limit_mm = 10;
static jmp_buf buf_mm;

static INDEX i, j, k;
volatile sig_atomic_t i_1, j_1, k_1;
static volatile INDEX *p_i, *p_j, *p_k;

static void gemm_handler(int sig, siginfo_t* si, void* unused) {
	printf("[dgemm handler]\n");
	printf(" >> Caught SIGSEGV signal (%d out of %d allowed)",
		fault_count_mm, fault_limit_mm);
	printf(" >> i(%p)=%d, j(%p)=%d, k(%p)=%d\n",
		p_i, *p_i, p_j, *p_j, p_k, *p_k);
	i_1 = *p_i; j_1 = *p_j; k_1 = *p_k;
		
	if(fault_count_mm < fault_limit_mm)
		siglongjmp(buf_mm, 1); // Revert to re-entry
	else
		siglongjmp(buf, 1); // Revert the entire routine
}

void
cblas_dgemm (const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA,
             const enum CBLAS_TRANSPOSE TransB, const int M, const int N,
             const int K, const double alpha, const double *A, const int lda,
             const double *B, const int ldb, const double beta, double *C,
             const int ldc)
{
#define BASE double
#include "source_gemm_r.h"
#undef BASE
}
