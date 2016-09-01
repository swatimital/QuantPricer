//
//  HestonStochVolMethod.h
//  QuantPricer
//
//  Created by Swati Mital on 31/08/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef QuantPricer_HestonStochVolMethod_h
#define QuantPricer_HestonStochVolMethod_h


#include "ICharacteristicFunctionMethod.h"

#include <vector>
#include <complex>


namespace QuantPricer
{
    namespace CharacteristicFunctionMethods
    {
        class HestonStochVolMethod : public ICharacteristicFunctionMethod
        {
        public:
            HestonStochVolMethod(double V0,
                                 double mean_rev_speed,
                                 double long_term_var,
                                 double vol_of_vol,
                                 double price_vol_corr,
                                 double St,
                                 double rf_rate, 
                                 double dividend,
                                 double time_to_maturity);
            virtual ~HestonStochVolMethod() {}
            std::complex<double> CharacteristicFunction(std::complex<double> u);
            
        private:
            double m_V0;
            double m_mean_rev_speed;
            double m_long_term_var;
            double m_vol_of_vol;
            double m_corr;
            double m_St;
            double m_rf_rate, m_time_to_maturity, m_dividend;
        };
    }
}



#endif
