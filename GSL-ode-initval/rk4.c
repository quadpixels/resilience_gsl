// This is branch replica2 (Using round-robin).
// Inspired by the preprocessor trick.
// Aug 09 2012.
#include "../real.h"
/* ode-initval/rk4.c
*
* Copyright (C) 1996, 1997, 1998, 1999, 2000 Gerard Jungman
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or (at
* your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

/* Runge-Kutta 4th order, Classical */

/* Author:  G. Jungman
*/

/* Reference: Abramowitz & Stegun, section 25.5. equation 25.5.10

Error estimation by step doubling, see eg. Ascher, U.M., Petzold,
L.R., Computer methods for ordinary differential and
differential-algebraic equations, SIAM, Philadelphia, 1998.
*/

#include <config.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv.h>
#include <gsl/tommy.h>
#include <gsl/triplicate.h>

#include "odeiv_util.h"

//----------------Fault injection related ----------
// My include files.
#include "tommy_rk4_defs.h"

#define K_PROTECT 7
#define K_REPLICA_PAIRS 7
#define Y_PROTECT 4
#define Y_REPLICA_PAIRS 4
#define Y0_PROTECT 3
#define Y0_REPLICA_PAIRS 3
#define YTMP_PROTECT 6
#define YTMP_REPLICA_PAIRS 6
//##@@!! Replication confs


// How many instances of 'k' should be protected ?
// Choose from 0,1,2,3,4,5,6,7 (because there are only 7 uses of k in rk4_step)
// #define K_PROTECT 7
// How many times would each pair of replica of 'k' be used ?
// Choose from 1,2,3,4,5,6,7
// #define K_REPLICA_PAIRS 7

// Same as 'k'
// There are 4 occurrances of 'y'
// #define Y_PROTECT 4
// #define Y_REPLICA_PAIRS 4

// There are 3 ocurrances of 'y0'
// #define Y0_PROTECT 3
// #define Y0_REPLICA_PAIRS 3

// There are 6 occurrances of 'ytmp'
// #define YTMP_PROTECT 6
// #define YTMP_REPLICA_PAIRS 6


/* For accounting on 2012-07-28 */
int cnt_diff_idx_i = 0;
int cnt_diff_ptr_y = 0;
int cnt_diff_ptr_k = 0;
int cnt_diff_ptr_ytmp = 0;
int cnt_diff_ptr_y0 = 0;
int cnt_oob_ptr_y = 0;
int cnt_oob_ptr_k = 0;
int cnt_oob_ptr_ytmp = 0;
int cnt_oob_ptr_y0 = 0;
size_t g_dim;


typedef struct
{
double *k;
double *k1;
double *y0;
double *ytmp;
double *y_onestep;
}
rk4_state_t;

rk4_state_t* state_backup;

static int
rk4_step (double *y, const rk4_state_t *state,
const double h, const double t, const size_t dim,
const gsl_odeiv_system *sys) noinline;

static int
rk4_step_actual (double *y, const rk4_state_t *state,
const double h, const double t, const size_t dim,
const gsl_odeiv_system *sys) noinline;

static void *
rk4_alloc (size_t dim)
{
rk4_state_t *state = (rk4_state_t *) malloc (sizeof (rk4_state_t));

if (state == 0)
{
GSL_ERROR_NULL ("failed to allocate space for rk4_state", GSL_ENOMEM);
}

state->k = (double *) malloc (dim * sizeof (double));

if (state->k == 0)
{
free (state);
GSL_ERROR_NULL ("failed to allocate space for k", GSL_ENOMEM);
}

state->k1 = (double *) malloc (dim * sizeof (double));

if (state->k1 == 0)
{
free (state->k);
free (state);
GSL_ERROR_NULL ("failed to allocate space for k1", GSL_ENOMEM);
}

state->y0 = (double *) malloc (dim * sizeof (double));

if (state->y0 == 0)
{
free (state->k);
free (state->k1);
free (state);
GSL_ERROR_NULL ("failed to allocate space for y0", GSL_ENOMEM);
}

state->ytmp = (double *) malloc (dim * sizeof (double));

if (state->ytmp == 0)
{
free (state->y0);
free (state->k);
free (state->k1);
free (state);
GSL_ERROR_NULL ("failed to allocate space for ytmp", GSL_ENOMEM);
}

state->y_onestep = (double *) malloc (dim * sizeof (double));

if (state->y_onestep == 0)
{
free (state->ytmp);
free (state->y0);
free (state->k);
free (state->k1);
free (state);
GSL_ERROR_NULL ("failed to allocate space for ytmp", GSL_ENOMEM);
}

return state;
}

