//      hatparams.h
//      
//      Copyright 2009 William Olson <wto@wto-virtualbox>
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


#ifndef HATPARAMS_H
#define HATPARAMS_H

class hatparams
{
	public:
		int N;
		double * M;
		double k;
		double G;
		hatparams(int argc, char** argv, double * t, double * t1,
				  double * h0, double * h1, double * accr, double y[]);
		bool success() {return SUCCESS;};
					
	private:		
		bool SUCCESS;
		void printHelp();
};

#endif /* HATPARAMS_H */ 
