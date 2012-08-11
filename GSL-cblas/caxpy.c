#include <gsl/gsl_math.h>
#include <gsl/gsl_cblas.h>
#include "cblas.h"
#include "../real.h" // Tommy 2012-07-05 Moved from source_axpy_c.h to here

void
cblas_caxpy (const int N, const void *alpha, const void *X, const int incX,
             void *Y, const int incY)
{
#define BASE float
#include "source_axpy_c.h"
#undef BASE
}
