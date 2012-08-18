#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>

double a = 2;

// Euler Integration
double x1 = 0, v1 = 3;

// RK4 Integration
double x2 = 0, v2 = 3;
typedef struct {
	double x;
	double v;
} State;
State state;
typedef struct {
	double dx;
	double dv;
} Derivative;

Derivative evaluate(const State* initial, double t, double dt, Derivative* d) {
	State state;
	state.x = (initial->x) + (d->dx * dt);
	state.v = (initial->v) + (d->dv * dt);

	Derivative output;
	output.dx = state.v;
	output.dv = a;
	return output;
}

void RK4(State* state, double t_from, double t_to) {
	double t  = t_from;
	double dt = t_to - t_from;
	Derivative empty;
	empty.dx = empty.dv = 0;
	Derivative a = evaluate(state, t, 0.0f, &empty);
	Derivative b = evaluate(state, t, dt*0.5f, &a);
	Derivative c = evaluate(state, t, dt*0.5f, &b);
	Derivative d = evaluate(state, t, dt,      &c);

	const double dxdt = 1.0f/6.0f * (a.dx + 2.0f*(b.dx + c.dx) + d.dx);
	const double dvdt = 1.0f/6.0f * (a.dv + 2.0f*(b.dv + c.dv) + d.dv);

	state->x = state->x + dxdt * dt;
	state->v = state->v + dvdt * dt;
}

void Euler(double* t_from, double t_to) {
	double step = 0.01;
	while(*t_from < t_to) {
		double t = (*t_from) + step;
		if(t > t_to) t=t_to;
		double diff = t - (*t_from);

		x1 = x1 + v1*diff;
		v1 = v1 + a*diff;
		*t_from = t;
	}
}

int func(double t, const double y[], double f[], void* params) {
	double mu = *(double*)params;
	f[0] = y[1];
	f[1] = a;
	return GSL_SUCCESS;
}

int jac(double t,  const double y[], double* dfdy, double dfdt[], void* params) {
	double mu = *(double *)params;
	gsl_matrix_view dfdy_mat = gsl_matrix_view_array(dfdy, 2, 2);
	gsl_matrix* m = &dfdy_mat.matrix;
	gsl_matrix_set(m, 0, 0, 0.0);
	gsl_matrix_set(m, 0, 1, 1.0);
	gsl_matrix_set(m, 1, 0, 0.0);
	gsl_matrix_set(m, 1, 1, 0.0);
	dfdt[0] = 0.0;
	dfdt[1] = 0.0;
	return GSL_SUCCESS;
}

int main(int argc, char** argv) {
	printf("Comparing Euler,RK4 and GSL's integrator\n");
	const gsl_odeiv_step_type* T = gsl_odeiv_step_rk4;
	gsl_odeiv_step* s = gsl_odeiv_step_alloc(T, 2);
	gsl_odeiv_control* c = gsl_odeiv_control_y_new (1e-04, 0.1);
	gsl_odeiv_evolve* e = gsl_odeiv_evolve_alloc(2);

	double mu = 10;
	gsl_odeiv_system sys = {func, jac, 2, &mu};

	int i=0; 

	double t = 0.0, t1 = 100000.0;
	double t_euler = 0.0;
	double h = 1e-01;
	double y[2] = {0.0, 3.0};
	state.x = 0; state.v = 3;

	printf("Time      X(GSL)                    X(Euler)     X(RK4)         X(reference)\n");
	while(t < t1) {
		double t_from = t;
		int status = gsl_odeiv_evolve_apply(e, c, s, &sys, &t, t1, &h, y);
		if(status != GSL_SUCCESS) {
			printf("Error, status=%d\n", status);
			break;
		}
		Euler(&t_euler, t);
		RK4(&state, t_from, t);
		printf("%.5e %.9e %.9e %.9e %.9e\n", t, y[0], x1, state.x, 3*t+t*t);
	}


	gsl_odeiv_evolve_free(e);
	gsl_odeiv_control_free(c);
	gsl_odeiv_step_free(s);

	return 0;

}
