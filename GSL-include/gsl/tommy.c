#include "tommy.h"
#include <sys/signal.h>
#include <signal.h>

#define FT_ROUTINES_VULN
#ifdef FT_ROUTINES_VULN
#include "real.h"
#else
#include "spoof_real.h"
#endif

void check_nan(double x, char* k) { if(/*isnan(x)*/x!=x) { printf("%s is nan\n", k); }}

double my_sum_vector(const gsl_vector* v) {
	REAL_TRY(0) {
		double ret = 0;
		int i; for(i=0; i<v->size; i++) {
			ret = ret + gsl_vector_get(v, i);
		}
	return ret;
	} REAL_CATCH(0) {} REAL_END(0); 
}

double my_sum_matrix(const gsl_matrix* m) {
	REAL_TRY(0) {
		double ret = 0;
		int i,j; for(i=0; i<m->size1; i++) {
			for(j=0; j<m->size2; j++) {
				ret = ret + gsl_matrix_get(m, i, j);
			}
		}
	return ret;
	} REAL_CATCH(0) {} REAL_END(0);
}

// Deprecated; removed
/*
unsigned long CRC_Matrix(const gsl_matrix* m) {
	mycrc_reset();
	REAL_TRY(0) {
		int i, j; for(i=0; i<m->size1; i++) {
			for(j=0; j<m->size2; j++) {
				mycrc_pushd(gsl_matrix_get(m, i, j));
			}
		}
		unsigned long ret = mycrc_get();
		printf("[CRC_Matrix] CRC: %08lX\n", ret);
	return ret;
	} REAL_CATCH(0) {} REAL_END(0);
}

unsigned long CRC_Vector(const gsl_vector* v) {
	mycrc_reset();
	REAL_TRY(0) {
		int i; for(i=0; i<v->size; i++) {
			mycrc_pushd(gsl_vector_get(v, i));
		}
		unsigned long ret = mycrc_get();
		printf("[CRC_Vector] CRC: %08lX\n", ret);
		return ret;
	} REAL_CATCH(0) {} REAL_END(0);
}*/

int my_dgemm(CBLAS_TRANSPOSE_t TransA, CBLAS_TRANSPOSE_t TransB, double alpha, const gsl_matrix* A, const gsl_matrix* B, double beta, gsl_matrix* C) {
		int m, n, p; // A is m*n, B is n*p, C is m*p
		int i, j, k;
		int szA1, szA2, szB1, szB2, szC1, szC2;
		if(TransA == CblasNoTrans) { szA1=A->size1; szA2=A->size2; }
		else if(TransA == CblasTrans) { szA1=A->size2; szA2=A->size1; }
		if(TransB == CblasNoTrans) { szB1=B->size1; szB2=B->size2; }
		else if(TransB == CblasTrans) { szB1=B->size2; szB2=B->size1; }
	REAL_TRY(0) {
		szC1 = C->size1; szC2 = C->size2;
	} REAL_CATCH(0) {} REAL_END(0);
		if(szA2!=szB1) { printf("[my_dgemm] Error: A's column count != B's row count.\n"); return -1; }
		if(szA1!=szC1) { printf("[my_dgemm] Error: A's row count != C's row count.\n");    return -1; }
		if(szB2!=szC2) { printf("[my_dgemm] Error: B's column count != C's column count.\n"); return -1; }
		m=szA1; n=szA2; p=szB2;
		double temp, elemA, elemB, elemC;
		for(i=0; i<m; i++) {
			for(j=0; j<p; j++) {
				temp=gsl_matrix_get(C, i, j) * beta;
				for(k=0; k<n; k++) {
					if(TransA == CblasNoTrans) elemA=gsl_matrix_get(A, i, k);
					else if(TransA==CblasTrans)elemA=gsl_matrix_get(A, k, i);
					if(TransB == CblasNoTrans) elemB=gsl_matrix_get(B, k, j);
					else if(TransB==CblasTrans)elemB=gsl_matrix_get(B, j, k);
	REAL_TRY(1) {
					temp=temp + elemA*elemB*alpha;
	} REAL_CATCH(1) {} REAL_END(1);
				}
				gsl_matrix_set(C, i, j, temp);
			}
		}
		return 0;
}

