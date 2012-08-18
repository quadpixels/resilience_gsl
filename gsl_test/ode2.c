#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>

#define TOLERANCE 1e-06

int nFunc = 0;
/*
int func(double t, const double x[], double y[], void* params) {
	nFunc++;
	double mu = *(double*)params;
	y[0] = x[1];
	y[1] = -x[1] - 1.25*x[0];
	return GSL_SUCCESS;
}*/

/* y数组中存放的内容：
   y[0]=在t时刻y的值
   y[1]=在t时刻y一阶导数的值
   y[2]=在t时刻y二阶导数的值，依次类推
   */
int func(double t, const double y[], double dydt[], void* params) {
	nFunc++;
	double mu = *(double*)params;
	dydt[0] = y[1];
	dydt[1] = -y[1] - 1.25*y[0];
	return GSL_SUCCESS;
}

int jac(double t, const double y[], double* dfdy, double dfdt[], void* params) {
	double mu = *(double *)params;
	gsl_matrix_view dfdy_mat = gsl_matrix_view_array(dfdy, 2, 2);
	gsl_matrix* m = &dfdy_mat.matrix;
	gsl_matrix_set(m, 0, 0, 0.0);
	gsl_matrix_set(m, 0, 1, 1.0);
	gsl_matrix_set(m, 1, 0, -1.25);
	gsl_matrix_set(m, 1, 1, -1);
	dfdt[0] = 0.0;
	dfdt[1] = 0.0;
	return GSL_SUCCESS;
}

int main(int argc, char** argv) {

	printf("Optional args: ./ode2_test h eps_abs eps_rel");

	double h = 1e-07;  // Step size
	double eps_abs = 1e-06;// Error
	double eps_rel = 0;
	/* Arguments */
	if(argc > 1) h = atof(argv[1]);
	if(argc > 2) eps_abs=atof(argv[2]);
	if(argc > 3) eps_rel=atof(argv[3]);

	printf("Configurations: eps_abs=%g, eps_rel=%g, h=%g\n", eps_abs, eps_rel, h);

	const gsl_odeiv_step_type* T = gsl_odeiv_step_rk4;

	// For adaptive step size control
	gsl_odeiv_step* s = gsl_odeiv_step_alloc(T, 2);
	gsl_odeiv_control* c = gsl_odeiv_control_y_new (eps_abs, eps_rel);
	gsl_odeiv_evolve* e = gsl_odeiv_evolve_alloc(2);

	double mu = 10;
	double sd = 0;
	gsl_odeiv_system sys = {func, NULL, 2, &mu};
	double t = 0.0, tEnd = 10.0;


	double y[2] = {1.0, 0.0};

	// For manual override stepping control
	gsl_odeiv_step* s1 = gsl_odeiv_step_alloc(T, 2);
	double t1 = 0.0;
	double h1 = 1e-02;
	double y1[2] = {1.0, 0.0}, y1err[2];
	double dy1dt_in[2], dy1dt_out[2];

	// Starting adaptive evolution ...
	printf("T\ty\ty\'\n");
	while(t < tEnd) {
		int status = gsl_odeiv_evolve_apply(e, c, s, &sys, &t, tEnd, &h, y);
		if(status != GSL_SUCCESS) {
			printf("Error, status=%d\n", status);
			break;
		}
//		printf("%g\t%g\t%g\n", t, y[0], y[1]);
	}
	int nFuncAdaptive = nFunc;
	nFunc = 0;
	gsl_odeiv_evolve_free(e);
	gsl_odeiv_control_free(c);
	gsl_odeiv_step_free(s);

/*
	while(t1 < tEnd) {
		int status = gsl_odeiv_step_apply(s1, t1, h1, y1, y1err, dy1dt_in, dy1dt_out, &sys);
		if(status != GSL_SUCCESS) {
			printf("Error in manual stepping, status=%d\n", status);
			break;
		}
		dy1dt_in[0] = dy1dt_out[0]; dy1dt_in[1] = dy1dt_out[1];
		t1 += h1;
	}
*/

	// Result of adaptive-step evolution
	printf("%g\t%g\t%g (adaptive)\n", t, y[0], y[1]);
	printf("func has been evaluated %d times.\n", nFuncAdaptive);
/*
	printf("%g\t%g\t%g (manual)\n", t1, y1[0], y1[1]);
	printf("func has been evaluated %d times.\n", nFunc);
*/
	return 0;
}