static int rk4_call_count = 0;
unsigned long k_0, k_1, k_2, k1_0, k1_1, k1_2, y0_0, y0_1, y0_2,
	ytmp_0, ytmp_1, ytmp_2, y_onestep_0, y_onestep_1, y_onestep_2, y_0, y_1, y_2,
	state_0, state_1, state_2, sys_0, sys_1, sys_2;

void *state_bk_1, *state_bk_2, *sys_bk_1, *sys_bk_2, *y_bk_1, *y_bk_2, *y_onestep_bk_1, *y_onestep_bk_2,
	*y0_bk_1, *y0_bk_2, *k_bk_1, *k_bk_2, *k1_bk_1, *k1_bk_2, *ytmp_bk_1, *ytmp_bk_2;

inline static void backup(void* orig, void* copy1, void* copy2, size_t sz) {
	if(copy1==NULL) copy1=malloc(sz);
	if(copy2==NULL) copy2=malloc(sz);
	memcpy(copy1, orig, sz);
	memcpy(copy2, orig, sz);
}

inline static unsigned char checksum(void* addr, size_t sz) {
	unsigned char ret = 0U;
	char* start = (char*)addr, *end = (char*)addr+sz;
	for(; start!=end; start++) {
		ret += *start;
	}
	return ret;
}

inline static void correct(void* orig, void* copy1, void* copy2, size_t sz) {
	unsigned char sum0 = checksum(orig, sz);
	unsigned char sum1 = checksum(copy1,sz);
	unsigned char sum2 = checksum(copy2,sz);
	#define PPP fprintf(stderr, ">>>>>=====CorrectingJmpBuf-----\n");
	if((sum0!=sum1) && (sum1==sum2)) { memcpy(orig, copy1, sz); PPP; }
	else if((sum0!=sum1) && (sum0==sum2)) { memcpy(copy1, orig, sz); PPP; }
	else if((sum1!=sum2) && (sum0==sum1)) { memcpy(copy2, orig, sz); PPP; }

}

