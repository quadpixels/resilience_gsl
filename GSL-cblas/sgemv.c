#include <gsl/gsl_math.h>
#include <gsl/gsl_cblas.h>
#include <gsl/triplicate.h>
#include <gsl/tommy.h>
#include "cblas.h"
#include "../real.h"
#include <setjmp.h>


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
cblas_sgemv (const enum CBLAS_ORDER order, const enum CBLAS_TRANSPOSE TransA,
             const int M, const int N, const float alpha, const float *A,
             const int lda, const float *X, const int incX, const float beta,
             float *Y, const int incY)
{
#define BASE float
#include "source_gemv_r.h"
#undef BASE
}
