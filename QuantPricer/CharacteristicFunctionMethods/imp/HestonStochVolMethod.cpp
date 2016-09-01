//
//  HestonStochVolMethod.cpp
//  QuantPricer
//
//  Created by Swati Mital on 31/08/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include "HestonStochVolMethod.h"
#include <math.h>
#include "boost/bind.hpp"

namespace QuantPricer
{
    namespace CharacteristicFunctionMethods
    {
        HestonStochVolMethod::HestonStochVolMethod(double V0,
                                                   double mean_rev_speed,
                                                   double long_term_var,
                                                   double vol_of_vol,
                                                   double price_vol_corr,
                                                   double St,
                                                   double rf_rate, 
                                                   double dividend,
                                                   double time_to_maturity):
        m_rf_rate(rf_rate), m_dividend(dividend), m_time_to_maturity(time_to_maturity),
        m_V0(V0), m_mean_rev_speed(mean_rev_speed), m_long_term_var(long_term_var), m_vol_of_vol(vol_of_vol), m_corr(price_vol_corr),
        m_St(St)
        {
            
        }
        
        std::complex<double> HestonStochVolMethod::CharacteristicFunction(
                                        std::complex<double> u)
        {
            auto real = std::complex<double>(1,0);
            auto img = std::complex<double>(0,1);
            std::complex<double> beta = m_mean_rev_speed - img*m_corr*m_vol_of_vol*u;
            std::complex<double> du = std::sqrt(beta*beta + m_vol_of_vol*m_vol_of_vol*u*(u+img));
            std::complex<double> rplus = (beta + du)/(m_vol_of_vol*m_vol_of_vol);
            std::complex<double> rminus = (beta - du)/(m_vol_of_vol*m_vol_of_vol);
            std::complex<double> cu = rminus/rplus;
            
            std::complex<double> aux1 = exp(-du*m_time_to_maturity);
            std::complex<double> Gu = (real - cu*aux1)/(real-cu);
            std::complex<double> Du = rminus*((real - aux1)/(real - cu*aux1));
            
            std::complex<double> aux2 = exp((m_mean_rev_speed*m_long_term_var*rminus*m_time_to_maturity) + (m_V0*Du));
            std::complex<double> aux3 = pow(Gu, 2.0*m_mean_rev_speed*m_long_term_var/(m_vol_of_vol*m_vol_of_vol));
           
            return (exp(img*u*log(m_St))*(aux2/aux3));
        }
        
    }
}