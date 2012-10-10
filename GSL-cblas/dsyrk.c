#include <gsl/gsl_math.h>
#include <gsl/gsl_cblas.h>
#include "cblas.h"
#include "../real.h"
#include <gsl/tommy.h>
#include <setjmp.h>
#include <signal.h>
#include <errno.h>

static volatile INDEX i, j, k;
static volatile int fault_count_rk = 0;
static const int fault_limit_rk = 10;
static jmp_buf buf_rk;

static void syrk_handler(int sig, siginfo_t* si, void* unused) {
	puts(" >> Routine rollback (RK)");
	if(++fault_count_rk < fault_limit_rk) 
		siglongjmp(buf_rk, 1);
	else
		siglongjmp(buf, 1);
}

void
cblas_dsyrk (const enum CBLAS_ORDER Order, const enum CBLAS_UPLO Uplo,
             const enum CBLAS_TRANSPOSE Trans, const int N, const int K,
             const double alpha, const double *A, const int lda,
             const double beta, double *C, const int ldc)
{
#define BASE double
#include "source_syrk_r.h"
#undef BASE
}
