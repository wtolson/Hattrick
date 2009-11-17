//      hattrick.c
//      
//      Copyright 2009 William Olson <wtolson@gmail.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.


#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>
#include <gsl/gsl_const_mksa.h>
#include <math.h>

#define N 2

int
func (double t, const double y[], double f[],
	  void *params)
{
	double * M = (double *) params;
	int i,j;
	for(i=0; i<8*N; i++)
	{
		if ((i%8%3) == 0)
		{
			f[i] = y[i+1];			
		} else if ((i%8%3) == 1) {
			f[i] = 0;
			for(j=(i%8); j<8*N; j+=8){
				if (i/8!=j/8)
				{
					double x[3];
					x[0] = y[8*(j/8)] - y[8*(i/8)];
					x[1] = y[8*(j/8)+3] - y[8*(i/8)+3];
					x[2] = y[8*(j/8)+6] - y[8*(i/8)+6];
					double r = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
					
					f[i] += M[j/8] * (y[j-1]-y[i-1]) / (r*r*r);
				}
			}
		} else {
			f[i] = 0.0;
		}
	}
	return GSL_SUCCESS;
}
/*
int
jac (double t, const double y[], double *dfdy, 
     double dfdt[], void *params)
{
	double * M = (double *) params;
	gsl_matrix_view dfdy_mat 
		= gsl_matrix_view_array (dfdy, 6*N, 6*N);
	gsl_matrix * m = &dfdy_mat.matrix;
	
	
	int i,j,k;
	for (i=0; i<6*N; i++)
	{
		for (j=0; j<6*N; j++)
		{
			if ((i%2) == 0)
			{
				if (i+1 == j)
				{
					gsl_matrix_set (m, i, j, 1.0);
				} else {
					gsl_matrix_set (m, i, j, 0.0);
				}
			} else {
				if ((j%2) == 1) {
					gsl_matrix_set (m, i, j, 0.0);
				} else if (i == j+1){
					double ftemp = 0;
					for(k=(i%6); k<6*N; k+=6)
					{
						if (i/6!=k/6)
						{
							double r = sqrt(
											(y[6*(k/6)] - y[6*(i/6)]) * (y[6*(k/6)] - y[6*(i/6)]) +
											(y[6*(k/6)+2] - y[6*(i/6)+2]) * (y[6*(k/6)+2] - y[6*(i/6)]+2) +
											(y[6*(k/6)+4] - y[6*(i/6)+4]) * (y[6*(k/6)+4] - y[6*(i/6)]+4) );
							ftemp -= 3 * M[j/6] * (y[k-1]-y[i-1]) * (y[k-1]-y[i-1]) / (r*r*r*r*r);
							ftemp += M[j/6] * y[i-1] / (r*r*r);
						}
					}					
					gsl_matrix_set (m, i, j, ftemp);
				} else if (i/6 == j/6) {
					double ftemp = 0;
					for(k=(i%6); k<6*N; k+=6)
					{
						if (i/6!=k/6)
						{
							double r = sqrt(
											(y[6*(k/6)] - y[6*(i/6)]) * (y[6*(k/6)] - y[6*(i/6)]) +
											(y[6*(k/6)+2] - y[6*(i/6)+2]) * (y[6*(k/6)+2] - y[6*(i/6)]+2) +
											(y[6*(k/6)+4] - y[6*(i/6)+4]) * (y[6*(k/6)+4] - y[6*(i/6)]+4) );
							ftemp -= 3 * M[j/6] * (y[k-1]-y[i-1]) * (y[6*(i/6) + j%6]-y[j]) / (r*r*r*r*r);
						}
					}					
					gsl_matrix_set (m, i, j, ftemp);
				} else if (i%6 == (j+1)%6) {
					double r = sqrt(
									(y[6*(j/6)] - y[6*(i/6)])*(y[6*(j/6)] - y[6*(i/6)]) +
									(y[6*(j/6)+2] - y[6*(i/6)+2])*(y[6*(j/6)+2] - y[6*(i/6)]+2) +
									(y[6*(j/6)+4] - y[6*(i/6)+4])*(y[6*(j/6)+4] - y[6*(i/6)]+4) );
					gsl_matrix_set (m, i, j, (M[j/6]/(r*r*r)) * (3*(y[j]-y[i-1])*(y[j]-y[i-1])/(r*r) - y[j]));
				} else {
					double r = sqrt(
									(y[6*(j/6)] - y[6*(i/6)])*(y[6*(j/6)] - y[6*(i/6)]) +
									(y[6*(j/6)+2] - y[6*(i/6)+2])*(y[6*(j/6)+2] - y[6*(i/6)]+2) +
									(y[6*(j/6)+4] - y[6*(i/6)+4])*(y[6*(j/6)+4] - y[6*(i/6)]+4) );
					gsl_matrix_set (m, i, j, (M[j/6]/(r*r*r)) * (3*(y[6*(j/6) + (i-1)%6]-y[i-1])*(y[j]-y[6*(i/6) + j%6])/(r*r)));
				}
			}
		}
	}
	
	
	
	for (i=0; i<6*N; i++)
	{
		dfdt[i] = 0.0;
	}
	
	return GSL_SUCCESS;
}
*/

int
main (void)
{	
	const gsl_odeiv_step_type * T 
		= gsl_odeiv_step_rkf45;
		//= gsl_odeiv_step_bsimp;
		
	gsl_odeiv_step * s 
		= gsl_odeiv_step_alloc (T, 8*N);
	gsl_odeiv_control * c 
		= gsl_odeiv_control_y_new (1e-2, 0.0);
	gsl_odeiv_evolve * e 
		= gsl_odeiv_evolve_alloc (8*N);
		
	
	double k = 0.01720209895;
	double G = k*k;
	double M[N];
	M[0] = G;
	M[1] = G/328900.56;
		
	//gsl_odeiv_system sys = {func, jac, 8*N, M};
	gsl_odeiv_system sys = {func, NULL, 8*N, M};
	
	double t = 0.0, t1 = 365.25;
	double h0 = 1E-1;
	double h = h0;
	double sp = 0.0;
	double y[8*N] = { 0.0, 0.0, sp, 0.0, 0.0, sp, 0.0, 0.0, 1, 0.0, sp, 0.0, k, sp, 0.0, 0.0 };
	
	
	while (t < t1)
	{
		int status = gsl_odeiv_evolve_apply (e, c, s,
		                                     &sys,
											 &t, t1,
											 &h, y);
											 
											 
		if (status != GSL_SUCCESS)
			break;
		
		printf("%f", t);
		h = h0;
		int i;
		for(i = 0; i < 8*N; i++) {
			if (i!=2 && i!=5)
			printf (" %f", y[i]);
		}
		printf("\n");
	}
	
	gsl_odeiv_evolve_free (e);
	gsl_odeiv_control_free (c);
	gsl_odeiv_step_free (s);
	return 0;
}
