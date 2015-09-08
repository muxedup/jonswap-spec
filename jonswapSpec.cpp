//
//  jonswapSpec.cpp
//  
//
//  Created by Munan Xu on 6/30/15.
//
//

#include <stdio.h>
#include "jonswapSpec.h"


// Default constructor uses pre-defined jonswap parameters
// alpha - ratio of windspeed to fetch
// wp - dominant period
// wmax - upper bound of jonswap spectrum
// gamma - peak sharpening factor
// s1 - variance when w =< wp
// s2 - variance when w > wp
jonswapSpec::jonswapSpec(double alpha, double wp, double wmax, double gamma, double s1, double s2) {
    this->alpha = alpha;
    this->wp = wp;
    this->wmax = wmax;
    this->gamma = gamma;
    this->s1 = s1;
    this->s2 = s2;
	
	this->F = 0.0;
	this->vel10 = 0.0;
	
    g = 9.81;
}

// Initializer that calculates alpha and wp based on:
// vel10 : wind speed at 10m (m/s)
// F : fetch (m) (length of ocean constant wind speed)
jonswapSpec::jonswapSpec(double vel10, double F) {
    
    this->vel10 = vel10;
    this->F = F;
    g = 9.81;
    
    alpha = calcAlpha();
    wp = calcWp();
    
    wmax = 33*wp / (2*M_PI);
    
    gamma = 3.3;
    s1 = 0.7;
    s2 = 0.9;
}

jonswapSpec::~jonswapSpec() {
    
}


// Calculate amplitude of jonswap spectrum for specific angular velocity
double jonswapSpec::getamp(double w) {
	
	// Compute intermediate values:
    double s = (w > wp ? s2 : s1); // set sigma 
    double dw = w - wp;
    double rexp_denom = s * wp ;
    double rexp = -pow(dw/rexp_denom, 2) / 2.0;
    double r = exp(rexp);
    
    double gammaR = pow(gamma,r);
    
    double jdistExp = -1.2 * pow(wp/w,4);
    double c = alpha * g * g * pow(w, -5); // alpha g^2/w^5
    
    return c * exp(jdistExp) * gammaR;
    
}

// Calculate amplitudes of jonswap spectrum for a vector of angular velocities
vector<double> jonswapSpec::getamp(vector<double> w) {
    vector<double> amps;
    vector<double>::iterator it;
    double amp;
    
    for ( it = w.begin(); it != w.end(); ++it) {
        amp = getamp(*it);
        amps.push_back(amp);
    }
    return amps;
}

// Randomly generate boundaries for N bins and calculate their center frequency
void jonswapSpec::bin(int n) {
	
#if USE_CPP11
	random_device gen;
    normal_distribution<double> distribution(wp, wp/2);
    double bound = distribution(gen);
#else
	srand(time(NULL));
	double bound;
	double range = 0.05 * (wmax / n); // range is +- 2.5% of bin width 
	double offset = -wmax / (2*n); // center range around 0
#endif
	
	set<double>::iterator it;
    
    for (int i = 1; i < n; i++) {
		bound = i * wmax/n + ((double) rand() / RAND_MAX) * range + offset;
		bounds.insert(bound);
    }
    
    for (it = bounds.begin(); it != bounds.end(); ++it) {
        if (it == bounds.begin()) {
            wc.push_back(*it/2);
        } else {
            wc.push_back((*it + *next(it))/2);
        }
    }
	
    wc.push_back((*prev(bounds.end()) + wmax)/2);
}

// Calculate alpha based on wind speed and fetch
// Helper function for automatic parameter calculation constructor
double jonswapSpec::calcAlpha() {
    double velDivF = vel10/F;
    
    double tmp = velDivF * vel10 / g;
    
    return 0.076 * pow(tmp, 0.22);
}

// Calculate peak angular velocity of jonswap spectrum from wind speed and fetch
// Helper function for automatic parameter calculation constructor
double jonswapSpec::calcWp() {
    double velxF = vel10*F;
    double g2 = g*g;
    
    return 22 * cbrt(g2/velxF);
}


// Integrate and normalize jonswap spectrum using discrete trapezoidal integration
// dw - specify the integration step size
// max_stroke - maximum horizontal stroke at free surface
vector<double> jonswapSpec::calcPaddleAmps(double dw, double max_stroke) {
    set<double>::iterator it = bounds.begin();
    double area = 0;
    double total_area = 0;
    
    for (double i = dw; i < wmax; i += dw) {
        if (i >= *it && it != bounds.end()) {
            amps.push_back(area);
            
            total_area += area;

            area = 0;
            it++;
        }
        
        double b1 = getamp(i);
        double b2 = getamp(i + dw);
        area += dw*(b1 + b2)/2;
        
    }
    
    total_area += area;
    amps.push_back(area);
	
    cout << "finished calculating areas... total area is: " << total_area << endl;
    
    for (vector<double>::iterator i = amps.begin(); i != amps.end(); i++) {
        *i = (*i / total_area) * max_stroke; // Compute the power or energy here???
    }
    return amps;
}

ostream &operator<<(ostream &output, const jonswapSpec & jonswap) {
    output << "jonswapSpec params:" << endl
        << "alpha\t: " << jonswap.alpha << endl
        << "gamma\t: " << jonswap.gamma << endl
        << "w_p\t: " << jonswap.wp << endl
        << "w_max\t: " << jonswap.wmax << endl
        << "s1\t: " << jonswap.s1 << " | (w <= w_p)" << endl
        << "s2\t: " << jonswap.s2 << " | (w > w_p)" << endl;
    if (jonswap.vel10 > 0 && jonswap.F > 0) {
        output << "vel10\t: " << jonswap.vel10 << endl
            << "F\t: " << jonswap.F << endl;
    }
    if (jonswap.amps.size()) {
	    output << "Nbins\t: " << jonswap.bounds.size() + 1 << endl;
	    output << "Amps\t: [ 1 x " << jonswap.amps.size() << " ]\n";
	    output << "W_c\t: [ 1 x " << jonswap.wc.size() << " ]\n";
    }
    
    return output;
}



