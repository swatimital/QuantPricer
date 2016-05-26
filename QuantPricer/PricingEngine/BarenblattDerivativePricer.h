//
//  BarenblattDerivativePricer.h
//  QuantPricer
//
//  Created by Swati Mital on 17/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef __QuantPricer__BarenblattDerivativePricer__
#define __QuantPricer__BarenblattDerivativePricer__

#include <stdio.h>

#include "BarenblattTrinomialTree.h"
#include "OptionPricer.h"

namespace QuantPricer
{
    namespace PricingEngine
    {
        class BarenblattDerivativePricer : public OptionPricer<double,Equities::BarenblattDerivative>
        {
        public:
            BarenblattDerivativePricer(double sigma_max, double sigma_min, double rf, double div, double T);
            BarenblattDerivativePricer(TreePtr ptr);
            
            virtual Equities::BarenblattDerivative GetPrice(boost::function<double(double)> payoff);
            virtual Equities::BarenblattDerivative GetPrice(std::vector<std::pair<double, boost::function<double(double)>>> payoffs);
        };

    }
}

#endif /* defined(__QuantPricer__BarenblattDerivativePricer__) */