int my_dgemv(CBLAS_TRANSPOSE_t Trans, double alpha, const gsl_matrix* A, const gsl_vector* X, double beta, gsl_vector* Y) {
		if(Trans==CblasNoTrans) {
			DBG(printf("A=(%lu x %lu), X=(%lu x 1), Y=(%lu x 1)\n", A->size1, A->size2, X->size, Y->size));
			if(A->size2 != X->size) { printf("my_dgemv: A's width is not equal to X's length.\n"); return -1; }
			if(A->size1 != Y->size) { printf("my_dgemv: A's height is not equal to Y's length.\n"); return -1; }
		} else if(Trans==CblasTrans) {
			DBG(printf("A=(%lu x %lu), X=(%lu x 1), Y=(%lu x 1)\n", A->size1, A->size2, X->size, Y->size));
			if(A->size1 != X->size) { printf("my_dgemv: A_transposed's width is not equal to X's length.\n"); return -1; }
			if(A->size2 != Y->size) { printf("my_dgemv; A_transposed's height is not equal to Y's length.\n"); return -1; }
		}
		int i,j;
		double tmp=0.0, elemA, elemX, elemY;
		for(i=0; i<Y->size; i++) {
			elemY = gsl_vector_get(Y, i);
	//		if(elemY > 1000000000) printf("Large elemY! %d,%d ", i, j);
			tmp = beta * elemY;
			for(j=0; j<X->size; j++) {
				elemX = gsl_vector_get(X, j);
				if(Trans==CblasNoTrans) { elemA = gsl_matrix_get(A, i, j);}
				else if(Trans==CblasTrans) { elemA = gsl_matrix_get(A, j, i); }
	REAL_TRY(0) {
				tmp = tmp + alpha * elemX * elemA; 
	} REAL_CATCH(0) {} REAL_END(0);
	//			if(elemX > 1000000000) printf("Large elemX! %d,%d ", i, j);
	//			if(elemA > 1000000000) printf("Large elemA! %d,%d ", i, j);
	//			if(tmp   > 1000000000) printf("Large tmp! %d,%d ", i, j);
			}
			gsl_vector_set(Y, i, tmp);
		}
}

int my_dgemv_upperlower(CBLAS_UPLO_t uplo, CBLAS_TRANSPOSE_t Trans, double alpha, const gsl_matrix* A, const gsl_vector* X, double beta, gsl_vector* Y) {
	if(A->size1 != A->size2) { printf("my_dgemv_upperlower: A is not square!\n"); return -1; }
	if(Trans==CblasNoTrans) {
		if(A->size2 != X->size) { printf("my_dgemv: A's width is not equal to X's length.\n"); return -1; }
		if(A->size1 != Y->size) { printf("my_dgemv: A's height is not equal to Y's length.\n"); return -1; } } else if(Trans==CblasTrans) {
		if(A->size1 != X->size) { printf("my_dgemv: A_transposed's width is not equal to X's length.\n"); return -1; }
		if(A->size2 != Y->size) { printf("my_dgemv; A_transposed's height is not equal to Y's length.\n"); return -1; }
	}
	int i,j;
	double tmp=0, tmp1;
	for(i=0; i<Y->size; i++) {
		tmp = beta * gsl_vector_get(Y, i);
		int start=0, end=X->size-1;
		if(uplo==CblasUpper) start=i;
		else if(uplo==CblasLower) end=i;
		for(j=0; j<end; j++) {
			if(Trans==CblasNoTrans) tmp1 = alpha * gsl_vector_get(X, j) * gsl_matrix_get(A, i, j);
			else if(Trans==CblasTrans) tmp1 = alpha * gsl_vector_get(X, j) * gsl_matrix_get(A, j, i);
REAL_TRY(0) {
			tmp = tmp + tmp1;
} REAL_CATCH(0) {} REAL_END(0);
		}
		gsl_vector_set(Y, i, tmp);
	}

}



void my_remove_SIGSEGV_handler() {
	printf(" >> Unsetted SIGSEGV handler.\n");
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGBUS,  &sa, NULL);
	printf(" >> Unsetted.\n");
}

