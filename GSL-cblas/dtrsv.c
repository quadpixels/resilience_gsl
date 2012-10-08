#include <gsl/gsl_math.h>
#include <gsl/gsl_cblas.h>
#include "cblas.h"
#include "../real.h"
#include <gsl/tommy.h>
#include <setjmp.h>
#include <errno.h>


static volatile int fault_count_sv = 0;
static const int fault_limit_sv = 10;
static jmp_buf buf_sv;
static volatile INDEX ix, jx, i, j;

static void trsv_handler(int sig, siginfo_t* si, void* unused) {
	if(++fault_count_sv < fault_limit_sv)
		siglongjmp(buf_sv, 1);
	else
		siglongjmp(buf, 1);
}

void cblas_dtrsv (const enum CBLAS_ORDER order, const enum CBLAS_UPLO Uplo,
             const enum CBLAS_TRANSPOSE TransA, const enum CBLAS_DIAG Diag,
             const int N, const double *A, const int lda, double *X,
             const int incX)
{
#define BASE double
#include "source_trsv_r.h"
#undef BASE
}
