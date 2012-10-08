#include <gsl/gsl_math.h>
#include <gsl/gsl_cblas.h>
#include <gsl/tommy.h>
#include "cblas.h"
#include "../real.h"
#include <setjmp.h>
#include <signal.h>

static volatile int fault_count_mv = 0;
static const int fault_limit_mv = 10;
static jmp_buf buf_mv;
static volatile INDEX i, j, iy;

static void gemv_handler(int sig, siginfo_t* si, void* unused) {
	printf(" >> Caught SIGSEGV signal (%d out of %d allowed)",
		fault_count_mv, fault_limit_mv);
	if(++fault_count_mv < fault_limit_mv)
		siglongjmp(buf_mv, 1);
	else
		siglongjmp(buf, 1);
}

void
cblas_dgemv (const enum CBLAS_ORDER order, const enum CBLAS_TRANSPOSE TransA,
             const int M, const int N, const double alpha, const double *A,
             const int lda, const double *X, const int incX,
             const double beta, double *Y, const int incY)
{
#define BASE double
#include "source_gemv_r.h"
#undef BASE
}
