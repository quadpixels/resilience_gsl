#include <stdio.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_cblas.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector.h>

#define DIM 3

double mydat[] =  {1, 4, 8, 2, 6, 11, 3, 7, 13};
double mydat2[] = {35, 95, 178};
double mydat3[] = {5, 6, 3, 7, 5, 1, 39, 5, 80};

gsl_matrix* matA, *matA_backup, *matV;
gsl_vector* vecS, *vecWork, *vecWork2;
gsl_vector_complex* veccA;
gsl_matrix_complex* matcA;
gsl_eigen_gen_workspace* egwsA;
gsl_permutation* permA;
int signum;

void print_matrix(gsl_matrix* mat, char* name) {
	int i,j;
	printf("Matrix [%s] (%d x %d)\n", name, (int)mat->size1, (int)mat->size2);
	for(i=0; i<mat->size1; i++) {
		for(j=0; j<mat->size2; j++) {
			printf("%g ", gsl_matrix_get(mat, i, j));
		}
		printf("\n");
	}
}

void print_vector(gsl_vector* vec, char* name) {
	int i;
	printf("Vector [%s] (%d x 1)\n", name, (int)vec->size);
	for(i=0; i<vec->size; i++) {
		printf("%g ", gsl_vector_get(vec, i));
	}
	printf("\n");
}

int vector_equals(const gsl_vector* v1, const gsl_vector* v2) {
	if(v1->size != v2->size) return 0;
	int i; for(i=0; i<v1->size; i++) if(gsl_vector_get(v1, i)!=gsl_vector_get(v2,i))return 0;
	return 1;
}

int main(void) {
	int i, j, ret;
	matA = gsl_matrix_alloc(DIM, DIM); matV = gsl_matrix_alloc(DIM, DIM); matA_backup = gsl_matrix_alloc(DIM, DIM);
	vecS = gsl_vector_alloc(DIM); vecWork = gsl_vector_alloc(DIM); vecWork2 = gsl_vector_alloc(DIM);
	egwsA = gsl_eigen_gen_alloc(DIM);
	permA = gsl_permutation_alloc(DIM);	

	/* How to use complex numbers? 01-13-2012, cs */
	veccA = gsl_vector_complex_alloc(DIM);
	matcA = gsl_matrix_complex_alloc(DIM, DIM);
	
	for(i=0; i<DIM; i++) {
		for(j=0; j<DIM; j++) {
//			gsl_matrix_set(matA, i, j, (double)(i+j));
			gsl_matrix_set(matA, i, j, mydat[j*DIM+i]);
			gsl_matrix_set(matV, i, j, mydat3[j*DIM+i]);
		}
	}
	gsl_matrix_memcpy(matA_backup, matA);

	for(i=0; i<DIM; i++) gsl_vector_set(vecS, i, mydat2[i]);
	
	print_matrix(matA, "Matrix A");

	

	/* Solves the formula, using householder solver
 *	[  1   4   8 ] []   [  35]
 *	[  2   6  11 ] [] = [  95]
 *	[  3   7  13 ] []   [ 178] */
//	ret = gsl_linalg_HH_solve(matA, vecS, vecWork);

	/* Solves with LU Decomposition */
//	ret = gsl_linalg_LU_decomp(matA, permA, &signum);
//	ret = gsl_linalg_LU_solve(matA, permA, vecS, vecWork);
	
	print_matrix(matV, ">> matV before dsyrk");
	ret = gsl_blas_dsyrk(CblasLower, CblasNoTrans, 1.0, matA, 1.0, matV);
	print_matrix(matV, ">> matV after dsyrk");
	
//	printf("After SV Decomp\n");
//	print_matrix(matA, "Matrix A");
//	print_matrix(matV, "Matrix V");
//	print_vector(vecS, "Vector S");
//	print_vector(vecWork, "Vector Work");

	/* Check if answer is correct */
	ret = gsl_blas_dgemv(CblasNoTrans, 1.0, matA_backup, vecWork, 1.0, vecWork2);
	print_vector(vecWork2, "Checking answers");
	if(vector_equals(vecWork2, vecS)==1) printf("Solution OK\n");
	else printf("Solution not OK\n");
	gsl_eigen_gen_free(egwsA);
}
