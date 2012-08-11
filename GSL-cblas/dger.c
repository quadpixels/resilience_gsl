#include <gsl/gsl_math.h>
#include <gsl/gsl_cblas.h>
#include "cblas.h"
#include "../real.h"

#ifdef __cplusplus
extern "C"
#endif
void
cblas_dger (const enum CBLAS_ORDER order, const int M, const int N,
            const double alpha, const double *X, const int incX,
            const double *Y, const int incY, double *A, const int lda)
{
#define BASE double
#include "source_ger.h"
#undef BASE
}
