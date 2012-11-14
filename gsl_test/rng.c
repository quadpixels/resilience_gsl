#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <time.h>

gsl_rng* g_rng;
unsigned long int g_seed;

int main(int argc, char** argv) {
	if(argc < 6) { printf("Generate input data for lasso! \nusage: ./rng_test mean sd SIZE FILENAME(must be A[1234].dat and b[1234].dat) Randseed [TYPE] \n");
		printf("Example data: A1, avg=-0.00045, sd=0.08, size=20000;\n");
		printf("              B1, avg=-0.21,    sd=0.76, size=40;\n");
		printf("TYPE: 0 ---- Gaussian Distribution\n");
		printf("      1 ---- Exponential Distribution (sd used as mu)\n");
		return 0;
	} 
	int col=500;
	double mean = atof(argv[1]);
	double sd = atof(argv[2]);
	int size = atoi(argv[3]);
	char* filename = argv[4];
	int randseed = atoi(argv[5]);

	int type = 0;
	if(argc >= 7) {
		type = atoi(argv[6]);
		switch(type) {
		case 0:
			printf(">> Using Gaussian Distribution.\n");
			break;
		case 1:
			printf(">> Using Exponential Distribution. (mu=%g)\n",
				sd);
			break;
		case 2:
			printf(">> Using Laplacian Distribution. (a=%g\n",
				sd);
			break;
		case 3:
			printf(">> Using Cauchy Distribution. (a=%g)\n",
				sd);
			break;
		}
	}

	if(filename[0]=='b') col=1;
	int row = size / col;
	printf("sd=%f\n", sd);
	
	g_seed = (unsigned long int)(time(0));
	gsl_rng_env_setup();
	g_rng = gsl_rng_alloc(gsl_rng_default);
	FILE* out;

	gsl_rng_set(g_rng, g_seed);
	double variate;
	out = fopen(filename, "w");
	fprintf(out, "%%%%MatrixMarket matrix array real general\n");
	fprintf(out, "%d %d\n", row, col);
	int i=0; for(i=0; i<size; i++) {
		if(i%1000000==0) { printf("i=%d/%d\n", i, size); }
		switch(type) {
			case 0:
				variate = gsl_ran_gaussian(g_rng, sd) + mean;
				break;
			case 1:
				variate = gsl_ran_exponential(g_rng, sd) + mean;
				break;
			case 2:
				variate = gsl_ran_laplace(g_rng, sd) + mean;
				break;
			case 3:
				variate = gsl_ran_cauchy(g_rng, sd) + mean;
				break;
		} 
		fprintf(out, "%f\n", variate);
	}
	fclose(out);
	return 0;
}
