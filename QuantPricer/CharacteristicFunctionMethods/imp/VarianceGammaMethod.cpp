//
//  VarianceGammaMethod.cpp
//  QuantPricer
//
//  Created by Swati Mital on 10/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include "VarianceGammaMethod.h"
#include <math.h>
#include "boost/bind.hpp"

namespace QuantPricer
{
    namespace CharacteristicFunctionMethods
    {
        VarianceGammaMethod::VarianceGammaMethod(double theta, double sigma,
                                                 double var_rate, double St,
                                                 double rf_rate, double dividend,
                                                 double time_to_maturity):
        m_rf_rate(rf_rate), m_dividend(dividend), m_time_to_maturity(time_to_maturity),
        m_theta(theta), m_sigma(sigma), m_var_rate(var_rate),
        m_St(St)
        {
            
        }
        
        std::complex<double> VarianceGammaMethod::CharacteristicFunction(std::complex<double> u)
        {
            auto real = std::complex<double>(1,0);
            auto img = std::complex<double>(0,1);
            
            double omega = (1/m_var_rate) * log(1-m_theta*m_var_rate - 0.5*m_sigma*m_sigma*m_var_rate);
            std::complex<double> power_term = pow(real - img*m_theta*m_var_rate*u + 0.5*m_sigma*m_sigma*u*u*m_var_rate, -m_time_to_maturity/m_var_rate);
            std::complex<double> num = exp(img*u*log(m_St)+img*u*(m_rf_rate - m_dividend + omega)*m_time_to_maturity);
            return(num*power_term);
        }
        
    }
}