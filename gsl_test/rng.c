#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <time.h>

gsl_rng* g_rng;
unsigned long int g_seed;

int main(int argc, char** argv) {
	if(argc < 5) { printf("Generate input data for lasso! \nusage: ./rng_test mean sd SIZE FILENAME(must be A[1234].dat and b[1234].dat) \n");
		printf("Example data: A1, avg=-0.00045, sd=0.08, size=20000;\n");
		printf("              B1, avg=-0.21,    sd=0.76, size=40;\n");
		return 0;
	}
	int col=500;
	double mean = atof(argv[1]);
	double sd = atof(argv[2]);
	int size = atoi(argv[3]);
	char* filename = argv[4];
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
		variate = gsl_ran_gaussian(g_rng, sd) + mean;
		fprintf(out, "%f\n", variate);
	}
	fclose(out);
	return 0;
}
