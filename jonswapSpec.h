#ifndef JONSWAPSPEC_H
#define JONSWAPSPEC_H 

#define USE_CPP11 0

#include <vector>
#include <set>
#include <iterator>
#include <iostream>
#include <random>
#include <cmath>
#include <time.h>

using std::ostream;
using std::cout;
using std::endl;
using std::vector;
using std::set;

#if USE_CPP11
using std::random_device;
using std::normal_distribution;
#endif

class jonswapSpec
{
    friend ostream &operator<<(ostream &, const jonswapSpec &);
	
public:
	jonswapSpec (double alpha, double wp, double wmax, double gamma = 3.3, double s1 = 0.07, double s2 = 0.09);
	jonswapSpec(double vel10, double F);
	double getamp(double w);
	void bin(int n);
    
    set<double> getBins() { return bounds; }
    
    vector<double> calcBinAmps(int integ_interval);
    
  //  set<double> getAmps() { return amps; }
    
    vector<double> getWCs(){ return wc; }
    
    double getWmax() { return wmax; }
    
    vector<double> getamp(vector<double> w);
    
    vector<double> calcPaddleAmps(double h);
    
	
	virtual ~jonswapSpec ();

private:
	double alpha, wp, wmax, gamma, s1, s2;
	double vel10, F;
    set<double> bounds;
    vector<double> wc;
    vector<double> amps;
    vector<double> paddleAmps;
	
	double g;
    double calcAlpha();
    double calcWp();
};

#endif