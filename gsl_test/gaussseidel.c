#include <stdio.h>
#include <assert.h>
#include "tommy1.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <time.h>

int size = 10;

void print_matrix(gsl_matrix*);
void print_vector(gsl_vector*);
void prepare_x_b();
void my_naive_solution_GaussSeidel_RA(gsl_matrix* A, gsl_vector* x, gsl_vector* b);
void my_naive_solution_Jacobi(gsl_matrix* A, gsl_vector* x, gsl_vector* b);
void my_naive_solution_GaussSeidel(gsl_matrix* A, gsl_vector* x, gsl_vector* b);

int main(int argc, char** argv) {
	if(argc > 1) size = atoi(argv[1]);
	printf("Size: %d\n", size);
	gsl_matrix* A = gsl_matrix_alloc(size, size);
	gsl_matrix* A2= gsl_matrix_alloc(size, size);
	gsl_vector* x = gsl_vector_alloc(size);
	gsl_vector* b = gsl_vector_alloc(size);
	gsl_vector* b2 = gsl_vector_alloc(size);

	if(argc > 2) {
		FILE* fA = fopen("matA", "r");
		FILE* fb = fopen("vecB", "r");
		gsl_matrix_fscanf(fA, A);
		gsl_vector_fscanf(fb, b);
		fclose(fA);
		fclose(fb);
	} 
entre:
	printf("Enter a choice.\n");
	printf("1) Populate A as sparse.\n");
	printf("2) Populate A as dense.\n");
	printf("3) Print A, x and b.\n");
	printf("4) Solve x using GSL's routine (HH Solver).\n");
	printf("5) Solve x using Gauss-Seidel method with rearrangement.\n");
	printf("6) Solve x using Jacobi method.\n");
	printf("7) Solve x using Gauss-Seidel method.\n");
	int ch, i, j;
	scanf("%d", &ch);
	switch(ch) {
		case 1:
			gsl_matrix_set_zero(A);
			gsl_vector_set_zero(x);
			for(i=0; i<(int)(size*size*0.1); i++) {
				float r = (float)rand()/RAND_MAX*10.0f-5.0f;
				float x = ((int)rand()) % size;
				float y = ((int)rand()) % size;
				gsl_matrix_set(A, x, y, r);
			}
			for(i=0; i<size; i++) {
				float r = (float)rand()/RAND_MAX*10.0f-5.0f;
				gsl_vector_set(x, i, r);
			}
			Print_Matrix(CblasNoTrans, A, "A");
			gsl_blas_dgemv(CblasNoTrans, 1.0f, A, x, 0.0f, b);
			break;
		case 2:
			gsl_matrix_set_zero(A);
			for(i=0; i<size; i++) {
				for(j=0; j<size; j++) {
					float r = (float)rand()/RAND_MAX*10.0f-5.0f;
					gsl_matrix_set(A, i, j, r);
				}
			}
			for(i=0; i<size; i++) {
				float r = (float)rand()/RAND_MAX*10.0f-5.0f;
				gsl_vector_set(x, i, r);
			}
			Print_Matrix(CblasNoTrans, A, "A");
			gsl_blas_dgemv(CblasNoTrans, 1.0f, A, x, 0.0f, b);
			break;
		case 3:
			Print_Matrix(CblasNoTrans, A, "This is A");
			Print_Vector(x, "This is x");
			Print_Vector(b, "This is A*x");
			break;
		case 4: // Use HH Solver
			gsl_matrix_memcpy(A2, A);
			gsl_linalg_HH_solve(A2, b, x);
			Print_Vector(x, "x as solved by HH Solver");
			break;
		case 5:
			gsl_vector_set_zero(x);
			gsl_matrix_memcpy(A2, A);
			gsl_vector_memcpy(b2, b);
			my_naive_solution_GaussSeidel_RA(A2, x, b2);
			Print_Vector(x, "x as solved by my Gauss-Seidel solver with rearrangement");
			break;
		case 6:
			gsl_vector_set_zero(x);
			gsl_matrix_memcpy(A2, A);
			gsl_vector_memcpy(b2, b);
			my_naive_solution_Jacobi(A2, x, b2);
			Print_Vector(x, "x as solved by my Jacobi solver");
			break;
		case 7:
			gsl_vector_set_zero(x);
			gsl_matrix_memcpy(A2, A);
			gsl_vector_memcpy(b2, b);
			my_naive_solution_GaussSeidel(A2, x, b2);
			Print_Vector(x, "x as solved by my Gauss-Seidel solver");
			break;
	}
	goto entre;
}

