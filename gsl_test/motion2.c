#include <string.h>
#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>

double mu = 10;
int nFunc = 0;

typedef struct {
	double x;
	double v;
} State;

typedef struct {
	double dx;
	double dv;
} Derivative;

Derivative derivative;

Derivative evaluate(const State* initial, double t, double dt, Derivative* d) {
	State state;
	state.x = (initial->x) + (d->dx) * dt;
	state.v = (initial->v) + (d->dv) * dt;
	
	Derivative output;
	output.dx = state.v;
	output.dv = -state.x + mu * state.v * (1-state.x * state.x);
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

State RK4WithError(State* start, double t_from, double t_to) {
	State tmp;
	memcpy(&tmp, start, sizeof(State));
	// One step
	RK4(start, t_from, t_to);
	// Two steps
	double t_middle = (t_from + t_to) * 0.5;
	RK4(&tmp, t_from, t_middle);
	RK4(&tmp, t_middle, t_to);
	tmp.x -= start->x; tmp.x *= 4.0/15.0;
	tmp.v -= start->v; tmp.v *= 4.0/15.0;
	return tmp;
}


int func(double t, const double y[], double f[], void* params) {
	++nFunc;
	double mu = *(double*)params;
	f[0] = y[1];
	f[1] = -y[0] - mu * y[1] * (y[0]*y[0] - 1);
	return GSL_SUCCESS;
}

int main(int argc, char** argv) {
	double h = 1e-02;
	if(argc >= 2) h = atof(argv[1]);

	const gsl_odeiv_step_type* T = gsl_odeiv_step_rk4;
	gsl_odeiv_step* s = gsl_odeiv_step_alloc(T, 2);
	gsl_odeiv_evolve* e = gsl_odeiv_evolve_alloc(2);
	double t = 0.0, tFinal = 100.0;
	double t2 = 0;//, h = 1e-02;
	double y[2] = {1.0, 0.0};
	double y_err[2]= {0.0, 0.0};
	double dydt_in[2];
	double dydt_out[2];
	gsl_odeiv_system sys = {func, NULL, 2, &mu};
	
	State state;
	state.x = 1.0; state.v = 0.0;
	
	GSL_ODEIV_FN_EVAL(&sys, t, y, dydt_in);
	while(t < tFinal) {
		int status = gsl_odeiv_step_apply(s, t, h, y, y_err,
						dydt_in, dydt_out, &sys);
		dydt_in[0] = dydt_out[0];
		dydt_in[1] = dydt_out[1];
		
		State error = RK4WithError(&state, t, t+h);
		
		printf("%g\t%g\t%.15e\t%.15e",
			t, h, y[0], state.x);
		printf(" yerr[GSL]:[%g,%g], yerr[me]:[%g,%g]",
			y_err[0], y_err[1], error.x, error.v);

		printf("\n");
		
		t = t + h;
	}
}
