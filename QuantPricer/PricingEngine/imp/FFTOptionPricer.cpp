//
//  FFTOptionPricer.cpp
//  QuantPricer
//
//  Created by Swati Mital on 21/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include "FFTOptionPricer.h"
#include "boost/bind.hpp"
#include <fstream>

namespace QuantPricer
{
    namespace PricingEngine
    {
        
        std::vector<std::pair<double, double>> FFTOptionPricer::PrecomputeFFTOptionPrices  
                                                        ( double St,
                                                          double rf_rate,
                                                          double dividend,
                                                          double time_to_maturity,
                                                          bool atm,
                                                          bool call,
                                                          boost::shared_ptr<CharacteristicFunctionMethods::ICharacteristicFunctionMethod> char_fn_method, 
                                                          boost::shared_ptr<FFTMethods::BaseFFTMethod> fft_method) const
        {
            return (fft_method->FFTEuropeanOptionPrices(boost::bind(boost::mem_fn(&CharacteristicFunctionMethods::ICharacteristicFunctionMethod::CharacteristicFunction), char_fn_method, _1),
               St,
               rf_rate,
               dividend,
               time_to_maturity, 
               atm, 
               call));
        }
        
        double FFTOptionPricer::GetPutCallPrice(double St, 
                                             double K,
                                             double rf_rate,
                                             double dividend,
                                             double time_to_maturity,
                                             bool call,
                                             boost::shared_ptr<CharacteristicFunctionMethods::ICharacteristicFunctionMethod> char_fn_method, 
                                             boost::shared_ptr<FFTMethods::BaseFFTMethod> fft_method) const
        {
            bool atm = (K/St <= 1.5 && K/St >= 0.5) ? true : false;
            
            std::vector<std::pair<double, double>> option_prices = PrecomputeFFTOptionPrices(St, rf_rate, dividend, time_to_maturity, call, atm, char_fn_method, fft_method);
            
            return(LinearInterpolateStrike(K, option_prices));
        }
        
        double FFTOptionPricer::LinearInterpolateStrike(double K, 
                                                        std::vector<std::pair<double, double>> strike_option_price_pair) const
        {
            size_t n = strike_option_price_pair.size();
            
            for (int i = 0; i < n-1; i++)
            {
                if(K > strike_option_price_pair[i].first && K <= strike_option_price_pair[i+1].first)
                {
                    double K1 = strike_option_price_pair[i].first;
                    double K2 = strike_option_price_pair[i+1].first;
                    double C1 = strike_option_price_pair[i].second;
                    double C2 = strike_option_price_pair[i+1].second;
                    
                    return((K-K1)*((C2-C1)/(K2-K1)) + C1);
                }
            }
            
            return strike_option_price_pair[n-1].second;
            
        }

    }
}