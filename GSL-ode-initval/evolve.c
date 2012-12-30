/* ode-initval/evolve.c
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

/* Author:  G. Jungman
 */
#include <config.h>
#include <string.h>
#include <stdlib.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv.h>

#include "odeiv_util.h"

#include <gsl/tommy.h>
#include <gsl/triplicate.h>
#include <gsl/tommy_config.h>

gsl_odeiv_evolve *
gsl_odeiv_evolve_alloc (size_t dim)
{
  gsl_odeiv_evolve *e =
    (gsl_odeiv_evolve *) malloc (sizeof (gsl_odeiv_evolve));

  if (e == 0)
    {
      GSL_ERROR_NULL ("failed to allocate space for evolve struct",
                      GSL_ENOMEM);
    }

  e->y0 = (double *) malloc (dim * sizeof (double));

  if (e->y0 == 0)
    {
      free (e);
      GSL_ERROR_NULL ("failed to allocate space for y0", GSL_ENOMEM);
    }

  e->yerr = (double *) malloc (dim * sizeof (double));

  if (e->yerr == 0)
    {
      free (e->y0);
      free (e);
      GSL_ERROR_NULL ("failed to allocate space for yerr", GSL_ENOMEM);
    }

  e->dydt_in = (double *) malloc (dim * sizeof (double));

  if (e->dydt_in == 0)
    {
      free (e->yerr);
      free (e->y0);
      free (e);
      GSL_ERROR_NULL ("failed to allocate space for dydt_in", GSL_ENOMEM);
    }

  e->dydt_out = (double *) malloc (dim * sizeof (double));

  if (e->dydt_out == 0)
    {
      free (e->dydt_in);
      free (e->yerr);
      free (e->y0);
      free (e);
      GSL_ERROR_NULL ("failed to allocate space for dydt_out", GSL_ENOMEM);
    }

  e->dimension = dim;
  e->count = 0;
  e->failed_steps = 0;
  e->last_step = 0.0;

  return e;
}

int
gsl_odeiv_evolve_reset (gsl_odeiv_evolve * e)
{
  e->count = 0;
  e->failed_steps = 0;
  e->last_step = 0.0;
  return GSL_SUCCESS;
}

void
gsl_odeiv_evolve_free (gsl_odeiv_evolve * e)
{
  RETURN_IF_NULL (e);
  free (e->dydt_out);
  free (e->dydt_in);
  free (e->yerr);
  free (e->y0);
  free (e);
}

/* Evolution framework method.
 *
 * Uses an adaptive step control object
 */

static int evolve_call_count = 100;
static bool is_alloced = false;
static gsl_odeiv_evolve* e_bak;
static gsl_odeiv_control* con_bak;
static gsl_odeiv_step* step_bak;
static gsl_odeiv_system* dydt_bak;
static int dimension;
static double* t_bak;
static double* h_bak;
static double* y_bak;
static unsigned long y_0, y_1, y_2;

int gsl_odeiv_evolve_apply_actual(gsl_odeiv_evolve* e,
	gsl_odeiv_control* con, gsl_odeiv_step* step, const gsl_odeiv_system* dydt,
	double* t, double t1, double* h, double y[]);

int gsl_odeiv_evolve_apply(gsl_odeiv_evolve* e,
	gsl_odeiv_control* con, gsl_odeiv_step* step, const gsl_odeiv_system* dydt,
	double* t, double t1, double* h, double y[]) 
{
// FT
	int ret;
#ifdef FT3
	if(evolve_call_count++ >= 100) {
		if(is_alloced == false) {	
			e_bak = (gsl_odeiv_evolve*)malloc(sizeof(gsl_odeiv_evolve));
			con_bak = (gsl_odeiv_control*)malloc(sizeof(gsl_odeiv_control));
			step_bak = (gsl_odeiv_step*)malloc(sizeof(gsl_odeiv_step));
			dydt_bak = (gsl_odeiv_system*)malloc(sizeof(gsl_odeiv_system));
			dimension = e->dimension;
			y_bak = (double*)malloc(sizeof(double) * dimension);
			is_alloced = true;
			TRIPLICATE(y, y_0, y_1, y_2);
		}
		memcpy(e_bak, e, sizeof(gsl_odeiv_evolve));
		memcpy(con_bak, con, sizeof(gsl_odeiv_control));
		memcpy(step_bak, step, sizeof(gsl_odeiv_step));
		memcpy(dydt_bak, dydt, sizeof(gsl_odeiv_system));
		memcpy(y_bak, y, sizeof(double)*dimension);
	}
	int jmpret;
	SUPERSETJMP("Before gsl_odeiv_evolve_apply_actual");
	if(jmpret != 0) {
		memcpy(e, e_bak, sizeof(gsl_odeiv_evolve));
		memcpy(con, con_bak, sizeof(gsl_odeiv_control));
		memcpy(step, step_bak, sizeof(gsl_odeiv_step));

		gsl_odeiv_system* p_dydt = (gsl_odeiv_system*)dydt;
		memcpy(p_dydt, dydt_bak, sizeof(gsl_odeiv_system));
		
		memcpy(y, y_bak, sizeof(double)*dimension);
		TRI_RECOVER(y_0, y_1, y_2);
		if((long)y != y_0) y = (double*)y_0;
	}
#endif
	ret = gsl_odeiv_evolve_apply_actual(e, con, step, dydt, t, t1, h, y);
	return ret;
}

