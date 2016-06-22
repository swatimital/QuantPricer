//
//  BaseFFTMethod.cpp
//  QuantPricer
//
//  Created by Swati Mital on 20/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include <stdio.h>
#include "BaseFFTMethod.h"
#include <utility>

namespace QuantPricer
{
    namespace FFTMethods
    {
        BaseFFTMethod::BaseFFTMethod(double rf_rate, double dividend, double time_to_maturity) :
            m_time_to_maturity(time_to_maturity),
            m_dividend(dividend),
            m_rf_rate(rf_rate),
            m_fft_eta(0.01),
            m_fft_N(20),
            m_alpha(1.1),
            m_fft_transform(new FFT::FastFourierTransform(m_fft_N))
        {
            m_fft_transform->Initialize();
        }
        
        std::complex<double> BaseFFTMethod::ZhiFunction(double u,
            boost::function<std::complex<double>(std::complex<double>)> characteristic_fn) const
        {
            return (
                exp(-m_rf_rate*m_time_to_maturity)*
                    characteristic_fn(std::complex<double>(u, -(m_alpha+1))) /
                    (m_alpha*m_alpha + m_alpha - u*u + (2*m_alpha+1)*u*
                     std::complex<double>(0,1)));

        }
        
        std::vector<std::pair<double, double>> BaseFFTMethod::FFTCallPrices(boost::function<std::complex<double>(std::complex<double>)> characteristic_fn) const
        {
            std::vector<std::complex<double>> fft_out = FFTOutputVector(characteristic_fn);
            double N = m_fft_transform->GetFFTArrayLength();
            double lambda = (2*M_PI)/(N*m_fft_eta);
            double b = 0.5*N*lambda;
            double ku = -b;
            
            std::vector<std::pair<double, double>> call_prices(N);
            
            for (int i = 0; i < N; i++)
            {
                call_prices[i] = std::make_pair(ku, exp(-m_alpha*ku/M_PI)*fft_out[i].real());
                ku += lambda;
            }
            
            return call_prices;
        }
        
        std::vector<std::complex<double>> BaseFFTMethod::FFTOutputVector(boost::function<std::complex<double>(std::complex<double>)> characteristic_fn) const
        {
            return (m_fft_transform->Execute(PopulateInputVector(characteristic_fn)));
        }
        
        std::vector<std::complex<double>> BaseFFTMethod::FFTOutputVector(std::vector<std::complex<double>> in) const
        {
            return (m_fft_transform->Execute(in));
        }
        
        std::vector<std::complex<double>> BaseFFTMethod::PopulateInputVector(boost::function<std::complex<double>(std::complex<double>)> characteristic_fn) const
        {
            int N = m_fft_transform->GetFFTArrayLength();
            std::vector<std::complex<double>> in_vector(N);
            
            double lambda = (2*M_PI)/(N*m_fft_eta);
            double b = 0.5*N*lambda;
            double nu;
            for (int i = 0; i < N; i++)
            {
                nu = i*m_fft_eta;
                std::complex<double> zhi = ZhiFunction(nu, characteristic_fn);
                in_vector[i] = exp(std::complex<double>(0,1)*nu*b)*m_fft_eta*zhi;
            }
            
            return in_vector;
        }

        
    }
}