static int
rk4_step (double *y, const rk4_state_t *state,
const double h, const double t, const size_t dim,
const gsl_odeiv_system *sys) {
/*
	double *k;
	double *k1;
	double *y0;
	double *ytmp;
	double *y_onestep;
*/
	double *_y; rk4_state_t* _state; gsl_odeiv_system* _sys;
	_y = y; _state = (rk4_state_t*)state; _sys = (gsl_odeiv_system*)sys;
	#ifdef FT3
	MY_SET_SIGSEGV_HANDLER();
	if(rk4_call_count++ >= ODE_HANDLER_INTERVAL) {
		rk4_call_count = 0;
		my_stopwatch_checkpoint(5);
		TRIPLICATE(state->k, k_0, k_1, k_2);
		TRIPLICATE(state->k1, k1_0, k1_1, k1_2);
		TRIPLICATE(state->y0, y0_0, y0_1, y0_2);
		TRIPLICATE(state->ytmp, ytmp_0, ytmp_1, ytmp_2);
		TRIPLICATE(state->y_onestep, y_onestep_0, y_onestep_1, y_onestep_2);
		TRIPLICATE(y, y_0, y_1, y_2);
		TRIPLICATE(state, state_0, state_1, state_2)
		TRIPLICATE(sys, sys_0, sys_1, sys_2);
		const size_t sz = sizeof(double)*dim;
		backup(state->k, k_bk_1, k_bk_2, sz);
		backup(state->k1, k1_bk_1, k1_bk_2, sz);
		backup(state->y0, y0_bk_1, y0_bk_2, sz);
		backup(state->ytmp, ytmp_bk_1, ytmp_bk_2, sz);
		backup(state->y_onestep, y_onestep_bk_1, y_onestep_bk_2, sz);
		my_stopwatch_stop(5);
	}
	int jmpret;
	SUPERSETJMP("After encoding (rk4_step)");
	if(jmpret != 0) {
		const int sz = dim * sizeof(double);
		my_stopwatch_checkpoint(5);
		TRI_RECOVER(state_0, state_1, state_2);
		if((unsigned long)_state != state_0) _state = (rk4_state_t*)state_0;
		TRI_RECOVER(k_0, k_1, k_2);
		if((unsigned long)(_state->k) != k_0) _state->k = (double*)k_0;
		TRI_RECOVER(k1_0, k1_1, k1_2);
		if((unsigned long)(_state->k1) !=k1_0)_state->k1=(double*)k1_0;
		TRI_RECOVER(y0_0, y0_1, y0_2);
		if((unsigned long)(_state->y0) != y0_0)_state->y0=(double*)y0_0;
		TRI_RECOVER(ytmp_0, ytmp_1, ytmp_2);
		if((unsigned long)(_state->ytmp)!=ytmp_0)_state->ytmp=(double*)ytmp_0;
		TRI_RECOVER(y_onestep_0, y_onestep_1, y_onestep_2);
		if((unsigned long)(_state->y_onestep)!=y_onestep_0)_state->y_onestep=(double*)y_onestep_0;
		TRI_RECOVER(y_0, y_1, y_2);
		if((unsigned long)_y != y_0) _y = (double*)y_0;
		TRI_RECOVER(sys_0, sys_1, sys_2);
		if((unsigned long)_sys != sys_0) _sys = (gsl_odeiv_system*)sys_0;
		my_stopwatch_stop(5);
		correct(state->k, k_bk_1, k_bk_2, sz);
		correct(state->k1, k1_bk_1, k1_bk_2, sz);
		correct(state->y0, y0_bk_1, y0_bk_2, sz);
		correct(state->ytmp, ytmp_bk_1, ytmp_bk_2, sz);
		correct(state->y_onestep, y_onestep_bk_1, y_onestep_bk_2, sz);
	}
	#endif
	return rk4_step_actual(_y, _state, h, t, dim, _sys);
}

