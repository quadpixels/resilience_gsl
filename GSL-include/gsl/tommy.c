// 08-12: Re-write some of the FT routines. Hope it will be fault-tolerant.
#include "tommy.h"
#include "triplicate.h"
#include <sys/signal.h>
#include <signal.h>
#include "real.h"

#define FT_ROUTINES_VULN
#ifdef FT_ROUTINES_VULN
	#ifndef FT3TEST
	#define FTV_REAL_TRY(label) REAL_TRY(label)
	#define FTV_REAL_CATCH(label) REAL_CATCH(label)
	#define FTV_REAL_END(label) REAL_END(label)
	#else
		#define FTV_REAL_TRY(label) ;
		#define FTV_REAL_CATCH(label) ;
		#define FTV_REAL_END(label) ;

	#endif
#else
	#define FTV_REAL_TRY(label) ;
	#define FTV_REAL_CATCH(label) ;
	#define FTV_REAL_END(label) ;
#endif

#define noinline __attribute__((noinline))

int nonEqualCount = 0;
void check_nan(double x, char* k) { if(/*isnan(x)*/x!=x) { printf("%s is nan\n", k); }}

noinline
double my_sum_vector_actual(const gsl_vector* v) {
	double ret = 0;
	DBG(printf("[my_sum_vector] v=%lx, size=%ld, stride=%ld\n", (unsigned long)v, v->size, v->stride));
	FTV_REAL_TRY(0) {
		int i; for(i=0; i<v->size; i++) {
			ret = ret + gsl_vector_get(v, i);
		}
	} FTV_REAL_CATCH(0) {} FTV_REAL_END(0); 
	return ret;
}

noinline
double my_sum_vector(const gsl_vector* v) {
	MY_SET_SIGSEGV_HANDLER();
	int jmpret;
	SUPERSETJMP("my_sum_vector");
	if(jmpret == 0) {
		return my_sum_vector_actual(v);
	} else {
		DBG(printf("Some errors -- giving up computing my_sum_vector\n"));
		return -999;
	}
}

noinline
double my_sum_matrix_actual(const gsl_matrix* m) {
	printf("[my_sum_matrix_actual] m=%lx(%ldx%ld), m->tda=%ld, m->data=%x\n", (long)m, m->size1, m->size2, m->tda, (void*)(m->data));
	double ret = 0;
	FTV_REAL_TRY(0) {
		int i,j; for(i=0; i<m->size1; i++) {
			for(j=0; j<m->size2; j++) {
//				ret = ret + gsl_matrix_get(m, i, j);
// Since we have our own fault checker, must stop GSL's checker from being invoked.
				ret = ret + m->data[i * m->tda + j];
			}
		}
	} FTV_REAL_CATCH(0) {} FTV_REAL_END(0);
	return ret;
}

noinline 
double my_sum_matrix(const gsl_matrix* m) {
	MY_SET_SIGSEGV_HANDLER();
	int jmpret;
	SUPERSETJMP("my_sum_matrix");
	printf("[my_sum_matrix] m=%lx, m->tda=%ld, m->data=%lx\n", (long)m, m->tda, (void*)(m->data));
	return my_sum_matrix_actual(m);
}

noinline
int my_dgemm_actual(CBLAS_TRANSPOSE_t TransA, CBLAS_TRANSPOSE_t TransB, double alpha, const gsl_matrix* A, const gsl_matrix* B, double beta, gsl_matrix* C) {
		int m, n, p; // A is m*n, B is n*p, C is m*p
		int i, j, k;
		int szA1, szA2, szB1, szB2, szC1, szC2;
		if(TransA == CblasNoTrans) { szA1=A->size1; szA2=A->size2; }
		else if(TransA == CblasTrans) { szA1=A->size2; szA2=A->size1; }
		if(TransB == CblasNoTrans) { szB1=B->size1; szB2=B->size2; }
		else if(TransB == CblasTrans) { szB1=B->size2; szB2=B->size1; }
	FTV_REAL_TRY(0) {
		szC1 = C->size1; szC2 = C->size2;
	} FTV_REAL_CATCH(0) {} FTV_REAL_END(0);
		if(szA2!=szB1) { printf("[my_dgemm] Error: A's column count != B's row count.\n"); return -1; }
		if(szA1!=szC1) { printf("[my_dgemm] Error: A's row count != C's row count.\n");    return -1; }
		if(szB2!=szC2) { printf("[my_dgemm] Error: B's column count != C's column count.\n"); return -1; }
		m=szA1; n=szA2; p=szB2;
		double temp, elemA, elemB;
		for(i=0; i<m; i++) {
			for(j=0; j<p; j++) {
				temp=gsl_matrix_get(C, i, j) * beta;
				for(k=0; k<n; k++) {
					if(TransA == CblasNoTrans) elemA=gsl_matrix_get(A, i, k);
					else if(TransA==CblasTrans)elemA=gsl_matrix_get(A, k, i);
					if(TransB == CblasNoTrans) elemB=gsl_matrix_get(B, k, j);
					else if(TransB==CblasTrans)elemB=gsl_matrix_get(B, j, k);
	FTV_REAL_TRY(1) {
					temp=temp + elemA*elemB*alpha;
	} FTV_REAL_CATCH(1) {} FTV_REAL_END(1);
				}
				gsl_matrix_set(C, i, j, temp);
			}
		}
		return 0;
}

noinline int my_dgemm(CBLAS_TRANSPOSE_t TransA, CBLAS_TRANSPOSE_t TransB, double alpha, const gsl_matrix* A, const gsl_matrix* B, double beta, gsl_matrix* C) {
	MY_SET_SIGSEGV_HANDLER();
	int jmpret = sigsetjmp(buf, 1);
	trick_me_jr(jmpret);
	return my_dgemm_actual(TransA, TransB, alpha, A, B, beta, C);
}

