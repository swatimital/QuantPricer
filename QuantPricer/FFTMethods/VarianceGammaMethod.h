//
//  VarianceGammaMethod.h
//  QuantPricer
//
//  Created by Swati Mital on 10/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef __QuantPricer__VarianceGammaMethod__
#define __QuantPricer__VarianceGammaMethod__

#include "BaseFFTMethod.h"
#include <vector>
#include <complex>


namespace QuantPricer
{
    namespace FFTMethods
    {
        class VarianceGammaMethod : public BaseFFTMethod
        {
        public:
            VarianceGammaMethod(double theta, double sigma,
                                double var_rate, double St,
                                double rf_rate, double dividend,
                                double time_to_maturity);
            virtual ~VarianceGammaMethod() {}
            std::complex<double> CharacteristicFunction(std::complex<double> u);
        
        protected:
            std::vector<std::complex<double>> PopulateInputVector() const;
            
        private:
            double m_theta;
            double m_sigma;
            double m_var_rate;
            double m_St;
        };
    }
}

#endif /* defined(__QuantPricer__VarianceGammaMethod__) */