int
gsl_odeiv_evolve_apply_actual (gsl_odeiv_evolve * e,
                        gsl_odeiv_control * con,
                        gsl_odeiv_step * step,
                        const gsl_odeiv_system * dydt,
                        double *t, double t1, double *h, double y[])
{

  const double t0 = *t;
  double h0 = *h;
  int step_status;
  int final_step = 0;
  double dt = t1 - t0;  /* remaining time, possibly less than h */

  if (e->dimension != step->dimension)
    {
      GSL_ERROR ("step dimension must match evolution size", GSL_EINVAL);
    }

  if ((dt < 0.0 && h0 > 0.0) || (dt > 0.0 && h0 < 0.0))
    {
      GSL_ERROR ("step direction must match interval direction", GSL_EINVAL);
    }

  /* No need to copy if we cannot control the step size. */

  if (con != NULL)
    {
      DBL_MEMCPY (e->y0, y, e->dimension);
    }

  /* Calculate initial dydt once if the method can benefit. */

  if (step->type->can_use_dydt_in)
    {
      int status = GSL_ODEIV_FN_EVAL (dydt, t0, y, e->dydt_in);

      if (status) 
        {
          return status;
        }
    }

	bool is_my_retried = false;

try_step:
    
  if ((dt >= 0.0 && h0 > dt) || (dt < 0.0 && h0 < dt))
    {
      h0 = dt;
      final_step = 1;
    }
  else
    {
      final_step = 0;
    }

  if (step->type->can_use_dydt_in)
    {
      step_status =
        gsl_odeiv_step_apply (step, t0, h0, y, e->yerr, e->dydt_in,
                              e->dydt_out, dydt);
    }
  else
    {
      step_status =
        gsl_odeiv_step_apply (step, t0, h0, y, e->yerr, NULL, e->dydt_out,
                              dydt);
    }

  /* Check for stepper internal failure */

  if (step_status != GSL_SUCCESS) 
    {
      *h = h0;  /* notify user of step-size which caused the failure */
      *t = t0;  /* restore original t value */
      return step_status;
    }

  e->count++;
  e->last_step = h0;

  if (final_step)
    {
      *t = t1;
    }
  else
    {
      *t = t0 + h0;
    }

  if (con != NULL)
    {
      /* Check error and attempt to adjust the step. */

      double h_old = h0;

      const int hadjust_status 
        = gsl_odeiv_control_hadjust (con, step, y, e->yerr, e->dydt_out, &h0);

	  #ifdef INTEGRATOR_THRESH
	  if(is_my_retried == false)
	  {
	  	for(int i=0; i<dydt->dimension; i++) {
			double dydt1 = e->dydt_out[i];
			if(fabs(y[i] - e->y0[i]) > fabs(dydt1 * h0 * INTEGRATOR_THRESH)) {
				fprintf(stderr, "========MyReTry--------\n");
                DBL_MEMCPY (y, e->y0, dydt->dimension);
				is_my_retried = true;
                goto try_step;
			}
		}
	  }
	  #endif

      if (hadjust_status == GSL_ODEIV_HADJ_DEC)
        {
          /* Check that the reported status is correct (i.e. an actual
             decrease in h0 occured) and the suggested h0 will change
             the time by at least 1 ulp */

          double t_curr = GSL_COERCE_DBL(*t);
          double t_next = GSL_COERCE_DBL((*t) + h0);

          if (fabs(h0) < fabs(h_old) && t_next != t_curr) 
            {
              /* Step was decreased. Undo step, and try again with new h0. */
              DBL_MEMCPY (y, e->y0, dydt->dimension);
              e->failed_steps++;
              goto try_step;
            }
          else
            {
              h0 = h_old; /* keep current step size */
            }
        }
    }

  *h = h0;  /* suggest step size for next time-step */

  return step_status;
}
