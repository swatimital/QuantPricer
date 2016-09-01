//
//  BrownianMotionMethod.cpp
//  QuantPricer
//
//  Created by Swati Mital on 29/08/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include <stdio.h>

#include "BrownianMotionMethod.h"
#include <math.h>
#include "boost/bind.hpp"

namespace QuantPricer
{
    namespace CharacteristicFunctionMethods
    {
        BrownianMotionMethod::BrownianMotionMethod(double sigma,
                                                   double St,
                                                   double rf_rate, 
                                                   double dividend,
                                                   double time_to_maturity):
        m_rf_rate(rf_rate),
        m_dividend(dividend), 
        m_time_to_maturity(time_to_maturity),
        m_sigma(sigma), 
        m_St(St)
        {
            
        }
        
        std::complex<double> BrownianMotionMethod::CharacteristicFunction(std::complex<double> u)
        {
            auto img = std::complex<double>(0,1);
            double mu = log(m_St) + (m_rf_rate-m_dividend-m_sigma*m_sigma*0.5)*m_time_to_maturity;
            double sigma_sqr = m_sigma*m_sigma*m_time_to_maturity;
            return std::exp(img*mu*u - sigma_sqr*u*u*0.5);
        }
    }
}