void my_set_SIGSEGV_handler(void) {
	printf(" >> My SIGSEGV Handler 1 - rerun routine engaged!\n");
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = my_action;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGBUS,  &sa, NULL);
}

double Matrix_Sum(const gsl_matrix* M) {
	double s=0; int i, j, rows=M->size1, cols=M->size2;
//	printf(" >> Matrix_Sum, size: %d x %d, addr:%x\n", rows, cols, M);
	for(i=0; i<rows; i++) {
		for(j=0; j<cols; j++) {
			s = s + gsl_matrix_get(M, i, j);
		}
	}
	return s;
}

void my_copy_double_to_matrix(double* arr, gsl_matrix* M) {
	int i,j,k;
	for(i=0; i<M->size1; i++) {
		for(j=0; j<M->size2; j++) {
			k=i*(M->size2) + j;
			gsl_matrix_set(M, i, j, arr[k]);
		}
	}
}


void Print_Matrix(CBLAS_TRANSPOSE_t Trans, const gsl_matrix* M, const char* info) {
	int i,j;
	printf("Print_Matrix");
	if(Trans==CblasNoTrans) { printf(">> Matrix [%s] (%d x %d)\n", info, (int)M->size1, (int)M->size2); 
		for(i=0; i<M->size1; i++) {
			for(j=0; j<M->size2; j++) { printf("%g ", gsl_matrix_get(M, i, j)); }
			printf("\n");
		}
	}
	else { printf(">> Matrix [%s] (%d x %d)\n", info, (int)M->size2, (int)M->size1); 
		for(j=0; j<M->size2; j++) {
			for(i=0; i<M->size1; i++) { printf("%g ", gsl_matrix_get(M, i, j)); }
			printf("\n");
		}
	}
}

void Print_Vector(const gsl_vector* V, const char* info) {
	printf(">> Vector [%s] (%lu x 1)\n", info, V->size); 
	int i;
	for(i=0; i<V->size; i++) printf("%g ", gsl_vector_get(V,i));
	printf("\n");
	return;
}

#define STOPWATCH_INCREMENT(id) \
	if(g_ptv[id]==NULL) g_ptv[id]=&g_tvTick[id]; \
	struct timeval* ptv_late  = g_ptv[id]; \
	struct timeval* ptv_early = (g_ptv[id]==&g_tvTick[id]) ? &g_tvTock[id] : &g_tvTick[id]; \
	g_ptv[id] = ptv_early; \
	gettimeofday(ptv_late, NULL); \
	if(ptv_early->tv_usec != 0 && ptv_early->tv_sec != 0){ \
		unsigned long deltaMicros = 1000000*(ptv_late->tv_sec - ptv_early->tv_sec) + ptv_late->tv_usec - ptv_early->tv_usec; \
		g_time[id] += deltaMicros; \
	}


int Is_GSL_Vector_Equal(const gsl_vector* A, const gsl_vector* B) /* Why? gsl_vector_equal is not available. */
{
	if(A->size!=B->size) { printf("Vector A and B size not equal.\n"); return 0;}
	double a, b;
	int i; for(i=0; i<A->size; i++) {
		a=gsl_vector_get(A, i); b=gsl_vector_get(B, i);
		if(a==b) continue; // 04-19-12 Could this be the performance culprit ...
		double ab = a-b;
		double r = (double)ab/(double)b; if(r<0.0) r=r*-1.0;
		if(r > FT_TOLERANCE) { // Changed on 04-20-2012
			printf(" A[%d]!=B[%d], %f and %f, a/b=%f, a-b=%f\n", i, i, a, b, r, ab);
			return 0; }
	}
	return 1;
}

int Is_GSL_Matrix_Equal(const gsl_matrix* A, const gsl_matrix* B)
{
	int i, j;
	double aa, bb;
	if(!((A->size1 == B->size1)&&(A->size2 == B->size2))) { 
		DBG(printf("[Is_Matrix_Equal] Matrices A and B are of different dimensions!\n"));
		DBG(printf("                  A:%lu x %lu; B:%lu x %lu\n", A->size1, A->size2, B->size1, B->size2));
		return 0;
	}
	for(i=0; i<A->size1; i++) {
		for(j=0; j<A->size2; j++) {
			aa = gsl_matrix_get(A, i, j);
			bb = gsl_matrix_get(B, i, j);
			if(aa==bb) continue; // 04-19-12 Could this be the performance culprit ...
			double ab = aa-bb;
			double r = (double)ab/(double)bb; if(r<0) r=r*-1;
			if(r > FT_TOLERANCE) { 
			DBG(printf(" A[%d,%d]!=B[%d,%d], %g and %g, a/b=%f\n", i, j, i, j, aa, bb, r));
			return 0; }
		}
	}
	return 1;
}