void my_naive_solution_GaussSeidel_RA(gsl_matrix* A, gsl_vector* x, gsl_vector* b) {
	printf("size of x=%d b=%d\n", x->size, b->size);
	assert(A->size1 == A->size2);
	assert(A->size1 == x->size);
	assert(x->size  == b->size);

	int i,j,k,n=A->size1;
	int niter = 1;

	for(k=0; k<n-1; k++) {
		for(i=k+1; i<n; i++) {
			double m = gsl_matrix_get(A,k,k)!=0 ? gsl_matrix_get(A,i,k)/gsl_matrix_get(A,k,k) : 0;
			for(j=k+1; j<n; j++) {
				double aij = gsl_matrix_get(A,i,j) - m*gsl_matrix_get(A,k,j);
				gsl_matrix_set(A,i,k,0);
				gsl_matrix_set(A,i,j,aij);
			}
			double bi = gsl_vector_get(b,i) - m*gsl_vector_get(b,k);
			gsl_vector_set(b,i,bi);
		}
	}
	Print_Matrix(CblasNoTrans, A, "A after rearrangement");
	
	// Here is the second step
	
	double xn = gsl_matrix_get(A,n-1,n-1)!=0 ? gsl_vector_get(b,n-1)/gsl_matrix_get(A,n-1,n-1) : 0;
	gsl_vector_set(x,n-1,xn);

	for(k=0; k<niter; k++) {
		for(i=n-2; i>=0; --i) {
			double xi = gsl_vector_get(b,i);
			for(j=i+1; j<n; ++j) {
				xi = xi - gsl_vector_get(x,j)*gsl_matrix_get(A,i,j);
			}
			xi = gsl_matrix_get(A,i,i)!=0 ? xi/gsl_matrix_get(A,i,i) : xi;
			gsl_vector_set(x, i, xi);
		}
		printf("Iteration %d\n", k);
		Print_Vector(x, "Approximate solution");
	}
}

// Jacobi Method
void my_naive_solution_Jacobi(gsl_matrix* A, gsl_vector* x, gsl_vector* b) {
	printf("size of x=%d b=%d\n", x->size, b->size);
	assert(A->size1 == A->size2);
	assert(A->size1 == x->size);
	assert(x->size  == b->size);

	int i,j,k;
	int n = A->size1;
	gsl_vector* xnew = gsl_vector_alloc(n);
	int niter = 10;
	for(k=0; k<niter; k++) {
		for(i=0; i<n; i++) {
			double aii = gsl_matrix_get(A,i,i);
			if(aii==0) continue;
			double aii_rec = 1/aii;
			double x_new = gsl_vector_get(b,i)*aii_rec;
			for(j=0; j<n; j++) {
				if(i!=j) x_new = x_new - gsl_matrix_get(A,i,j)*gsl_vector_get(x,j)*aii_rec;
			}
			gsl_vector_set(xnew,i,x_new);
		}
		gsl_vector_memcpy(x, xnew);
		printf("Iteration %d\n", k);
		Print_Vector(x, "x after this iteration");
	}
}

void my_naive_solution_GaussSeidel(gsl_matrix* A, gsl_vector* x, gsl_vector* b) {
	printf("size of x=%d b=%d\n", x->size, b->size);
	assert(A->size1 == A->size2);
	assert(A->size1 == x->size);
	assert(x->size  == b->size);

	int i,j,k;
	int n = A->size1;
	int niter = 10;
	for(k=0; k<niter; k++) {
		for(i=0; i<n; i++) {
			double aii = gsl_matrix_get(A,i,i);
			if(aii==0) continue;
			double aii_rec = 1/aii;
			double x_new = gsl_vector_get(b,i)*aii_rec;
			for(j=0; j<n; j++) {
				if(i!=j) x_new = x_new - gsl_matrix_get(A,i,j)*gsl_vector_get(x,j)*aii_rec;
			}
			gsl_vector_set(x,i,x_new);
		}
		printf("Iteration %d\n", k);
		Print_Vector(x, "x after this iteration");
	}
}

void my_naive_solution_GaussianElimination(gsl_matrix* A, gsl_vector* x, gsl_vector* b) {
	
}
