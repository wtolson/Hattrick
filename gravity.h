/***********************************************************************
 * gravity.h
 *
 * Copyright 2009 William Olson <wtolson@gmail.com>
 * GNU General Public License v3
 * 
 * Header file for gravity.cpp
 * 
 */

int func (double t, const double y[], double f[], void *params);


int jac (double t, const double y[], double *dfdy, double dfdt[],
		 void *params);
