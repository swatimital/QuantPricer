//
//  BaseFFTMethod.cpp
//  QuantPricer
//
//  Created by Swati Mital on 20/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include <stdio.h>
#include "BaseFFTMethod.h"

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
        
        
        std::vector<std::complex<double>> BaseFFTMethod::FFTOutputVector(std::vector<std::complex<double>> in) const
        {
            return (m_fft_transform->Execute(in));
        }
        
    }
}