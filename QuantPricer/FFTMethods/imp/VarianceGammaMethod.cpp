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
    namespace FFTMethods
    {
        VarianceGammaMethod::VarianceGammaMethod(double theta, double sigma,
                                                 double var_rate, double St,
                                                 double rf_rate, double dividend,
                                                 double time_to_maturity):
        BaseFFTMethod(rf_rate, dividend, time_to_maturity),
        m_theta(theta), m_sigma(sigma), m_var_rate(var_rate),
        m_St(St)
        {
            
        }
        
        std::complex<double> VarianceGammaMethod::CharacteristicFunction(std::complex<double> u)
        {
            auto real = std::complex<double>(1,0);
            auto img = std::complex<double>(0,1);
            
            double omega = (1/m_var_rate) * log(1-m_theta*m_var_rate - 0.5*m_sigma*m_sigma*m_var_rate);
            std::complex<double> power_term = pow(real - img*m_theta*m_var_rate*u + real*0.5*m_sigma*m_sigma*u*u*m_var_rate, -m_time_to_maturity/m_var_rate);
            
            return( exp(log(m_St) + (m_rf_rate + omega)*m_time_to_maturity)*power_term );
        }
        
        
        std::vector<std::complex<double>> VarianceGammaMethod::PopulateInputVector() const
        {
            int N = m_fft_transform->GetFFTArrayLength();
            std::vector<std::complex<double>> in_vector(N);
            
            double lambda = (2*M_PI)/(N*m_fft_eta);
            double b = 0.5*N*lambda;
            double nu;
            for (int i = 0; i < N; i++)
            {
                nu = i*m_fft_eta;
                std::complex<double> zhi = ZhiFunction(nu, boost::bind(boost::mem_fn(&VarianceGammaMethod::CharacteristicFunction), *this, _1));
                in_vector[i] = exp(std::complex<double>(0,1)*nu*b)*m_fft_eta*zhi;
            }
            
            return in_vector;
        }
        
    }
}