/* Validating if C2=A*B+C. Alpha and Beta both need be set to 1. */
/* Returns 1 for equal and 0 for not equal. */
int Is_GSL_MM_Equal(CBLAS_TRANSPOSE_t TransA, CBLAS_TRANSPOSE_t TransB,
		    double alpha, const gsl_matrix* A, const gsl_matrix* B,
		    double beta,  const gsl_matrix* C, const gsl_matrix* C2) {
REAL_TRY(0) {
	my_stopwatch_checkpoint(4);
	int A_rc = (TransA == CblasNoTrans) ? A->size1 : A->size2;
	int A_cc = (TransA == CblasNoTrans) ? A->size2 : A->size1;
	int B_rc = (TransB == CblasNoTrans) ? B->size1 : B->size2;
	int B_cc = (TransB == CblasNoTrans) ? B->size2 : B->size1;
	int C_rc = C->size1;
	int C_cc = C->size2;
	if(!((A_cc==B_rc)&&(A_rc==C_rc))) {
		printf("Size a != size b!\n"); 
		my_stopwatch_stop(4);
		return 0; }
	srand(time(0));
	gsl_vector* r = gsl_vector_alloc(A_rc);
	gsl_vector* C2r, *ABCr;
	C2r=gsl_vector_alloc(A_rc); ABCr=gsl_vector_alloc(A_rc);
	int i;
	for(i=0; i<A_rc; i++) gsl_vector_set(r, i, (float)rand()/(float)RAND_MAX + 5.0f);
//	gsl_blas_dgemv(CblasNoTrans, 1, B, r,    0, ABCr); // ABCr = Br
	my_dgemv(TransB, alpha, B, r, 0, ABCr);
//	gsl_blas_dgemv(CblasNoTrans, 1, A, ABCr, 0, C2r); // ABCr = ABr
	my_dgemv(TransA, 1, A, ABCr, 0, C2r);
	gsl_vector_memcpy(ABCr, C2r);
//	gsl_blas_dgemv(CblasNoTrans, 1, C, r,    1, ABCr); // ABCr = ABr + Cr
	my_dgemv(CblasNoTrans, beta, C, r, 1, ABCr);
	gsl_vector_set_zero(C2r);
//	gsl_blas_dgemv(CblasNoTrans, 1, C2,r,    0, C2r ); // C2r  = C2r
	my_dgemv(CblasNoTrans, 1, C2, r, 0, C2r);
	int result = Is_GSL_Vector_Equal(C2r, ABCr);
//	Print_Vector(ABCr, "ABCr");
//	Print_Vector(C2r,  "C2r");
	gsl_vector_free(C2r); gsl_vector_free(ABCr); gsl_vector_free(r);
	my_stopwatch_stop(4);
	if(result==0) printf("## DGEMM not equal\n");
	return result;
} REAL_CATCH(0) {} REAL_END(0);
}

