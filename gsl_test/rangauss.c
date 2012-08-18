#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>

#define NUM 100000

int main() {
	const gsl_rng_type* T;
	gsl_rng* r;
	gsl_vector* v;
	v = gsl_vector_alloc(NUM);
	gsl_rng_env_setup();
	T = gsl_rng_mt19937;
	r = gsl_rng_alloc(T);
	int i; for(i=0; i<NUM; i++) {
//		double tmp = gsl_rng_uniform(r);
		double tmp = gsl_ran_gaussian(r, 2);
		gsl_vector_set(v, i, tmp);
	}
	FILE* f = fopen("./vecV", "w");
	gsl_vector_fprintf(f, v, "%f");
	fclose(f);
	printf("Done.\n");
	gsl_rng_free(r);
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
