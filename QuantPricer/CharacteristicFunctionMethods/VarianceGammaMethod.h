//
//  VarianceGammaMethod.h
//  QuantPricer
//
//  Created by Swati Mital on 10/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef __QuantPricer__VarianceGammaMethod__
#define __QuantPricer__VarianceGammaMethod__

#include "ICharacteristicFunctionMethod.h"
#include <vector>
#include <complex>


namespace QuantPricer
{
    namespace CharacteristicFunctionMethods
    {
        class VarianceGammaMethod : public ICharacteristicFunctionMethod
        {
        public:
            VarianceGammaMethod(double theta, double sigma,
                                double var_rate, double St,
                                double rf_rate, double dividend,
                                double time_to_maturity);
            virtual ~VarianceGammaMethod() {}
            std::complex<double> CharacteristicFunction(std::complex<double> u);
        
        private:
            double m_theta;
            double m_sigma;
            double m_var_rate;
            double m_St;
            double m_rf_rate, m_time_to_maturity, m_dividend;
        };
    }
}

#endif /* defined(__QuantPricer__VarianceGammaMethod__) */