/* Validating if C2=A*t(A)+B. Same as MM. */
int Is_GSL_DSYRK_Equal(const CBLAS_UPLO_t uplo, CBLAS_TRANSPOSE_t trans,
		       double alpha, const gsl_matrix* A,
		       double beta,  const gsl_matrix* C, const gsl_matrix* C2) {
	my_stopwatch_checkpoint(4);
	int i, j, len;
	if(C->size1 != C->size2) { printf("[Is_GSL_DSYRK_Equal] Matrix C is not square!\n"); 
		my_stopwatch_stop(4);
		return 0; }
	len = C->size1;
	printf("[Is_GSL_DSYRK_Equal]\n");
	gsl_matrix* tA = gsl_matrix_alloc(A->size2, A->size1);
	gsl_matrix* uploC = gsl_matrix_alloc(C->size1, C->size2);
	gsl_matrix_memcpy(uploC, C);
	gsl_matrix_transpose_memcpy(tA, A);

	gsl_matrix* AAt = gsl_matrix_alloc(len, len);
REAL_TRY(0) {
	if(trans==CblasNoTrans) {
//		gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, A, tA, 0.0, AAt); // I think this causes problems
		my_dgemm(CblasNoTrans, CblasNoTrans, 1.0, A, tA, 0.0, AAt);
	} else {
//		gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, tA, A, 0.0, AAt);
		my_dgemm(CblasNoTrans, CblasNoTrans, 1.0, tA, A, 0.0, AAt);
	}
} REAL_CATCH(0) {} REAL_END(0);
	if(uplo == CblasUpper) {
		for(i=1; i<len; i++) {
			for(j=0; j<i; j++) {
//				gsl_matrix_set(uploC, i, j, 0);
				gsl_matrix_set(AAt,   i, j, 0);
			}
		}
	} else if(uplo == CblasLower) {
		for(i=0; i<len; i++) {
			for(j=i+1; j<C->size1; j++) {
//				gsl_matrix_set(uploC, i, j, 0);
				gsl_matrix_set(AAt,   i, j, 0);
			}
		}
	}
	gsl_matrix_add(uploC, AAt);
	int result = Is_GSL_Matrix_Equal(uploC, C2);
	gsl_matrix_free(uploC);
	gsl_matrix_free(AAt);
	my_stopwatch_stop(4);
	if(result==0) printf("## DSYRK not equal\n");
	return result;
}

int Is_GSL_DSYRK_Equal2(const CBLAS_UPLO_t uplo, CBLAS_TRANSPOSE_t trans,
		       double alpha, const gsl_matrix* A,
		       double beta,  const gsl_matrix* C, const gsl_matrix* C2)
{
	printf("[Is_GSL_DSYRK_Equal2]\n");
	my_stopwatch_checkpoint(4);
	if(C2->size1 != C2->size2) { printf("[Is_GSL_DSYRK_Equal2] C2 is not square. \n"); return 0; }
	int n = C2->size1, i, j;
	if(trans==CblasNoTrans) {
		if(!((C->size1==n) && (C->size2==n) && (A->size1==n))) { printf("[Is_GSL_DSYRK_Equal2] Dimensions do not agree.\n"); return 0; }
	} else {
		if(!((C->size1==n) && (C->size2==n) && (A->size2==n))) { printf("[Is_GSL_DSYRK_Equal2] Dimensions do not agree.\n"); return 0; }
	}

	gsl_matrix* c;
	if(beta!=0) {
		c = gsl_matrix_alloc(n, n);
		for(i=0; i<n; i++) {
			for(j=0; j<n; j++) {gsl_matrix_set(c, i, j, gsl_matrix_get(C2, i, j) - beta * gsl_matrix_get(C, i, j));}
		}
	}
	int ret;
REAL_TRY(0) {
	if(beta != 0) {
		if(uplo==CblasUpper) ret = check_aat_triangle(c, uplo, 0, n-1, n, alpha, A, trans);
		if(uplo==CblasLower) ret = check_aat_triangle(c, uplo, n-1, 0, n, alpha, A, trans);
		gsl_matrix_free(c);
	} else {
		if(uplo==CblasUpper) ret = check_aat_triangle(C2, uplo, 0, n-1, n, alpha, A, trans);
		if(uplo==CblasLower) ret = check_aat_triangle(C2, uplo, n-1, 0, n, alpha, A, trans);	
	}
} REAL_CATCH(0) {} REAL_END(0);
	my_stopwatch_stop(4);
	if(ret==0) printf("## DSYRK not equal\n");
	return ret;
}

