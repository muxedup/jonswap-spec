//
//  jonswapTest.cpp
//  
//  Use makefile to create the executable: jonswap
//
//  Created by Munan Xu on 7/2/15.
//
//
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include "jonswapSpec.h"

using std::ostream;
using std::ofstream;
using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    
    vector<double> w;
    vector<double> dist;
    vector<double> wc;
    vector<double> amps;
    set<double> bounds;
    vector<double>::iterator wc_it;
    vector<double>::iterator amps_it;
    set<double>::iterator bounds_it;
    vector<double>::iterator it;
    vector<double>::iterator dist_it;
    vector<double> paddleAmps;
    double max_freq;
    int count = 0;
	int nbins = 10;
//  nbins can be changed during execution: for example, 'jonswap 20'  
	if(argc > 1) {
		nbins = atoi(argv[1]);
	}
	
// initialize spectrum with alpha, peak radial frequency, and max radial frequency
    max_freq = 6.0;
    jonswapSpec jonswap = jonswapSpec(.05, 3.5, max_freq);
// specify U_10 and F to get gamma,alpha, peak radial frequency, max radial freq, s1, and s2
//	jonswapSpec jonswap2 = jonswapSpec(15, 2e4);
    
    jonswap.bin(nbins);          // sets number of bins, finds bounds and center values of frequency
    cout<<"bins ok" <<endl;
    bounds = jonswap.getBins();  // returns bin bounds
    wc = jonswap.getWCs(); //wc  bin center angular frequencies (method defined in .h file)
    cout <<"size of wc: " <<wc.size()<<endl;
    amps = jonswap.calcBinAmps(20);  // calculate the bin average amplitude
    double depth = 0.4;
    paddleAmps = jonswap.calcPaddleAmps(depth);
	cout << "parametrized jonswap constructor:" << endl;
    cout << jonswap;
//	cout << "computed parameter jonswap constructor:" << endl;
//	cout << jonswap2;
    cout << "\n------------------------------------\n" << endl;
    
    cout << "#\tBin\t\t\tW_c\t\tAmp" << endl;
    // print out sampled spectrum
    amps_it = amps.begin();
    wc_it = wc.begin();
   
    ofstream data0;
    data0.open("jonswap_sample.txt",std::ofstream::out|ofstream::trunc);
    
    for (bounds_it = bounds.begin(); bounds_it != prev(bounds.end()); ++bounds_it) {
		cout << count << "\t";
        if (bounds_it == bounds.begin()) {
            cout << "0 - " << *bounds_it << "\t\t: " << *wc_it << "\t" << *amps_it << endl;
            data0 << *wc_it <<"\t"<<*amps_it <<endl;
        } else {
            cout << *prev(bounds_it) << " - " << *bounds_it << "\t: " << *wc_it << "\t" << *amps_it << endl;
            data0 << *wc_it <<"\t"<<*amps_it <<endl;
        }
        
        wc_it++;
        amps_it++;
		count++;
    }
    
    cout << count << "\t";
    cout << *prev(bounds.end()) << " - " << jonswap.getWmax() << "\t\t: " << *wc_it << "\t" << *amps_it << endl;
    data0 << *wc_it <<"\t"<<*amps_it <<endl;
    data0.close();
    
    // Calculate the actual spectrum for comparison
    double di=max_freq/200.;
    for (double i = 0.0; i < max_freq; i += di )
        w.push_back(i);
    
    dist = jonswap.getamp(w);
    
    dist_it = dist.begin();
    
    ofstream data;
    data.open("jonswap_spec.txt",std::ofstream::out|std::ofstream::trunc);
    
//    data << "w\t\tamp"<< endl;
    for (it = w.begin(); it != w.end(); ++it) {
        data << *it << "\t" << *dist_it <<endl;
        dist_it++;
    }
    data.close();
    
    ofstream data2;
    data2.open("jonswap_amps.txt",std::ofstream::out|std::ofstream::trunc);
    amps_it = paddleAmps.begin();
//    data << "w\t\tamp"<< endl;
    for (it = wc.begin(); it != wc.end(); ++it) {
        data2 << *it << "\t" << *amps_it <<endl;
        amps_it++;
    }
    data2.close();
}