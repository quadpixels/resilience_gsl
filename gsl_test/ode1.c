#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>
//#include <gsl/tommy.h>

int nFunc=0;
int nFunc2=0;

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

int do_func(double t, const double y[], double f[], void* params) {
	double mu = *(double*)params;
	f[0] = y[1];
	f[1] = -y[0] - mu * y[1] * (y[0]*y[0] - 1);
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
	double eps_abs2 = 1e-06, h2 = 1e-04;
	double refThresh = 1e-04;
	int stride = 100; // 多少次追及之后进行一次比较
	int catchups = 0; // 追次次数计数器

	if(argc >= 5) {
		eps_abs2 = atof(argv[1]);
		h2       = atof(argv[2]);
		refThresh= atof(argv[3]);
		stride  = atoi(argv[4]);
	}
	printf("eps_abs2=%.3e, h2=%.3e, refThresh=%.3e, stride=%d\n", eps_abs2, h2, refThresh, stride);

	const gsl_odeiv_step_type* T = gsl_odeiv_step_rk4;
	gsl_odeiv_step* s = gsl_odeiv_step_alloc(T, 2);
	gsl_odeiv_control* c = gsl_odeiv_control_y_new (1e-06, 0.0);
//	gsl_odeiv_control* c = gsl_odeiv_control_y_new (1e-04, 0.0);
	gsl_odeiv_evolve* e = gsl_odeiv_evolve_alloc(2);

	gsl_odeiv_step* s2 = gsl_odeiv_step_alloc(T, 2);
	gsl_odeiv_control* c2 = gsl_odeiv_control_y_new(eps_abs2, 0.0);
	gsl_odeiv_evolve* e2 = gsl_odeiv_evolve_alloc(2);

	double mu = 10;
	double sum= 0, sum2=0;
	gsl_odeiv_system sys = {func, jac, 2, &mu};
	gsl_odeiv_system sys2 ={func2,NULL, 2, &mu};

	double t = 0.0, tFinal = 10.0;
	double t2= 0.0;
	double h = 1e-06;

	double y[2] = {1.0, 0.0};
	double y2[2]= {1.0, 0.0};
	double yold[2]={1.0, 0.0};
	double told = 0;
	
//	MY_SET_SIGSEGV_HANDLER();
	int jmpret;
	int nSync=0; double syncSD=0.0; int nErr=0;
	int nItr=0, nStride=0;
	int revertable=0; // 不能无限制地revert下去
	int nReverts = 0;
	while(t < tFinal) {
		double hold = h;
		int status = gsl_odeiv_evolve_apply(e, c, s, &sys, &t, tFinal, &h, y);
		if(status != GSL_SUCCESS) {
			printf("Error, status=%d\n", status);
			return 0;
		}
		sum += y[0] * (h-hold);
		while(t2 < t) {
			int status = gsl_odeiv_evolve_apply(e2,c2,s2, &sys2, &t2, tFinal, &h2, y2);
			++catchups;
			if(catchups == stride) {
				catchups = 0;
				revertable = revertable + 1;
				double err = y[0]-y2[0];
				syncSD = syncSD + err*err;
				if(err*err > refThresh) { // I'm tweaking this parameter.
					++nErr;
					if(revertable >= 2) {
						y[0]=yold[0];
						y[1]=yold[1];
						t = told;
						t2= told;
						revertable = revertable - 2;
						nReverts ++;
					}
				}
				y2[0] = y[0];
				y2[1] = y[1];
				yold[0]=y[0];
				yold[1]=y[1];
				told=t;
				++nSync;
			}
		}
		++nItr;
	}
	gsl_odeiv_evolve_free(e);
	gsl_odeiv_control_free(c);
	gsl_odeiv_step_free(s);

	printf("x=%.13e nFunc=%d nFunc2=%d nErr=(%d/%d) nReverts=%d\n", y[0], nFunc, nFunc2, nErr, nSync, nReverts);
	return 0;
}