static int
rk4_step_actual (double *y, const rk4_state_t *state,
const double h, const double t, const size_t dim,
const gsl_odeiv_system *sys)
{
/* Makes a Runge-Kutta 4th order advance with step size h. */

/* initial values of variables y. */
const double *y0 = state->y0;

/* work space */
double *ytmp = state->ytmp;

/* Runge-Kutta coefficients. Contains values of coefficient k1
in the beginning
*/
double *k = state->k;

// Include this file replicates the pointers according to your parameters above
// So that k has replica k1a, k1b, k2a, k2b, k3a, k3b... depending on how many
// times instances of k needs be protected and how many times each replica should
// be used, the corresponding number of replicas would be generated in this
// header file.

#include "tommy_rk4_replicate.h"
size_t i;

REAL_TRY(0) {

/* k1 step */

for (i = 0; i < dim; i++)
{
//    Human-translated of the following stmt
//    for ease of replicating k and y
//    y[i] += h / 6.0 * k[i];
double* p_ki = k + i;
#include "tommy_rk4_protect_k.h"
double ki = *p_ki;
double hki = h / 6.0 * ki;
double* p_yi = y + i;
#include "tommy_rk4_protect_y.h"
(*p_yi) += hki;


//    ytmp[i] = y0[i] + hki;
double* p_y0i = (double*)(y0) + i;
#include "tommy_rk4_protect_y0.h"
double y0i = *p_y0i;
hki = 0.5 * h * ki;
double y0hki = y0i + hki;
double* p_ytmpi = ytmp + i;
#include "tommy_rk4_protect_ytmp.h"
(*p_ytmpi) = y0hki;
}

/* k2 step */
{
#define THIS_IS_FN_EVAL_K
#include "tommy_rk4_protect_k.h"
#define THIS_IS_FN_EVAL_YTMP
#include "tommy_rk4_protect_ytmp.h"
int s = GSL_ODEIV_FN_EVAL (sys, t + 0.5 * h, ytmp, k);

if (s != GSL_SUCCESS)
{
return s;
}
}

for (i = 0; i < dim; i++)
{
//    y[i] += h / 3.0 * k[i];
double* p_ki = k + i;
#include "tommy_rk4_protect_k.h"
double ki = *p_ki;
double hki = h / 3.0 * ki;
double* p_yi = y + i;
#include "tommy_rk4_protect_y.h"
(*p_yi) += hki;

//    ytmp[i] = y0[i] + 0.5 * h * ki;
double* p_y0i = (double*)(y0) + i;
#include "tommy_rk4_protect_y0.h"
double y0i = *p_y0i;
hki = 0.5 * h * ki;
double y0hki = y0i + hki;
double* p_ytmpi = ytmp + i;
#include "tommy_rk4_protect_ytmp.h"
(*p_ytmpi) = y0hki;
}

/* k3 step */
{
#define THIS_IS_FN_EVAL_K
#include "tommy_rk4_protect_k.h"
#define THIS_IS_FN_EVAL_YTMP
#include "tommy_rk4_protect_ytmp.h"
int s = GSL_ODEIV_FN_EVAL (sys, t + 0.5 * h, ytmp, k);

if (s != GSL_SUCCESS)
{
return s;
}
}

for (i = 0; i < dim; i++)
{
//    y[i] += h / 3.0 * k[i];
double* p_ki = k + i;
#include "tommy_rk4_protect_k.h"
double ki = *p_ki;
double hki = h / 3.0 * ki;
double* p_yi = y + i;
#include "tommy_rk4_protect_y.h"
(*p_yi) += hki;

//    ytmp[i] = y0[i] + h * k[i];
double* p_y0i = (double*)(y0) + i;
double y0i = *p_y0i;
hki = h * ki;
double y0hki = y0i + hki;
double* p_ytmpi = ytmp + i;
#include "tommy_rk4_protect_ytmp.h"
(*p_ytmpi) = y0hki;
}

/* k4 step */
{
#define THIS_IS_FN_EVAL_K
#include "tommy_rk4_protect_k.h"
#define THIS_IS_FN_EVAL_YTMP
#include "tommy_rk4_protect_ytmp.h"
int s = GSL_ODEIV_FN_EVAL (sys, t + h, ytmp, k);

if (s != GSL_SUCCESS)
{
return s;
}
}

for (i = 0; i < dim; i++)
{
//    y[i] += h / 6.0 * k[i];
double* p_ki = k + i;
#include "tommy_rk4_protect_k.h"
double ki = *p_ki;
double hki = h / 6.0 * ki;
double* p_yi = y + i;
#include "tommy_rk4_protect_y.h"
(*p_yi) += hki;
}

return GSL_SUCCESS;

} REAL_CATCH(0) {} REAL_END(0);

}


