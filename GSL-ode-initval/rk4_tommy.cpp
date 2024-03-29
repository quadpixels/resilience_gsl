#include "../real.h"
#include <gsl/tommyptr.hpp>
#include <gsl/tommyint.hpp>
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

#include "odeiv_util.h"

// Deprecated
/* Tommy's addition on July 24: Pointer triplication. */
//#define TRIPLICATION_RK4
//#define DUPLICATION_RK4
/*#define TRIO_CORRECT(a,b,c) {\
	if(a!=b) { if(a==c) b=c; else a=c; } \
	else { if(c!=a) c=a; } \
}*/

typedef struct
{
  double *k;
  double *k1;
  double *y0;
  double *ytmp;
  double *y_onestep;
}
rk4_state_t;

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

static int
rk4_step (double *y, const rk4_state_t *state,
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
  
  // Tommy 07-05-12: Back up ptrs.
  MyDoublePtr my_y(y, "y", 2);
  MyDoublePtr my_y0(y0, "y0", 2);
  MyDoublePtr my_ytmp(ytmp, "ytmp", 2);
  MyDoublePtr my_k(k, "k", 2);

  size_t i, i1, i2;

  /* k1 step */
REAL_TRY(0) {
  MyInt my_i1(0, "i1");
  for (my_i1 = 0; my_i1 < dim; my_i1++)
    {
      my_y[my_i1] += h / 6.0 * my_k[my_i1];
      my_ytmp[my_i1] = my_y0[my_i1] + 0.5 * h * my_k[my_i1];
    }
} REAL_CATCH(0) {} REAL_END(0);
  /* k2 step */
  {
REAL_TRY(1) {
    int s = GSL_ODEIV_FN_EVAL (sys, t + 0.5 * h, my_ytmp, my_k);
    if (s != GSL_SUCCESS)
      {
	return s;
      }
} REAL_CATCH(1) {} REAL_END(1);
  }

REAL_TRY(2) {
  MyInt my_i2(0, "i2");
  for (my_i2 = 0; my_i2 < dim; my_i2++)
    {
      my_y[my_i2] += h / 3.0 * my_k[my_i2];
      my_ytmp[my_i2] = my_y0[my_i2] + 0.5 * h * my_k[my_i2];
    }
} REAL_CATCH(2) {} REAL_END(2);

REAL_TRY(3) {
  /* k3 step */
  {
    int s = GSL_ODEIV_FN_EVAL (sys, t + 0.5 * h, my_ytmp, my_k);
    if (s != GSL_SUCCESS)
      {
	return s;
      }
  }
} REAL_CATCH(3) {} REAL_END(3);

REAL_TRY(4) {
  MyInt my_i3(0, "i3");
  for (my_i3 = 0; my_i3 < dim; my_i3++)
    {
      my_y[my_i3] += h / 3.0 * my_k[my_i3];
      my_ytmp[my_i3] = my_y0[my_i3] + h * my_k[my_i3];
    }
} REAL_CATCH(4) {} REAL_END(4);

  /* k4 step */
REAL_TRY(5) {
  {
    int s = GSL_ODEIV_FN_EVAL (sys, t + h, my_ytmp, my_k);

    if (s != GSL_SUCCESS)
      {
	return s;
      }
  }
} REAL_CATCH(5) {} REAL_END(5);

REAL_TRY(6) {
  MyInt my_i4(0, "i4");
  for (my_i4 = 0; my_i4 < dim; my_i4++)
    {
      my_y[my_i4] += h / 6.0 * my_k[my_i4];
    }
} REAL_CATCH(6) {} REAL_END(6);

  return GSL_SUCCESS;
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
