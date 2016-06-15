//
//  main.cpp
//  QuantPricer
//
//  Created by Swati Mital on 06/03/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//


#include "OptionPriceBounds.h"
#include "fftw3.h"

using namespace std;

int main(int argc, const char * argv[])
{
    int N = 10;
    fftw_complex *in, *out;
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    
    
    
    fftw_plan p;
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);
    
    QuantPricer::OptionPriceBounds::ComputeCallSpreadBounds();
    QuantPricer::OptionPriceBounds::ComputeCalendarSpreadBounds();
    QuantPricer::OptionPriceBounds::ComputeCallPricesAsFunctionOfK();
    QuantPricer::OptionPriceBounds::ComputeButterflyBounds();
    QuantPricer::OptionPriceBounds::ComputeStraddleBounds();
    QuantPricer::OptionPriceBounds::ComputeCallBounds();
    
}
