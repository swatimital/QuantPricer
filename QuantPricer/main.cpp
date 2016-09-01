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
#include "CharacteristicFunctionMethods/BrownianMotionMethod.h"
#include "CharacteristicFunctionMethods/HestonStochVolMethod.h"
#include "PricingEngine/FFTOptionPricer.h"
#include "Calibration/ImpliedVolatility.h"

using namespace QuantPricer::FFTMethods;
using namespace QuantPricer::CharacteristicFunctionMethods;
using namespace QuantPricer::PricingEngine;
using namespace QuantPricer::FFT;
using namespace QuantPricer::Calibration;
using namespace std;

int main(int argc, const char * argv[])
{
    double rf = 0.05;
    double q = 0.03;
    
    double T = 0.25;
    double St = 100.0;
    double K[] = {5.0, 10.0, 33.0, 43.0, 53.0, 60.0, 70.0, 74.0, 77.0, 80.0, 83.0, 90.0, 97.0, 100.0, 120.0, 130.0, 140.0, 150.0, 200.0};
    
    double sigma = 0.81;
    double nu = 2.0;
    double theta = 0.10;
    
    double V0 = 0.1568;
    double corr = -0.74;
    double mean_rev_speed = 5.73;
    double long_term_var = 0.05;
    double vol_of_vol = sigma;
    
        
    boost::shared_ptr<BaseFFTMethod> fft_method(new BaseFFTMethod());
    boost::shared_ptr<VarianceGammaMethod> vg_method(
                    new VarianceGammaMethod(theta, sigma, nu, St, rf, q, T));
    boost::shared_ptr<BrownianMotionMethod> bm_method(new BrownianMotionMethod(sigma, St, rf, q, T));
    boost::shared_ptr<HestonStochVolMethod> hs_method(
                     new HestonStochVolMethod(V0, mean_rev_speed, long_term_var,                                                                               vol_of_vol, corr, St, rf, q, T));
                                              
                                                                               
    
    boost::shared_ptr<FFTOptionPricer> fft_option_pricer(new FFTOptionPricer());
    
    
    for (bool call: {true})
    {
        auto atm_option_prices = fft_option_pricer->PrecomputeFFTOptionPrices(St, rf, q, T, call, true, hs_method, fft_method);
        auto otm_option_prices = fft_option_pricer->PrecomputeFFTOptionPrices(St, rf, q, T, call, false, hs_method, fft_method);
        
        ///// Start serialization
        std::ofstream atm_fft_file;
        std::ofstream otm_fft_file;
        atm_fft_file.open("/Users/swatimital/GitHub/QuantPricer/Results/FFTATMCallPrices.csv");
        otm_fft_file.open("/Users/swatimital/GitHub/QuantPricer/Results/FFTOTMCallPrices.csv");
        
        for (std::pair<double, double> opt: atm_option_prices)
        {
            atm_fft_file << opt.first << "," << opt.second << std::endl;
        }
        atm_fft_file.close();

        
        for (std::pair<double, double> opt: otm_option_prices)
        {
            otm_fft_file << opt.first << "," << opt.second << std::endl;
        }
        otm_fft_file.close();
        ///// End serialization
        
        for(double k: K)
        {   
            double price;
            if (k/St <= 1.5 && k/St >= 0.5)
            {
                price = fft_option_pricer->LinearInterpolateStrike(k, atm_option_prices);
            }
            else
            {
                price = fft_option_pricer->LinearInterpolateStrike(k, otm_option_prices);
            }
            
            double bs_price = BlackScholesOptionPricer::BSPrice(St, k, T, sigma, rf, q, call);
            std::cout << "Variance Gamma Option price for Strike: " << k << " is: " << price << std::endl;
            std::cout << "Black Scholes Option price for Strike: " << k << " is: " << bs_price << std::endl;
        }
    }
    
    QuantPricer::OptionPriceBounds::ComputeCallSpreadBounds();
    QuantPricer::OptionPriceBounds::ComputeCalendarSpreadBounds();
    QuantPricer::OptionPriceBounds::ComputeCallPricesAsFunctionOfK();
    QuantPricer::OptionPriceBounds::ComputeButterflyBounds();
    QuantPricer::OptionPriceBounds::ComputeStraddleBounds();
    QuantPricer::OptionPriceBounds::ComputeCallBounds();
    
    ImpliedVolatility* imp_vol = new ImpliedVolatility();
    std::ifstream file("/Users/swatimital/GitHub/QuantPricer/Inputs/spx_vols.csv");
    imp_vol->PopulateData(boost::gregorian::from_string("2016/06/28"), file);
}
