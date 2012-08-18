#include <stdio.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_fft_complex.h>

#define REAL(z,i) ((z)[2*(i)])
#define IMAG(z,i) ((z)[2*(i)+1])

int main(void) {
	int i;
	const int n = 128;
	double data[2*n];
	
	gsl_fft_complex_wavetable* wavetable;
	gsl_fft_complex_workspace* workspace;

	for(i=0; i<n; i++) {
		REAL(data, i) = 0.0;
		IMAG(data, i) = 0.0;
	}
	REAL(data,0) = 1.0;
	for(i=1; i<10; i++) {
		REAL(data, i) = REAL(data, 128-i) = 1.0;
	}
	for(i=0; i<128; i++) {
		printf("%d %e %e\n", i, REAL(data, i), IMAG(data, i));
	}
	printf("\n");

	wavetable = gsl_fft_complex_wavetable_alloc(n);
	workspace = gsl_fft_complex_workspace_alloc(n);
	for(i=0; i<wavetable->nf; i++) {
		printf("# factor %d: %d\n", i, wavetable->factor[i]);
	}

	gsl_fft_complex_forward(data, 1, 128, wavetable, workspace);

	for(i=0; i<128; i++) {
		printf("%d %e %e\n", i, REAL(data, i), IMAG(data, i));
	}
	return 0;
}