/* Check if Y2 = A * X */
int Is_GSL_DGEMV_Equal(CBLAS_TRANSPOSE_t Trans, double alpha, const gsl_matrix* A, const gsl_vector* X, double beta, const gsl_vector* Y, const gsl_vector* Y2) {
	my_stopwatch_checkpoint(4);
	printf(" >> \n");
	if(X==NULL) printf("X is null\n");
	if(Y2==NULL) printf("Y2 is null\n");
	if(Y->size != Y2->size) { printf("[Is_GSL_DGEMV_Equal]Error: X's size != Y's size\n");
		my_stopwatch_stop(4);
		return 0; 
	}
	int i, j, result;
	double sum1=0, sum2=0;
REAL_TRY(0) {
	if(beta!=0) {
		for(i=0; i<Y->size; i++) sum1 = sum1 - gsl_vector_get(Y, i);
		sum1 = sum1 * beta;
	}
	for(i=0; i<X->size; i++) {
		double currRS = 0; /* current Row Sum */
		int jj = (Trans==CblasNoTrans) ? A->size1 : A->size2;
		for(j=0; j<jj; j++) {
			if(Trans==CblasNoTrans) currRS = currRS + gsl_matrix_get(A, j, i);
			else currRS = currRS + gsl_matrix_get(A, i, j);
		}
		sum1 = sum1 + currRS * gsl_vector_get(X, i);
	}
	for(i=0; i<Y->size; i++) { sum2 = sum2 + gsl_vector_get(Y2, i); }
	sum1 = sum1 - sum2;
	sum1 = sum1 / sum2; if(sum1 < 0) sum1 = -sum1;
	if(sum1 < FT_TOLERANCE /** X->size*/) result = 1; /* Precision problem!!! */
	else result = 0;
	my_stopwatch_stop(4);
	if(result==0) printf("## DGEMV not equal\n");
	return result;
} REAL_CATCH(0) {} REAL_END(0);
}

/* Check dtrsv routine, should be inv(op(A)) * X2 == X */
int Is_GSL_DTRSV_Equal(CBLAS_UPLO_t Uplo, CBLAS_TRANSPOSE_t TransA, CBLAS_DIAG_t diag, const gsl_matrix* A, const gsl_vector* X, const gsl_vector* X2) {
	my_stopwatch_checkpoint(4);
	if(A->size1 != A->size2) { printf("[Is_GSL_DTRSV_Equal] Matrix A is not square.\n"); return -1; }
	if(A->size1 != X->size)  { printf("[Is_GSL_DTRSV_Equal] Sizes of A and X don't match.\n"); return -1; }
	if(X->size  != X2->size) { printf("[Is_GSL_DTRSV_Equal] Sizes of X and X2 don't match.\n");return -1; }
	gsl_matrix* AA = gsl_matrix_alloc(A->size1, A->size2);
	gsl_matrix_memcpy(AA, A);
	int i,j;
	if(Uplo==CblasUpper) {
		for(i=1; i<A->size1; i++) {
			for(j=0; j<i; j++) gsl_matrix_set(AA, i, j, 0.0);
		}
	} else if(Uplo==CblasLower) {
		for(i=0; i<A->size1-1; i++) {
			for(j=i+1; j<A->size1; j++) gsl_matrix_set(AA, i, j, 0.0);
		}
	}
	if(diag==CblasUnit) { for(i=0; i<A->size1; i++) gsl_matrix_set(AA, i, i, 1.0); }
	gsl_vector* XX = gsl_vector_alloc(X->size);
	gsl_vector_set_zero(XX);
//	Print_Vector(X,  "[DTRSV Checker]X");
//	Print_Vector(X2, "[DTRSV Checker]X2");
	my_dgemv(TransA, 1.0, AA, X2, 0.0, XX);
//	Print_Vector(XX, "[DTRSV Checker]Result of MV multiplication");
	int result = Is_GSL_Vector_Equal(XX, X);
	gsl_vector_free(XX); gsl_matrix_free(AA);
	my_stopwatch_stop(4);
	if(result==0) printf("## DTRSV not equal\n");
	return result;
}

