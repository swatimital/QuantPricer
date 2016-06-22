//
//  FFTOptionPricer.h
//  QuantPricer
//
//  Created by Swati Mital on 21/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef __QuantPricer__FFTOptionPricer__
#define __QuantPricer__FFTOptionPricer__

#include <stdio.h>
#include "boost/shared_ptr.hpp"
#include "ICharacteristicFunctionMethod.h"
#include "BaseFFTMethod.h"

namespace QuantPricer
{
    namespace PricingEngine
    {
        class FFTOptionPricer
        {
        public:
            FFTOptionPricer();
            virtual ~FFTOptionPricer();
            double LinearInterpolateStrike(
                    double K, std::vector<std::pair<double, double>> strike_call_pairs);
            double GetCallPrice(double K,
                            boost::shared_ptr<CharacteristicFunctionMethods::ICharacteristicFunctionMethod> char_fn_method, 
                            boost::shared_ptr<FFTMethods::BaseFFTMethod> fft_method);
        private:
            
        };
    }
}


#endif /* defined(__QuantPricer__FFTOptionPricer__) */