static int
rk4_apply (void *vstate,
size_t dim,
double t,
double h,
double y[],
double yerr[],
const double dydt_in[],
double dydt_out[],
const gsl_odeiv_system * sys)
{
	rk4_state_t *state = (rk4_state_t *) vstate;

	size_t i;

	double *const k = state->k;
	double *const k1 = state->k1;
	double *const y0 = state->y0;
	double *const y_onestep = state->y_onestep;

	DBL_MEMCPY (y0, y, dim);

	if (dydt_in != NULL)
	{
		DBL_MEMCPY (k, dydt_in, dim);
	}
	else
	{
		int s = GSL_ODEIV_FN_EVAL (sys, t, y0, k);

		if (s != GSL_SUCCESS)
		{
			return s;
		}
	}

	/* Error estimation is done by step doubling procedure */

	/* Save first point derivatives*/

	DBL_MEMCPY (k1, k, dim);

	/* First traverse h with one step (save to y_onestep) */

	DBL_MEMCPY (y_onestep, y, dim);

	{
	int s = rk4_step (y_onestep, state, h, t, dim, sys);

	if (s != GSL_SUCCESS)
		{
			return s;
		}
	}

	/* Then with two steps with half step length (save to y) */

	DBL_MEMCPY (k, k1, dim);

	{
		int s = rk4_step (y, state, h/2.0, t, dim, sys);

		if (s != GSL_SUCCESS)
		{
			/* Restore original values */
			DBL_MEMCPY (y, y0, dim);
			return s;
		}
	}

	/* Update before second step */
	{
	int s = GSL_ODEIV_FN_EVAL (sys, t + h/2.0, y, k);

		if (s != GSL_SUCCESS)
		{
		/* Restore original values */
		DBL_MEMCPY (y, y0, dim);
		return s;
		}
	}

	/* Save original y0 to k1 for possible failures */
	DBL_MEMCPY (k1, y0, dim);

	/* Update y0 for second step */
	DBL_MEMCPY (y0, y, dim);

	{
	int s = rk4_step (y, state, h/2.0, t + h/2.0, dim, sys);

	if (s != GSL_SUCCESS)
		{
		/* Restore original values */
		DBL_MEMCPY (y, k1, dim);
		return s;
		}
	}

	/* Derivatives at output */

	if (dydt_out != NULL) {
	int s = GSL_ODEIV_FN_EVAL (sys, t + h, y, dydt_out);

		if (s != GSL_SUCCESS)
		{
			/* Restore original values */
			DBL_MEMCPY (y, k1, dim);
			return s;
		}
	}

	/* Error estimation

	yerr = C * 0.5 * | y(onestep) - y(twosteps) | / (2^order - 1)

	constant C is approximately 8.0 to ensure 90% of samples lie within
	the error (assuming a gaussian distribution with prior p(sigma)=1/sigma.)

	*/

	for (i = 0; i < dim; i++)
	{
		yerr[i] = 4.0 * (y[i] - y_onestep[i]) / 15.0;
	}

	return GSL_SUCCESS;
}

static int
rk4_reset (void *vstate, size_t dim)
{
rk4_state_t *state = (rk4_state_t *) vstate;

DBL_ZERO_MEMSET (state->k, dim);
DBL_ZERO_MEMSET (state->k1, dim);
DBL_ZERO_MEMSET (state->y0, dim);
DBL_ZERO_MEMSET (state->ytmp, dim);
DBL_ZERO_MEMSET (state->y_onestep, dim);

return GSL_SUCCESS;
}

static unsigned int
rk4_order (void *vstate)
{
rk4_state_t *state = (rk4_state_t *) vstate;
state = 0; /* prevent warnings about unused parameters */
return 4;
}

static void
rk4_free (void *vstate)
{
rk4_state_t *state = (rk4_state_t *) vstate;
free (state->k);
free (state->k1);
free (state->y0);
free (state->ytmp);
free (state->y_onestep);
free (state);
}

static const gsl_odeiv_step_type rk4_type = { "rk4",    /* name */
1,                            /* can use dydt_in */
1,                            /* gives exact dydt_out */
&rk4_alloc,
&rk4_apply,
&rk4_reset,
&rk4_order,
&rk4_free
};

const gsl_odeiv_step_type *gsl_odeiv_step_rk4 = &rk4_type;
