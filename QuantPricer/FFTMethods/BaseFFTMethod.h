//
//  BaseFFTMethod.h
//  QuantPricer
//
//  Created by Swati Mital on 16/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef QuantPricer_BaseFFTMethod_h
#define QuantPricer_BaseFFTMethod_h

#include <complex>
#include "boost/shared_ptr.hpp"
#include "boost/function.hpp"
#include "FastFourierTransform.h"

namespace QuantPricer
{
    namespace FFTMethods
    {
        class BaseFFTMethod
        {
        public:
            BaseFFTMethod(double rf_rate, double dividend, double time_to_maturity);
            virtual ~BaseFFTMethod() {};
            virtual std::complex<double> ZhiFunction(double u,
                                                     boost::function<std::complex<double>(std::complex<double>)> characteristic_fn) const;
            virtual std::vector<std::complex<double>> FFTOutputVector(std::vector<std::complex<double>> in) const;
            
        protected:
            double m_fft_N;
            double m_fft_eta;
            double m_time_to_maturity;
            double m_rf_rate;
            double m_dividend;
            double m_alpha;
            boost::shared_ptr<FFT::FastFourierTransform> m_fft_transform;
        };
    }
}

#endif