int Is_GSL_linalg_cholesky_decomp_Equal(const gsl_matrix* A, const gsl_matrix* A_out) {
	if(A->size1 != A->size2) { printf("[Is_GSL_linalg_cholesky_decomp_Equal] A is not square -- not to say symmetric.\n"); return 0; }
	if(A_out->size1 != A_out->size2) { printf("[Is_GSL_linalg_cholesky_decomp_Equal] A_out is not square -- not to say symmetric.\n"); return 0; }
	if(A->size1 != A_out->size1) { printf("[Is_GSL_linalg_cholesky_decomp_Equal] A and A_out's dimensions do not agree.\n"); return 0; }
	int n = A->size1, i, j, k;

//	my_dgemm(CblasNoTrans, CblasTrans, 1.0, L, L, 0.0, LU); // This is too slow. May take up to 100x time of decomposition!
//	gsl_blas_dgemm(CblasNoTrans, CblasTrans, 1.0, L, L, 0.0, LU);  // Confirmed. Executing this on my laptop with an OPTIMIZED gsl library reduces overhead.
								       // 20x faster than unoptimized -- but still not fast enough.
REAL_TRY(0) {
	int ret = 1;
	for(i=0; i<n; i++) {
		for(j=0; j<=i; j++) {
			double tmp = 0;
			double tmp0 = gsl_matrix_get(A, i, j);
			for(k=0; k<=j; k++) {
				tmp = tmp + gsl_matrix_get(A_out, i, k)*gsl_matrix_get(A_out, j, k);
			}
			if(tmp == tmp0) continue;
			else {
				tmp=tmp-tmp0; double r = tmp/tmp0; if(r<0) r=r*-1.0;
				if(r > FT_TOLERANCE) { ret = 0; goto re; }
			}
		}
	}
	re:
	if(ret==0) printf("## Cholesky_Decomp not equal\n");
	return ret;
} REAL_CATCH(0) {} REAL_END(0);
}

int check_aat_triangle(const gsl_matrix* C, CBLAS_UPLO_t uplo, int row, int col, int len, double alpha, const gsl_matrix* A, CBLAS_TRANSPOSE_t trans) {
REAL_TRY(0) {
	DBG(printf("check_aat_triangle [%d, %d] len=%d\n", row, col, len));
	/* Let's say, we have a "body" and two "flanks" */
	int n = C->size1;
	if(C->size1 != C->size2) { printf("C is not square.\n"); return 0; }
	int body_rs, body_re, body_cs, body_ce, body_len;
	int uflank_row, uflank_col, dflank_row, dflank_col, flank_len;
	if(len <= 2) {
		if(len==1) {
			int ret = check_aat_elem(C, row, col, alpha, A, trans);
			return ret;
		} else {
			int ret;
			ret = check_aat_elem(C, row, col, alpha, A, trans); if(ret==0) return 0;
			if(uplo==CblasUpper) {
				ret = check_aat_elem(C, row, col-1, alpha, A, trans); if(ret==0) return 0;
				ret = check_aat_elem(C, row+1, col, alpha, A, trans); if(ret==0) return 0;
				return 1;
			} else {
				ret = check_aat_elem(C, row, col+1, alpha, A, trans); if(ret==0) return 0;
				ret = check_aat_elem(C, row-1, col, alpha, A, trans); if(ret==0) return 0;
				return 1;
			}
		}
	} else {
		body_len = (int)((len+1) / 2); flank_len = (int)(len / 2);
		int ret;
		if(uplo==CblasUpper) {
			body_rs = row; body_re = row + body_len - 1;
			body_cs = col - body_len + 1; body_ce = col;
			uflank_row = row; uflank_col = body_cs - 1;
			dflank_row = body_re + 1; dflank_col = body_ce;
		} else {
			body_rs = row - body_len + 1; body_re = row;
			body_cs = col; body_ce = col + body_len - 1;
			uflank_row = body_rs - 1; uflank_col = col;
			dflank_row = body_re; dflank_col = body_ce + 1;
		}
		DBG(printf("body rows: %d ~ %d, cols: %d ~ %d, flanks at: (%d, %d) and (%d, %d)\n", body_rs, body_re, body_cs, body_ce, uflank_row, uflank_col, dflank_row, dflank_col));
		ret = check_aat_submatrix(C, body_rs, body_re, body_cs, body_ce, alpha, A, trans); if(ret==0) { DBG(printf("@")); return 0; }
		ret = check_aat_triangle(C, uplo, uflank_row, uflank_col, flank_len, alpha, A, trans); if(ret==0) { DBG(printf("#")); return 0; }
		ret = check_aat_triangle(C, uplo, dflank_row, dflank_col, flank_len, alpha, A, trans); if(ret==0) { DBG(printf("$")); return 0; }
		return 1;
	}
} REAL_CATCH(0) {} REAL_END(0);
}