noinline 
int my_dgemv_actual(CBLAS_TRANSPOSE_t Trans, double alpha, const gsl_matrix* A, const gsl_vector* X, double beta, gsl_vector* Y) {
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
			tmp = beta * elemY;
			for(j=0; j<X->size; j++) {
				elemX = gsl_vector_get(X, j);
				if(Trans==CblasNoTrans) { elemA = gsl_matrix_get(A, i, j);}
				else if(Trans==CblasTrans) { elemA = gsl_matrix_get(A, j, i); }
	FTV_REAL_TRY(0) {
				tmp = tmp + alpha * elemX * elemA; 
	} FTV_REAL_CATCH(0) {} FTV_REAL_END(0);
			}
			gsl_vector_set(Y, i, tmp);
		}
	return 1;
}

noinline
int my_dgemv(CBLAS_TRANSPOSE_t Trans, double alpha, const gsl_matrix* A, const gsl_vector* X, double beta, gsl_vector* Y) {
	MY_SET_SIGSEGV_HANDLER();
	int jmpret = sigsetjmp(buf, 1);
	trick_me_jr(jmpret);
	return my_dgemv_actual(Trans, alpha, A, X, beta, Y);
}

noinline 
int my_dgemv_upperlower_actual(CBLAS_UPLO_t uplo, CBLAS_TRANSPOSE_t Trans, double alpha, const gsl_matrix* A, const gsl_vector* X, double beta, gsl_vector* Y) {
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
FTV_REAL_TRY(0) {
			tmp = tmp + tmp1;
} FTV_REAL_CATCH(0) {} FTV_REAL_END(0);
		}
		gsl_vector_set(Y, i, tmp);
	}
	return 1;
}

