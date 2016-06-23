//
//  FFTOptionPricer.cpp
//  QuantPricer
//
//  Created by Swati Mital on 21/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include "FFTOptionPricer.h"
#include "boost/bind.hpp"

namespace QuantPricer
{
    namespace PricingEngine
    {
        double FFTOptionPricer::LinearInterpolateStrike(
            double K, std::vector<std::pair<double, double>> strike_call_pairs)
        {
            size_t n = strike_call_pairs.size();
            
            for (int i = 0; i < n-1; i++)
            {
                if(K > strike_call_pairs[i].first && K <= strike_call_pairs[i+1].first)
                {
                    double K1 = strike_call_pairs[i].first;
                    double K2 = strike_call_pairs[i+1].first;
                    double C1 = strike_call_pairs[i].second;
                    double C2 = strike_call_pairs[i+1].second;
                    
                    return((K-K1)*((C2-C1)/(K2-K1)) + C1);
                }
            }
            
            return strike_call_pairs[n-1].second;
            
        }
        
        double FFTOptionPricer::GetCallPrice(
                double K,
                boost::shared_ptr<CharacteristicFunctionMethods::ICharacteristicFunctionMethod> char_fn_method, 
                boost::shared_ptr<FFTMethods::BaseFFTMethod> fft_method)
        {
        
            std::vector<std::pair<double, double>> call_prices = fft_method->FFTCallPrices(boost::bind(boost::mem_fn(&CharacteristicFunctionMethods::ICharacteristicFunctionMethod::CharacteristicFunction), char_fn_method, _1));
            
            return(LinearInterpolateStrike(K, call_prices));
        }
    }
}