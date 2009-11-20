/***********************************************************************
 * hatparams.cpp
 *
 * Copyright 2009 William Olson <wtolson@gmail.com>
 * GNU General Public License v3
 * 
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "hatparams.h"

using namespace std;

hatparams::hatparams(int argc, char** argv)
{
	k = 0.01720209895;
	G = k*k;
	this->SUCCESS = true;
	
	if (argc == 1) {
		this->SUCCESS = false;
		printHelp();
		return;
	}
	
	this->SUCCESS = true;
	
	ifstream ifs ( argv[1] , ifstream::in );
	
	if (!ifs.is_open()) {
		this->SUCCESS = false;
		cerr << "The file " << argv[1] << " does not exist." << endl;
		printHelp();
		return;
	}
	
	int n1, n2=0;
	
	ifs >> n1;
	ifs >> t0;
	ifs >> t1;
	ifs >> h0;
	ifs >> h1;
	ifs >> accr;
	ifs >> printSkip;
	ifs >> stepType;
	
	if (stepType!=0 && stepType!=1) {
		this->SUCCESS = false;
		cerr << "Invalid integration step type." << endl;
		printHelp();
		return;
	}
	
	if (argc >= 3)
		n2 = atoi(argv[2]);
	
	if (argc >= 3 && argc != (3+2*n2)) {
		this->SUCCESS = false;
		if (argc > (3+2*n2))
			cerr << "Too many arguments." << endl;
		if (argc < (3+2*n2))
			cerr << "Too few arguments." << endl;
		printHelp();
		return;
	}
	
	N = n1+n2;
	y = new double[9*N];
	M = new double[N];
	for(int i = 0; i < n1; i++)
	{
		ifs >> M[i];
		M[i] *= G;
		for(int k=0; k<3; k++) ifs >> y[this->vi(i,k)];
		for(int k=0; k<3; k++) ifs >> y[this->ai(i,k)];
		for(int k=0; k<3; k++) y[9*i + 3*k + 2] = 0.0;
	}
	ifs.close();
	
	for(int i=n1; i<N; i++) {
		M[i] = G*atof(argv[3+2*(i-n1)]);  // M
		for (int j=0; j<9; j++) y[9*i + j] = 0.0;
		y[9*i] = atof(argv[4+2*(i-n1)]);  // x
		y[9*i + 4] = sqrt(M[0]/y[9*i]);   // vy
	}	
}

void hatparams::print(double t) {
	cout << t << " " << h0;
	for(int i = 0; i < 9*N; i++) {
		if (i%3!=2) cout << " " << y[i];
	}
	cout << endl;
}

	
void hatparams::printHelp() {
	cerr << "Input format:" << endl;
	cerr << "    ./hattrick baseSys" << endl;	
	cerr << "    ./hattrick baseSys n m1 r1 ... mn rn"  << endl << endl;
	
	cerr << "Where:"  << endl;
	cerr << "    baseSys: Name of the file specifying the base system." << endl;
	cerr << "    n (optional): Number of perturbing bodies." << endl;
	cerr << "    mn (optional): Mass of the nth perturbing body." << endl;
	cerr << "    rn (optional): Radius of the nth perturbing body." << endl << endl;
	
	cerr << "Base system file format:" << endl;
	cerr << "    n t0 t1 h0 h1 accr printSkip stepType" << endl;
	cerr << "    m1 x1 y1 z1 vx1 vy1 vz1" << endl;
	cerr << "    m2 x2 y2 z2 vx2 vy2 vz2" << endl;
	cerr << "        ..." << endl;
	cerr << "    mn xn yn zn vxn vyn vzn" << endl << endl;
	
	cerr << "Where:"  << endl;
	cerr << "    n: Number of bodies in the base system." << endl;
	cerr << "    t0: Initial system time." << endl;
	cerr << "    t1: Final system time." << endl;
	cerr << "    h0: Initial time step." << endl;
	cerr << "    h1: Max time step." << endl;
	cerr << "    accr: Accuracy paramater." << endl;
	cerr << "    skipPrint: The time skiped between prints." << endl;
	cerr << "        -1 for print orbits only." << endl;
	cerr << "    stepType: 0 for rk45, 1 for bsimp." << endl;
}