noinline
int my_dgemv_upperlower(CBLAS_UPLO_t uplo, CBLAS_TRANSPOSE_t Trans, double alpha, const gsl_matrix* A, const gsl_vector* X, double beta, gsl_vector* Y) {
	MY_SET_SIGSEGV_HANDLER();
	int jmpret = sigsetjmp(buf, 1);
	trick_me_jr(jmpret);
	return my_dgemv_upperlower_actual(uplo, Trans, alpha, A, X, beta, Y);
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
	DBG(printf("[Is_Matrix_Equal] A:%lu x %lu; B:%lu x %lu\n", A->size1, A->size2, B->size1, B->size2));
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

double Get_GSL_Matrix_RMSD(const gsl_matrix* A, const gsl_matrix* B) {
	int s1, s2;
	s1 = A->size1; s2 = A->size2; // Pretend A and B are of same size
	if(!((A->size1 == B->size1) && (A->size2 == B->size2))) {
		printf("[Get_GSL_Matrix_RMSD] A's size (%ldx%ld) != B's size(%ldx%ld).\n",
		A->size1, A->size2, B->size1, B->size2);
		s1 = (A->size1 > B->size1 ? B->size1 : A->size1);
		s2 = (A->size2 > B->size2 ? B->size2 : A->size2);
		printf("                      Assume size is %dx%d.\n", s1, s2);
	}
	
	double rsd = 0;
	int i, j; for(i=0; i<s1; i++) {
		for(j=0; j<s2; j++) {
			double a_elem = gsl_matrix_get(A, i, j);
			double b_elem = gsl_matrix_get(B, i, j);
			a_elem -= b_elem;
			rsd += (a_elem * a_elem);
		}
	}
	rsd *= (1.0 / s1 / s2);
	return rsd;
}

double Get_GSL_Vector_RMSD(const gsl_vector* X, const gsl_vector* Y) {
	int s;
	s = X->size;
	if(X->size != Y->size) {
		printf("[Get_GSL_Vector_RMSD] X's size (%ld) != Y's size (%ld).\n",
			X->size, Y->size);
		s = (X->size > Y->size) ? Y->size : X->size;
		printf("                      Assume size is %d.\n", s);
	}
	double rsd = 0;
	int i;
	for(i=0; i<s; i++) {	
		double x_elem = gsl_vector_get(X, i);
		double y_elem = gsl_vector_get(Y, i);
		x_elem -= y_elem;
		rsd += x_elem * x_elem;
	}
	rsd *= (1.0 / s);
	return rsd;
}

/* Validating if C2=A*B+C. Alpha and Beta both need be set to 1. */
/* Returns 1 for equal and 0 for not equal. */
noinline 
int Is_GSL_MM_Equal_actual(CBLAS_TRANSPOSE_t TransA, CBLAS_TRANSPOSE_t TransB,
		    double alpha, const gsl_matrix* A, const gsl_matrix* B,
		    double beta,  const gsl_matrix* C, const gsl_matrix* C2) {
	int result;
FTV_REAL_TRY(0) {
	my_stopwatch_checkpoint(4);
	int A_rc = (TransA == CblasNoTrans) ? A->size1 : A->size2;
	int A_cc = (TransA == CblasNoTrans) ? A->size2 : A->size1;
	int B_rc = (TransB == CblasNoTrans) ? B->size1 : B->size2;
	int C_rc = C->size1;
	int C_cc = C->size2;
	if(!((A_cc==B_rc)&&(A_rc==C_rc))) {
		printf("Size a != size b!\n"); 
		my_stopwatch_stop(4);
		return 0; }
	srand(time(0));
	gsl_vector* r = gsl_vector_alloc(C_cc);
	gsl_vector* C2r, *Br, *ABr;
	C2r = gsl_vector_alloc(A_rc);
	Br = gsl_vector_alloc(B_rc);
	ABr = gsl_vector_alloc(A_rc);
	printf("sizeof r, C2r, Br, ABr: %ld %ld %ld %ld\n", r->size, C2r->size, Br->size, ABr->size);
	int i;
	for(i=0; i<r->size; i++) gsl_vector_set(r, i, (float)rand()/(float)RAND_MAX + 5.0f);
	my_dgemv(TransB, alpha, B, r, 0, Br);  // Br <- B * r
	my_dgemv(TransA, 1, A, Br, 0, ABr);    // ABr <- A * B * r
	my_dgemv(CblasNoTrans, beta, C, r, 1, ABr); // ABr <- ABr + C * r
	my_dgemv(CblasNoTrans, 1, C2, r, 0, C2r);
	result = Is_GSL_Vector_Equal(C2r, ABr);
	gsl_vector_free(C2r);
	gsl_vector_free(Br);
	gsl_vector_free(ABr);
	gsl_vector_free(r);
	my_stopwatch_stop(4);
	if(result==0) printf("## DGEMM not equal\n");
} FTV_REAL_CATCH(0) {} FTV_REAL_END(0);
	return result;
}

noinline
int Is_GSL_MM_Equal(CBLAS_TRANSPOSE_t TransA, CBLAS_TRANSPOSE_t TransB,
		    double alpha, const gsl_matrix* A, const gsl_matrix* B,
		    double beta,  const gsl_matrix* C, const gsl_matrix* C2) {
	MY_SET_SIGSEGV_HANDLER();
	int jmpret;
	SUPERSETJMP("Is_GSL_MM_Equal");
	trick_me_jr(jmpret);
	return Is_GSL_MM_Equal_actual(TransA, TransB, alpha, A, B, beta, C, C2);
}

/* Validating if C2=A*t(A)+B. Same as MM. */
noinline 
int Is_GSL_DSYRK_Equal_actual(const CBLAS_UPLO_t uplo, CBLAS_TRANSPOSE_t trans,
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
FTV_REAL_TRY(0) {
	if(trans==CblasNoTrans) {
//		gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, A, tA, 0.0, AAt); // I think this causes problems
		my_dgemm(CblasNoTrans, CblasNoTrans, 1.0, A, tA, 0.0, AAt);
	} else {
//		gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, tA, A, 0.0, AAt);
		my_dgemm(CblasNoTrans, CblasNoTrans, 1.0, tA, A, 0.0, AAt);
	}
} FTV_REAL_CATCH(0) {} FTV_REAL_END(0);
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

noinline
int Is_GSL_DSYRK_Equal(const CBLAS_UPLO_t uplo, CBLAS_TRANSPOSE_t trans,
		       double alpha, const gsl_matrix* A,
		       double beta,  const gsl_matrix* C, const gsl_matrix* C2) {
	MY_SET_SIGSEGV_HANDLER();
	return Is_GSL_DSYRK_Equal_actual(uplo, trans, alpha, A, beta, C, C2);
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
FTV_REAL_TRY(0) {
	if(beta != 0) {
		if(uplo==CblasUpper) ret = check_aat_triangle(c, uplo, 0, n-1, n, alpha, A, trans);
		if(uplo==CblasLower) ret = check_aat_triangle(c, uplo, n-1, 0, n, alpha, A, trans);
		gsl_matrix_free(c);
	} else {
		if(uplo==CblasUpper) ret = check_aat_triangle(C2, uplo, 0, n-1, n, alpha, A, trans);
		if(uplo==CblasLower) ret = check_aat_triangle(C2, uplo, n-1, 0, n, alpha, A, trans);	
	}
} FTV_REAL_CATCH(0) {} FTV_REAL_END(0);
	my_stopwatch_stop(4);
	if(ret==0) printf("## DSYRK not equal\n");
	return ret;
}

/* Check if Y2 = A * X */
noinline 
int Is_GSL_DGEMV_Equal_actual(CBLAS_TRANSPOSE_t Trans, double alpha, const gsl_matrix* A, const gsl_vector* X, double beta, const gsl_vector* Y, const gsl_vector* Y2) {
	/* Y2 is the output alphaAX+betaY */
	my_stopwatch_checkpoint(4);
	printf(" >> \n");
	if(X==NULL) printf("X is null\n");
	if(Y2==NULL) printf("Y2 is null\n");
	if(Y->size != Y2->size) { printf("[Is_GSL_DGEMV_Equal]Error: X's size != Y's size\n");
		my_stopwatch_stop(4);
		return 0; 
	}
	int i, j, result;
	double sum1_1=0, sum1=0, sum2=0;
	double abserr, relerr;
FTV_REAL_TRY(0) {
	if(beta!=0) {
		for(i=0; i<Y->size; i++) sum1 = sum1 + gsl_vector_get(Y, i);
		sum1 = sum1 * beta;
	}
	for(i=0; i<X->size; i++) {
		double currCS = 0; /* current column Sum */
		int jj = (Trans==CblasNoTrans) ? A->size1 : A->size2;
		for(j=0; j<jj; j++) {
			if(Trans==CblasNoTrans) currCS = currCS + gsl_matrix_get(A, j, i);
			else currCS = currCS + gsl_matrix_get(A, i, j);
		}
		sum1_1 = sum1_1 + currCS * gsl_vector_get(X, i);
	}
	sum1_1 *= alpha;
	sum1 += sum1_1;
	for(i=0; i<Y->size; i++) { sum2 = sum2 + gsl_vector_get(Y2, i); }
	abserr = sum1 - sum2; if(abserr < 0) abserr = -abserr;
	relerr = abserr / sum2; if(relerr < 0) relerr = -relerr;
	const double tol = FT_TOLERANCE / 100;
	printf("[Is_GSL_DGEMV_Equal] DGEMV Equal, abs.err=%g, rel.err=%g (tol=%g)\n", abserr, relerr, tol);
	if(abserr > tol && relerr > tol) {
		result = 0; /* Precision problem!!! */
	}
	else result = 1;
	my_stopwatch_stop(4);
	if(result==0) printf("[Is_GSL_DGEMV_Equal] DGEMV not equal\n");
} FTV_REAL_CATCH(0) {} FTV_REAL_END(0);
	return result;
}

noinline
int Is_GSL_DGEMV_Equal(CBLAS_TRANSPOSE_t Trans, double alpha, const gsl_matrix* A, const gsl_vector* X, double beta, const gsl_vector* Y, const gsl_vector* Y2) {
	MY_SET_SIGSEGV_HANDLER();
	return Is_GSL_DGEMV_Equal_actual(Trans, alpha, A, X, beta, Y, Y2);
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

noinline 
int Is_GSL_linalg_cholesky_decomp_Equal_actual(const gsl_matrix* A, const gsl_matrix* A_out) {
	if(A->size1 != A->size2) { printf("[Is_GSL_linalg_cholesky_decomp_Equal] A is not square -- not to say symmetric.\n"); return 0; }
	if(A_out->size1 != A_out->size2) { printf("[Is_GSL_linalg_cholesky_decomp_Equal] A_out is not square -- not to say symmetric.\n"); return 0; }
	if(A->size1 != A_out->size1) { printf("[Is_GSL_linalg_cholesky_decomp_Equal] A and A_out's dimensions do not agree.\n"); return 0; }
	int n = A->size1, i, j, k;

//	my_dgemm(CblasNoTrans, CblasTrans, 1.0, L, L, 0.0, LU); // This is too slow. May take up to 100x time of decomposition!
//	gsl_blas_dgemm(CblasNoTrans, CblasTrans, 1.0, L, L, 0.0, LU);  // Confirmed. Executing this on my laptop with an OPTIMIZED gsl library reduces overhead.
								       // 20x faster than unoptimized -- but still not fast enough.
	int ret = 1;
FTV_REAL_TRY(0) {
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
} FTV_REAL_CATCH(0) {} FTV_REAL_END(0);
	return ret;
}

noinline
int Is_GSL_linalg_cholesky_decomp_Equal(const gsl_matrix* A, const gsl_matrix* A_out) {
	MY_SET_SIGSEGV_HANDLER();
	return Is_GSL_linalg_cholesky_decomp_Equal_actual(A, A_out);
}

noinline 
int check_aat_triangle_actual(const gsl_matrix* C, CBLAS_UPLO_t uplo, int row, int col, int len, double alpha, const gsl_matrix* A, CBLAS_TRANSPOSE_t trans) {
FTV_REAL_TRY(0) {
	DBG(printf("check_aat_triangle [%d, %d] len=%d\n", row, col, len));
	/* Let's say, we have a "body" and two "flanks" */
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
} FTV_REAL_CATCH(0) {} FTV_REAL_END(0);
	return 0;
}

noinline
int check_aat_triangle(const gsl_matrix* C, CBLAS_UPLO_t uplo, int row, int col, int len, double alpha, const gsl_matrix* A, CBLAS_TRANSPOSE_t trans) {
	MY_SET_SIGSEGV_HANDLER();
	return check_aat_triangle_actual(C, uplo, row, col, len, alpha, A, trans);
}

#define AAT_CHECK_DIM \
	int n = C->size1; \
	if(C->size1 != C->size2) { printf("C is not square.\n"); return 0; }

noinline 
int check_aat_elem_actual(const gsl_matrix* C, int row, int col, double alpha, const gsl_matrix* A, CBLAS_TRANSPOSE_t trans) {
FTV_REAL_TRY(0) {
	AAT_CHECK_DIM; int i;
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
		if(r > FT_TOLERANCE) {
			printf("check_aat_elem(%d, %d)=0\n", row, col);
			printf("%f vs %f\n", temp, tmp0);
			return 0; }
	}
} FTV_REAL_CATCH(0) {} FTV_REAL_END(0);
	return 1;
}

int check_aat_elem(const gsl_matrix* C, int row, int col, double alpha, const gsl_matrix* A, CBLAS_TRANSPOSE_t trans) {
	MY_SET_SIGSEGV_HANDLER();
	return check_aat_elem_actual(C, row, col, alpha, A, trans); 
}

/* This is the routine to be integrated into tommy.h */
noinline 
int check_aat_submatrix_actual(const gsl_matrix* C, int rowstart, int rowend, int colstart, int colend, double alpha, const gsl_matrix* A, CBLAS_TRANSPOSE_t trans) {
	int ret;
FTV_REAL_TRY(0) {
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

	ret = Is_GSL_Vector_Equal(aatv, cv);
	free(cv); free(aatv); free(atv); free(v);
} FTV_REAL_CATCH(0) {} FTV_REAL_END(0);
	return ret;
}

noinline
int check_aat_submatrix(const gsl_matrix* C, int rowstart, int rowend, int colstart, int colend, double alpha, const gsl_matrix* A, CBLAS_TRANSPOSE_t trans) {
	MY_SET_SIGSEGV_HANDLER();
	return check_aat_submatrix_actual(C, rowstart, rowend, colstart, colend, alpha, A, trans);
}

int Print_Hello(void) {
	printf("haha! ver 03-15\n");
	return 0;
}

noinline
void GSL_BLAS_DGEMM_FT3(CBLAS_TRANSPOSE_t TransA, CBLAS_TRANSPOSE_t TransB, 
			double alpha, const gsl_matrix* matA, const gsl_matrix* matB, 
			double beta, gsl_matrix* matC)

{
	/* Protect the pointers to matrix A, B, (the original) C */
	unsigned long matA_0, matA_1, matA_2, 
	     matB_0, matB_1, matB_2,
	     matC_0, matC_1, matC_2;
	TRIPLICATE(matA, matA_0, matA_1, matA_2);
	TRIPLICATE(matB, matB_0, matB_1, matB_2);

	/* Protect the tda, size1 and size2 of matC_bak */
	size_t mcb0, mcb1, mcb2;
	size_t mcs10, mcs11, mcs12;
	size_t mcs20, mcs21, mcs22;
	size_t mbs10, mbs11, mbs12;
	size_t mbs20, mbs21, mbs22;
	size_t mas10, mas11, mas12;
	size_t mas20, mas21, mas22;
	TRIPLICATE_SIZE_T(matB->size1, mbs10, mbs11, mbs12);
	TRIPLICATE_SIZE_T(matB->size2, mbs20, mbs21, mbs22);
	TRIPLICATE_SIZE_T(matA->size1, mas10, mas11, mas12);
	TRIPLICATE_SIZE_T(matA->size2, mas20, mas21, mas22);
	
	double sumA, sumB, sumC;
	sumA=my_sum_matrix(matA); sumB=my_sum_matrix(matB); sumC=my_sum_matrix(matC); 
	/* Protect the pointer to ECC codes. */
	void* ecMatA, *ecMatB, *ecMatC, *ecMatC_2;
	unsigned long ema_0, ema_1, ema_2, emb_0, emb_1, emb_2, emc_0, emc_1, emc_2;
	
	nonEqualCount=0;
	MY_SET_SIGSEGV_HANDLER();
	gsl_matrix* matC_bak = gsl_matrix_alloc(matC->size1, matC->size2);
	/* Protect matC_bak -> data */
	unsigned long mCbd_0, mCbd_1, mCbd_2;
	TRIPLICATE(matC_bak, matC_0, matC_1, matC_2);
	TRIPLICATE(matC_bak->data, mCbd_0, mCbd_1, mCbd_2);
	gsl_matrix_memcpy(matC_bak, matC);
	TRIPLICATE_SIZE_T(matC_bak->tda, mcb0, mcb1, mcb2);
	TRIPLICATE_SIZE_T(matC_bak->size1, mcs10, mcs11, mcs12);
	TRIPLICATE_SIZE_T(matC_bak->size2, mcs20, mcs21, mcs22);
	int jmpret, isEqual;
	encode((matB->data), (matB->size1*matB->size2), &ecMatB);
	TRIPLICATE(ecMatB, emb_0, emb_1, emb_2);
	int emc_size = encode((matC_bak->data), (matC->size1*matC->size2), &ecMatC);
	ecMatC_2 = malloc(sizeof(double) * emc_size);
	memcpy(ecMatC_2, ecMatC, sizeof(double) * emc_size);
	TRIPLICATE(ecMatC, emc_0, emc_1, emc_2);
	encode((matA->data), (matA->size1*matA->size2), &ecMatA); 
	TRIPLICATE(ecMatA, ema_0, ema_1, ema_2);
	DBG(printf("[DGEMM_FT3] ECC Code Addr: A=%lx, B=%lx, C=%lx\n", (unsigned long)ecMatA, (unsigned long)ecMatB, (unsigned long)ecMatC));
	SUPERSETJMP("After encoding");
	if(jmpret != 0) {
		kk:
		/* Chasing the wind, only */
		DBG(printf("[DGEMM_FT3]Recovering input...\n"));
		{
			/* Recover ptr to matA and matA's ecc */
			TRI_RECOVER(matA_0, matA_1, matA_2);
			size_t *mas1 = &(((gsl_matrix*)matA)->size1),
			       *mas2 = &(((gsl_matrix*)matA)->size2);
			TRI_RECOVER_SIZE_T((*mas1), mas10, mas11, mas12);
			TRI_RECOVER_SIZE_T((*mas2), mas20, mas21, mas22);
			TRI_RECOVER(ema_0, ema_1, ema_2);
			if((long)ecMatA != ema_0) ecMatA = (void*)ema_0;
			MY_MAT_CHK_RECOVER_POECC(sumA, ecMatA, (gsl_matrix*)matA_0);

			/* Recover ptr to matB and matB's ecc */
			TRI_RECOVER(matB_0, matB_1, matB_2);
			size_t *mbs1 = &(((gsl_matrix*)matB)->size1),
			       *mbs2 = &(((gsl_matrix*)matB)->size2);
			TRI_RECOVER_SIZE_T((*mbs1), mbs10, mbs11, mbs12);
			TRI_RECOVER_SIZE_T((*mbs2), mbs20, mbs21, mbs22);
			TRI_RECOVER(emb_0, emb_1, emb_2);
			if((long)ecMatB != emb_0) ecMatB = (void*)emb_0;
			MY_MAT_CHK_RECOVER_POECC(sumB, ecMatB, (gsl_matrix*)matB_0);

			/* Recover ptr to mat C and matC's ecc */
			/* Mat C is special: its size1 and size2 are also 
			 * vulnerable to soft errors therefore needs extra care
			 */
			TRI_RECOVER(matC_0, matC_1, matC_2);
			TRI_RECOVER(mCbd_0, mCbd_1, mCbd_2);
			if(matC_bak->data != (double*)mCbd_0)
				matC_bak->data = (double*)mCbd_0;
			TRI_RECOVER_SIZE_T(matC_bak->size1, mcs10, mcs11, mcs12);
			TRI_RECOVER_SIZE_T(matC_bak->size2, mcs20, mcs21, mcs22);
			TRI_RECOVER_SIZE_T(matC->size1, mcs10, mcs11, mcs12);
			TRI_RECOVER_SIZE_T(matC->size2, mcs20, mcs21, mcs22);
			TRI_RECOVER_SIZE_T(matC_bak->tda, mcb0, mcb1, mcb2);
			TRI_RECOVER(emc_0, emc_1, emc_2);
			if((long)ecMatC != emc_0) ecMatC = (void*)emc_0;
			int n_retry = 0;
chk_rec_c:
			float fr = MY_MAT_CHK_RECOVER_POECC(sumC, ecMatC, (gsl_matrix*)matC_0);
			if(fr > 0.5 && n_retry < 5) {
				DBG(printf("[DGEMM_FT3] Oh! Is ecMatC damaged? (%d/%d)\n", n_retry, 5));
				memcpy(ecMatC, ecMatC_2, sizeof(double) * emc_size);
				n_retry++;
				goto chk_rec_c;
			}

			//And there we have completed recovering it....
			DBG(printf("[DGEMM_FT3]Copying back input...\n"));
			int ec;
			if((ec = gsl_matrix_memcpy(matC, matC_bak)) != GSL_SUCCESS) {
				printf("[DGEMM_FT3] ! memcpy returned %d\n", ec);
			}
		}

	}
	DBG(printf("[DGEMM_FT3]Normal call to dgemm.. nonEqualCount=%d\n", nonEqualCount));
	SW3START;
	SUPERSETJMP("Just before dgemm");
	if(jmpret != 0) {
		goto kk;
	}
	gsl_blas_dgemm(TransA, TransB, alpha, matA, matB, beta, matC);
	SW3STOP;
	DBG(printf("[DGEMM_FT3] Check Results..\n"));
	isEqual = Is_GSL_MM_Equal(TransA, TransB, alpha, matA, matB, beta, matC_bak, matC);
	if(isEqual==1) { DBG(printf("[DGEMM_FT3]Result: Equal\n")); }
	else {
		my_stopwatch_checkpoint(6);
		printf("[DGEMM_FT3]Result: NOT Equal\n");
		nonEqualCount = nonEqualCount + 1;
		if(nonEqualCount < NUM_OF_RERUN) {
			printf("[DGEMM_FT3]Restart calculation.\n");
			goto kk;
		}
	}
	my_stopwatch_stop(6);
	printf("[DGEMM_FT3]Releasing memory for mat_bak's and deleting temp files\n");
	SUPERSETJMP("Just before free");
	if(jmpret == 0) {
		TRI_RECOVER(matC_0, matC_1, matC_2);
		gsl_matrix_free((gsl_matrix*)matC_0);
		printf("Released.\n");
		// Last, restore the sizes of elements in case there are corruptions
		{
			size_t *mas1 = &(((gsl_matrix*)matA)->size1),
			       *mas2 = &(((gsl_matrix*)matA)->size2);
			TRI_RECOVER_SIZE_T((*mas1), mas10, mas11, mas12);
			TRI_RECOVER_SIZE_T((*mas2), mas20, mas21, mas22);
			size_t *mbs1 = &(((gsl_matrix*)matB)->size1),
			       *mbs2 = &(((gsl_matrix*)matB)->size2);
			TRI_RECOVER_SIZE_T((*mbs1), mbs10, mbs11, mbs12);
			TRI_RECOVER_SIZE_T((*mbs2), mbs20, mbs21, mbs22);
			TRI_RECOVER_SIZE_T(matC->size1, mcs10, mcs11, mcs12);
			TRI_RECOVER_SIZE_T(matC->size2, mcs20, mcs21, mcs22)
		}

	} else {
		printf("Oops. There is a segfault while trying to free. giving up freeing.");
	}
}

noinline
void GSL_BLAS_DGEMV_FT3(CBLAS_TRANSPOSE_t Trans, double alpha, 
			const gsl_matrix* matA, const gsl_vector* vecX, 
			double beta, gsl_vector* vecY)
{
	/* Protect pointers to matrix A, vectors X and (the original) Y */
	unsigned long matA0, matA1, matA2,
	              vecX0, vecX1, vecX2,
		      vecY0, vecY1, vecY2;
	TRIPLICATE(matA, matA0, matA1, matA2);
	TRIPLICATE(vecX, vecX0, vecX1, vecX2);

	/* Protect size_t of A, X and Y */
	size_t mas10, mas11, mas12;
	size_t mas20, mas21, mas22;
	size_t vxs0, vxs1, vxs2;
	size_t vys0, vys1, vys2;
	TRIPLICATE_SIZE_T(matA->size1, mas10, mas11, mas12);
	TRIPLICATE_SIZE_T(matA->size2, mas20, mas21, mas22);
	TRIPLICATE_SIZE_T(vecX->size,  vxs0,  vxs1,  vxs2);
	TRIPLICATE_SIZE_T(vecY->size,  vys0,  vys1,  vys2);


	MY_SET_SIGSEGV_HANDLER();
	double sumA, sumX, sumY; 
	sumA=my_sum_matrix(matA); sumX=my_sum_vector(vecX); sumY=my_sum_vector(vecY); 

	/* Protect ptrs to ECC codes. */
	void* ecMatA, *ecVecX, *ecVecY;
	unsigned long ema0, ema1, ema2, evx0, evx1, evx2, evy0, evy1, evy2;

	DBG(printf("[DGEMV_FT3] 2. Encoding input\n"));
	encode((matA->data), (matA->size1*matA->size2), &ecMatA); 
	TRIPLICATE(ecMatA, ema0, ema1, ema2);
	encode((vecX->data), (vecX->size), &ecVecX); 
	TRIPLICATE(ecVecX, evx0, evx1, evx2);
	encode((vecY->data), (vecY->size), &ecVecY); 
	TRIPLICATE(ecVecY, evy0, evy1, evy2);
	
	gsl_vector* vecY_bak = gsl_vector_alloc(vecY->size);
	gsl_vector_memcpy(vecY_bak, vecY);
	TRIPLICATE(vecY_bak, vecY0, vecY1, vecY2);

	nonEqualCount=0;
	int jmpret, isEqual;
	DBG(printf("[DGEMV_FT3] 3. Setjmp\n"));
	SUPERSETJMP("[DGEMV_FT3] After encoding\n");
	if(jmpret != 0) {
		kk: 
		DBG(printf("[DGEMV_FT3]Recovering input...\n"));
		{
		/* Recover ptr to A's ECC */
		size_t *mas1 = &(((gsl_matrix*)matA)->size1);
		size_t *mas2 = &(((gsl_matrix*)matA)->size2);
		TRI_RECOVER_SIZE_T((*mas1), mas10, mas11, mas12);
		TRI_RECOVER_SIZE_T((*mas2), mas20, mas21, mas22);
		TRI_RECOVER(matA0, matA1, matA2);
		TRI_RECOVER(ema0, ema1, ema2);
		MY_MAT_CHK_RECOVER_POECC(sumA, (void*)ema0, (gsl_matrix*)matA0);

		TRI_RECOVER(vecX0, vecX1, vecX2);
		size_t *vxs = &(((gsl_vector*)vecX)->size);
		TRI_RECOVER_SIZE_T((*vxs), vxs0, vxs1, vxs2);
		TRI_RECOVER(evx0, evx1, evx2);
		MY_VEC_CHK_RECOVER_POECC(sumX, (void*)evx0, (gsl_vector*)vecX0);

		TRI_RECOVER(vecY0, vecY1, vecY2);
		size_t *vys = &(((gsl_vector*)vecY_bak)->size);
		TRI_RECOVER_SIZE_T((*vys), vys0, vys1, vys2);
		TRI_RECOVER(evy0, evy1, evy2);
		MY_VEC_CHK_RECOVER_POECC(sumY, (void*)evy0, (gsl_vector*)vecY0);

		gsl_vector_memcpy(vecY, vecY_bak);
		} 
	}
	DBG(printf("[DGEMV_FT3]Normal call to dgemv.. nonEqualCount=%d\n", nonEqualCount));
	SW3START; 
	SUPERSETJMP("Just before DGEMV");
	if(jmpret != 0) {
		goto kk;
	}
	gsl_blas_dgemv(Trans, alpha, matA, vecX, beta, vecY);
	SW3STOP; 
	my_stopwatch_checkpoint(11); 
	isEqual = Is_GSL_DGEMV_Equal(Trans, alpha, matA, vecX, beta, vecY_bak, vecY);
	my_stopwatch_stop(11); 
	if(isEqual==1) { DBG(printf("[DGEMV_FT3]Result: Equal\n")); }
	else {
		my_stopwatch_checkpoint(6); 
		printf("[DGEMV_FT3]Result: NOT Equal\n");
		nonEqualCount = nonEqualCount + 1;
		if(nonEqualCount < NUM_OF_RERUN) {
			printf("[DGEMV_FT3]Restart calculation.\n");
			goto kk; 
		}
	}
	my_stopwatch_stop(6); 
	DBG(printf("[DGEMV_FT3]Releasing memory for vec_bak's\n"));
	SUPERSETJMP("Just before free");
	if(jmpret == 0) {
		gsl_vector_free(vecY_bak);
		DBG(printf("Released.\n"));
	} else {
		printf("Oops. Something wrong trying to free the memory. Giving up freeing.");
	}
	MY_REMOVE_SIGSEGV_HANDLER();
}

noinline
void GSL_BLAS_DSYRK_FT3(CBLAS_UPLO_t uplo, CBLAS_TRANSPOSE_t trans, 
	           double alpha, const gsl_matrix* A,
		   double beta, gsl_matrix* C)
{
	double sumA, sumC;
	sumA=my_sum_matrix(A); sumC=my_sum_matrix(C); 
	void *ecMatA, *ecMatC;
	nonEqualCount=0;
	MY_SET_SIGSEGV_HANDLER();
	gsl_matrix* C_bak = gsl_matrix_alloc(C->size1, C->size2);
	gsl_matrix_memcpy(C_bak, C);
	encode((A->data), (A->size1*A->size2), &ecMatA); 
	encode((C_bak->data), (C_bak->size1*C_bak->size2), &ecMatC); 
	int jmpret, isEqual;
	jmpret = sigsetjmp(buf, 1);
	if(jmpret!=0) {
		kk: 
		DBG(printf("[DSYRK_FT3]Recovering matrices from error correction data (jmpret=%d)\n", jmpret));
		MY_MAT_CHK_RECOVER_POECC(sumA, ecMatA, (gsl_matrix*)A);
		MY_MAT_CHK_RECOVER_POECC(sumC, ecMatC, (gsl_matrix*)C);
	}
	gsl_matrix_memcpy(C_bak, C); 
	DBG(printf("[DSYRK_FT3]Normal call to dsyrk.. nonEqualCount=%d\n", nonEqualCount));
	my_stopwatch_checkpoint(3); 
	gsl_blas_dsyrk(uplo, trans, alpha, A, beta, C);
	my_stopwatch_stop(3); 
	isEqual = Is_GSL_DSYRK_Equal2(uplo, trans, alpha, A, beta, C_bak, C);
	if(isEqual==1) { DBG(printf("[DSYRK_FT3]Result: Equal\n")); }
	else {
		my_stopwatch_checkpoint(6); 
		printf("[DSYRK_FT3]Result: NOT Equal\n");
		nonEqualCount = nonEqualCount + 1;
		if(nonEqualCount < NUM_OF_RERUN) {
			printf("[DSYRK_FT3]Restart calculation.\n");
			jmpret = 1; 
			goto kk; 
		}
	}
	my_stopwatch_stop(6); 
	DBG(printf("[DSYRK_FT3]Releasing memory for C_bak\n"));
	gsl_matrix_free(C_bak);
	DBG(printf("Released.\n"));
	MY_REMOVE_SIGSEGV_HANDLER();
}

noinline
void GSL_BLAS_DTRSV_FT3(CBLAS_UPLO_t uplo, CBLAS_TRANSPOSE_t TransA,
			CBLAS_DIAG_t Diag, const gsl_matrix* A, 
			gsl_vector* X)
{
	double sumA, sumX;
	sumA=my_sum_matrix(A); sumX=my_sum_vector(X); 
	void *ecMatA, *ecVecX;
	nonEqualCount=0;
	MY_SET_SIGSEGV_HANDLER();
	encode(A->data, (A->size1*A->size2), &ecMatA); 
	encode(X->data, (X->size), &ecVecX); 
	int jmpret=0, isEqual;
	jmpret = sigsetjmp(buf, 1);
	if(jmpret != 0) {
		kk: 
		printf("[DTRSV_FT]Recovering matrix and vector from file\n");
		MY_MAT_CHK_RECOVER_POECC(sumA, ecMatA, (gsl_matrix*)A);
		MY_VEC_CHK_RECOVER_POECC(sumX, ecVecX, (gsl_vector*)X);
	}
	gsl_vector* X_bak = gsl_vector_alloc(X->size);
	gsl_vector_memcpy(X_bak, X);
	printf("[DTRSV_FT]Normal call to dsyrk.. nonEqualCount=%d\n", nonEqualCount);
	SW3START; 
	int ret = gsl_blas_dtrsv(uplo, TransA, Diag, A, X);
	SW3STOP; 
	if(ret != GSL_SUCCESS) { printf("[DTRSV_FT3] GSL_ERROR occurred\n"); isEqual=1; /* To force the routine to quit */ } 
	else isEqual = Is_GSL_DTRSV_Equal(uplo, TransA, Diag, A, X_bak, X);
	if(isEqual==1) { DBG(printf("[DTRSV_FT]Result: Equal\n")); }
	else {
		my_stopwatch_checkpoint(6); 
		printf("[DTRSV_FT]Result: NOT Equal\n");
		nonEqualCount = nonEqualCount + 1;
		if(nonEqualCount < NUM_OF_RERUN) {
			printf("[DTRSV_FT]Restart calculation.\n");
			goto kk; 
		}
	}
	my_stopwatch_stop(6); 
	printf("[DTRSV_FT]Releasing memory for X_bak\n");
	gsl_vector_free(X_bak);
	printf("Released.\n");
	MY_REMOVE_SIGSEGV_HANDLER();
}

noinline
void GSL_LINALG_CHOLESKY_DECOMP_FT3(gsl_matrix* A)
{
	double sumA; 
	sumA=my_sum_matrix(A); 
	void* ecMatA; 
	nonEqualCount=0; 
	MY_SET_SIGSEGV_HANDLER(); 
	encode(A->data, (A->size1*A->size2), &ecMatA); 
	int jmpret = 0, isEqual; 
	jmpret = sigsetjmp(buf, 1); 
	if(jmpret != 0) {
		kk: 
		MY_MAT_CHK_RECOVER_POECC(sumA, ecMatA, A); 
	}
	gsl_matrix* A_bak = gsl_matrix_alloc(A->size1, A->size2); 
	gsl_matrix_memcpy(A_bak, A); 
	DBG(printf("[GSL_LINALG_CHOLESKY_DECOMP_FT3] normal call to cholesky_decomp. nonEqualCount=%d\n", nonEqualCount)); 
	SW3START; 
	int ret = gsl_linalg_cholesky_decomp(A); 
	SW3STOP; 
	if(ret != GSL_SUCCESS) { printf("[GSL_LINALG_CHOLESKY_DECOMP_FT3] GSL_ERROR occurrec\n"); isEqual=1; /* To force the routine to quit */} 
	else isEqual = Is_GSL_linalg_cholesky_decomp_Equal(A_bak, A); 
	if(isEqual==1) { DBG(printf("[GSL_LINALG_CHOLESKY_DECOMP_FT3] Result: Equal\n"));} 
	else {
		my_stopwatch_checkpoint(6); 
		printf("[GSL_LINALG_CHOLESKY_DECOMP_FT3] Result: NOT Equal\n"); 
		nonEqualCount = nonEqualCount+1; 
		if(nonEqualCount < NUM_OF_RERUN) {
			printf("[GSL_LINALG_CHOLESKY_DECOMP_FT3] Restart calculation.\n"); 
			goto kk; 
		} 
	} 
	my_stopwatch_stop(6); 
	gsl_matrix_free(A_bak); 
	MY_REMOVE_SIGSEGV_HANDLER(); 
}

// Taken from tommy.h
#ifndef POECC_VERSION1
// Returns how much could not be recovered (0 to 1)
noinline
float MY_MAT_CHK_RECOVER_POECC(double sum, void* ecMat, gsl_matrix* mat) {
	MY_SET_SIGSEGV_HANDLER();
	// Protect mat and ecMat
	unsigned long mat0, mat1, mat2;
	unsigned long em0, em1, em2;
	TRIPLICATE(mat, mat0, mat1, mat2);
	TRIPLICATE(ecMat, em0, em1, em2);
	
	float ret = 0;
	my_stopwatch_checkpoint(9);
	double c = my_sum_matrix(mat);
	DBG(printf("[Matrix RECOVER_POECC] Sum=%g vs %g\n", sum, c));
	if(c!=sum) {
		my_stopwatch_checkpoint(8);
		int jmpret=0;
		SUPERSETJMP("MAT_CHK_RECOVER_POECC");
		/* Recover mat and ecMat */
		TRI_RECOVER(mat0, mat1, mat2);
		if((unsigned long)mat != mat0) mat = (gsl_matrix*)mat0;
		TRI_RECOVER(em0, em1, em2);
		unsigned int failed_corr = decode(mat->data, mat->size1*mat->size2, (const double*)em0);
		int n_blks = (int)((mat->size1 * mat->size2)/BLK_LEN/BLK_LEN+1);
		DBG(printf("[Matrix RECOVER_POECC] FAILED corrections = %d out of %d blocks\n", failed_corr, n_blks));
		ret = (float)failed_corr / n_blks;
		my_stopwatch_stop(8);
	} else {
		DBG(printf("[Matrix RECOVER_POECC] Sums equal for ecMat=%lx, mat=%lx\n", (long)ecMat, (long)mat));
	}
	my_stopwatch_stop(9);
	return ret;
}

noinline
float MY_VEC_CHK_RECOVER_POECC(double sum, void* ecVec, gsl_vector* vec) {
	my_stopwatch_checkpoint(9);
	double* ecv1, *ecv2; ecv1=(double*)ecVec; ecv2=(double*)ecVec;
	double c = my_sum_vector(vec);
	if(c!=sum) {
		gsl_vector *vec1, *vec2; vec1=(gsl_vector*)vec; vec2=(gsl_vector*)vec;
		my_stopwatch_checkpoint(8);
		MY_SET_SIGSEGV_HANDLER();
		int jmpret = 0;
		SUPERSETJMP("VEC_CHK_RECOVER_POECC");
		while(vec!=TRIO_DOUBLE(vec,vec1,vec2)) vec=TRIO_DOUBLE(vec,vec1,vec2);
		while(ecVec!=TRIO_DOUBLE(ecVec,ecv1,ecv2)) ecVec=TRIO_DOUBLE(ecVec,ecv1,ecv2);
		decode(vec->data, vec->size, (const double*)ecVec);
		my_stopwatch_stop(8);
	} else {
		DBG(printf("[Vector RECOVER_POECC] Sums equal for ecVec=%lx, vec=%lx.\n", (long)ecVec, (long)vec));
	}
	my_stopwatch_stop(9);
}
#endif
