//
//  main.cpp
//  QuantPricer
//
//  Created by Swati Mital on 06/03/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//


#include "OptionPriceBounds.h"
#include "fftw3.h"
#include "FFT/FastFourierTransform.h"

using namespace std;


#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "FFTMethods/BaseFFTMethod.h"
#include "CharacteristicFunctionMethods/VarianceGammaMethod.h"
#include "PricingEngine/FFTOptionPricer.h"

using namespace QuantPricer::FFTMethods;
using namespace QuantPricer::CharacteristicFunctionMethods;
using namespace QuantPricer::PricingEngine;
using namespace QuantPricer::FFT;
using namespace std;

int main(int argc, const char * argv[])
{
    double rf = 0.01;
    double q = 0.0;
    
    double T = 1.0;
    double St = 110.0;
    double K = 111;
    
    double sigma = 0.5;
    double nu = 0.5;
    double theta = -0.2;
    
    boost::shared_ptr<BaseFFTMethod> fft_method(new BaseFFTMethod(rf,q,T));
    boost::shared_ptr<VarianceGammaMethod> vg_method(new VarianceGammaMethod(theta, sigma,
                                                                             nu, St,
                                                                             rf,q,T));
    boost::shared_ptr<FFTOptionPricer> fft_option_pricer(new FFTOptionPricer());
    
    double price = fft_option_pricer->GetCallPrice(K, vg_method, fft_method);
    
    double bs_price = BlackScholesOptionPricer::BSPrice(St, K, 0, T, sigma, rf, q);
    
    std::cout << "Variance Gamma Call price is: " << price << std::endl;
    std::cout << "Black Scholes Option price is: " << bs_price << std::endl;
        
    QuantPricer::OptionPriceBounds::ComputeCallSpreadBounds();
    QuantPricer::OptionPriceBounds::ComputeCalendarSpreadBounds();
    QuantPricer::OptionPriceBounds::ComputeCallPricesAsFunctionOfK();
    QuantPricer::OptionPriceBounds::ComputeButterflyBounds();
    QuantPricer::OptionPriceBounds::ComputeStraddleBounds();
    QuantPricer::OptionPriceBounds::ComputeCallBounds();
    
}
