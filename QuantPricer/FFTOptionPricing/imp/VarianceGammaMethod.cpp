//
//  VarianceGammaMethod.cpp
//  QuantPricer
//
//  Created by Swati Mital on 10/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include "VarianceGammaMethod.h"
#include <math.h>

namespace QuantPricer
{
    namespace FFTOptionPricing
    {
        VarianceGammaMethod::VarianceGammaMethod(double drift, double sigma,
                                                 double var_rate, double rf_rate,
                                                 double time_to_maturity, double St):
        m_drift(drift), m_sigma(sigma), m_var_rate(var_rate),
        m_rf_rate(rf_rate), m_time_to_maturity(time_to_maturity), m_S_t(St)
        {
            
        }
        
        double VarianceGammaMethod::CharacteristicFunction(double u)
        {
            double i = sqrt(-1);
            double omega = (1/m_var_rate) * log(1-m_drift*m_var_rate -
                                                0.5*m_sigma*m_sigma*m_var_rate);
            double char_function = exp(log(m_S_t) + (m_rf_rate + omega)*m_time_to_maturity)*
            pow(1-i*m_drift*m_var_rate*u+0.5*m_sigma*m_sigma*u*u*m_var_rate, 
                -m_time_to_maturity/m_var_rate);
            
            return char_function;
        }
        
    }
}