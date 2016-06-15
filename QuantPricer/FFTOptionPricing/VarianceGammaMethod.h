//
//  VarianceGammaMethod.h
//  QuantPricer
//
//  Created by Swati Mital on 10/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef __QuantPricer__VarianceGammaMethod__
#define __QuantPricer__VarianceGammaMethod__

#include <stdio.h>

namespace QuantPricer
{
    namespace FFTOptionPricing
    {
        class VarianceGammaMethod
        {
        public:
            VarianceGammaMethod(double drift, double sigma,
                                double var_rate, double rf_rate,
                                double time_to_maturity, double St);
            double CharacteristicFunction(double u);
            
        private:
            double m_drift;
            double m_sigma;
            double m_var_rate;
            double m_S_t;
            double m_time_to_maturity;
            double m_rf_rate;
        };
    }
}

#endif /* defined(__QuantPricer__VarianceGammaMethod__) */
