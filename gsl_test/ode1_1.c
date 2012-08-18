#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>
//#include <gsl/tommy.h>

int nFunc=0;
int nFunc2=0;

int do_func(double t, const double y[], double f[], void* params) {
	double mu = *(double*)params;
	f[0] = y[1];
	f[1] = -y[0] - mu * y[1] * (y[0]*y[0] - 1);
	return GSL_SUCCESS;
}

int func(double t, const double y[], double f[], void* params) {
	++nFunc;
	do_func(t, y, f, params);
	return GSL_SUCCESS;
}

int func2(double t, const double y[], double f[], void* params) {
	++nFunc2;
	do_func(t, y, f, params);
	return GSL_SUCCESS;
}

int jac(double t, const double y[], double* dfdy, double dfdt[], void* params) {
	double mu = *(double *)params;
	gsl_matrix_view dfdy_mat = gsl_matrix_view_array(dfdy, 2, 2);
	gsl_matrix* m = &dfdy_mat.matrix;
	gsl_matrix_set(m, 0, 0, 0.0);
	gsl_matrix_set(m, 0, 1, 1.0);
	gsl_matrix_set(m, 1, 0, -2.0*mu*y[0]*y[1] - 1.0);
	gsl_matrix_set(m, 1, 1, -mu*(y[0]*y[0] - 1.0));
	dfdt[0] = 0.0;
	dfdt[1] = 0.0;
	return GSL_SUCCESS;
}

int main(int argc, char** argv) {
	int stride = 4; // 多少次追及之后进行一次比较
	double stepdiff = 1e-05;
	if(argc >= 2) {
		stride = atoi(argv[1]);
	}
	if(argc >= 3) {
		stepdiff = atof(argv[2]);
	}
	printf("stride=%d, stepdiff=%g\n", stride, stepdiff);

	double refThresh = 1e-04;
	int catchups = 0; // 追次次数计数器

	const gsl_odeiv_step_type* T = gsl_odeiv_step_rk4;
	gsl_odeiv_step* s = gsl_odeiv_step_alloc(T, 2);
	gsl_odeiv_control* c = gsl_odeiv_control_y_new (1e-06, 0.0);
	gsl_odeiv_evolve* e = gsl_odeiv_evolve_alloc(2);
	
	gsl_odeiv_step* s2 = gsl_odeiv_step_alloc(T, 2);

	double mu = 10;
	gsl_odeiv_system sys = {func, jac, 2, &mu};
	gsl_odeiv_system sys2= {func2,jac, 2, &mu};

	double t = 0.0, tFinal = 40.0;
	double t2= 0.0, tToe   = 0; // 后脚跟，就是说后面的时间步。
	double h = 1e-06, h2;

	double y[2] = {1.0, 0.0};

	double told = 0;
	double sum = 0, sum2 = 0;
	
	int jmpret;
	int nSync=0; double syncSD=0.0; int nErr=0;
	int nItr=0, nStride=0;
	int revertable=0; // 不能无限制地revert下去
	int nReverts = 0;
	while(t < tFinal) {
		told = t;
		int status = gsl_odeiv_evolve_apply(e, c, s, &sys, &t, tFinal, &h, y);
		if(status != GSL_SUCCESS) {
			printf("Error, status=%d\n", status);
			return 0;
		}
		printf("%g %g\n", t, y[0]);
	}

	gsl_odeiv_evolve_free(e);
	gsl_odeiv_control_free(c);
	gsl_odeiv_step_free(s);

	printf("y1=%g nFunc =%d nItr=%d\n", y[0], nFunc, nItr);
	return 0;
}