#define AAT_CHECK_DIM \
	int n = C->size1; \
	if(C->size1 != C->size2) { printf("C is not square.\n"); return 0; }

int check_aat_elem(const gsl_matrix* C, int row, int col, double alpha, const gsl_matrix* A, CBLAS_TRANSPOSE_t trans) {
REAL_TRY(0) {
	AAT_CHECK_DIM; int i, j;
	double temp=0, tmp0 = gsl_matrix_get(C, row, col);
	if(trans==CblasNoTrans) {
		n=A->size2;
		for(i=0; i<A->size2; i++)  { temp = temp + gsl_matrix_get(A, row, i) * gsl_matrix_get(A, col, i) * alpha;}
	} else {
		n=A->size1;
		for(i=0; i<A->size1; i++) temp = temp + gsl_matrix_get(A, i, row) * gsl_matrix_get(A, i, col) * alpha;
	}

	if(temp != tmp0) {
		double r = (temp-tmp0)/tmp0; if(r<0.0) r=r*-1.0;
		double tolerance = FT_TOLERANCE;
		if(r > FT_TOLERANCE) {
			printf("check_aat_elem(%d, %d)=0\n", row, col);
			printf("%f vs %f\n", temp, tmp0);
			return 0; }
	}
	return 1;
} REAL_CATCH(0) {} REAL_END(0);
}

/* This is the routine to be integrated into tommy.h */
int check_aat_submatrix(const gsl_matrix* C, int rowstart, int rowend, int colstart, int colend, double alpha, const gsl_matrix* A, CBLAS_TRANSPOSE_t trans) {
REAL_TRY(0) {
	int n = C->size1;
	if(C->size1 != C->size2) { printf("C is not square.\n"); return 0; }
	gsl_vector* v = gsl_vector_alloc(n);
	int m = (trans==CblasNoTrans) ? A->size2 : A->size1;
	int i, j; for(i=0; i<n; i++) { gsl_vector_set(v, i, (double)rand()); }
	gsl_vector* aatv = gsl_vector_alloc(n), *cv=gsl_vector_alloc(n), *atv=gsl_vector_alloc(m);
	gsl_vector_set_zero(aatv); gsl_vector_set_zero(cv); gsl_vector_set_zero(atv);
	for(i=rowstart; i<=rowend; i++) {
		double tmp = 0;
		for(j=colstart; j<=colend; j++) {
			tmp = tmp + gsl_vector_get(v, j) * gsl_matrix_get(C, i, j);
		}
		gsl_vector_set(cv, i, tmp);
	}

	for(i=0; i<m; i++) {
		double tmp = 0;
		if(trans == CblasNoTrans) 
		{ for(j=colstart; j<=colend; j++) tmp=tmp+gsl_vector_get(v, j)*gsl_matrix_get(A, j, i)*alpha; }
		else
		{ for(j=colstart; j<=colend; j++) tmp=tmp+gsl_vector_get(v, j)*gsl_matrix_get(A, i, j)*alpha; }
		gsl_vector_set(atv, i, tmp);
	}
	for(i=rowstart; i<=rowend; i++) {
		double tmp = 0;
		if(trans == CblasNoTrans)
		{ for(j=0; j<m; j++) tmp=tmp+gsl_vector_get(atv, j)*gsl_matrix_get(A, i, j)*alpha; }
		else
		{ for(j=0; j<m; j++) tmp=tmp+gsl_vector_get(atv, j)*gsl_matrix_get(A, j, i)*alpha; }
		gsl_vector_set(aatv, i, tmp);
	}

	int ret = Is_GSL_Vector_Equal(aatv, cv);
	free(cv); free(aatv); free(atv); free(v);
	return ret;
} REAL_CATCH(0) {} REAL_END(0);
}


int Print_Hello(void) {
	printf("haha! ver 03-15\n");
	return 0;
}

/*
 *
 *
 *
 *
 *
 *
 *
 * Strengthened routines macro
 *
 *
 *
 *
 *
 *
 */


