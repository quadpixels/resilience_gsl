#include <gsl/gsl_math.h>
#include <gsl/gsl_cblas.h>
#include <gsl/triplicate.h>
#include <gsl/tommy.h>
#include "cblas.h"
#include "../real.h"
#define forceinline __inline__ __attribute__((always_inline))

/*
#ifdef TEST1
struct node {
	struct node* next;
	int value;
};

struct node* g_nodestart;

void alloc_nodes(void) {
	struct node* tmp;
	struct node* prev;
	int size = sizeof(struct node);
	g_nodestart = (struct node*)malloc(size);
	g_nodestart->value = 12345;
	prev = g_nodestart;
	int i; for(i=0; i<100*100; i++) {
		tmp = (struct node*)malloc(size);
		tmp->value = i; tmp->next = NULL; prev->next = tmp; prev = tmp;
	}
}

void print_nodes() {
	struct node* tmp = g_nodestart;
	int i=0;
	do{
		tmp->value = (float)i/100*pow(i, 3);
		tmp->value = sin(123*(tmp->value)) + (float)i/20;
		tmp=tmp->next;
		if(i%2==1) printf("\n");
	}while(tmp->next!=NULL);
}
#endif
*/

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
