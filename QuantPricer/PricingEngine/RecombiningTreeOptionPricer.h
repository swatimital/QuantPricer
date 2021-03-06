//
//  VanillaOptionPricer.h
//  QuantPricer
//
//  Created by Swati Mital on 15/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef __QuantPricer__RecombiningTreeOptionPricer__
#define __QuantPricer__RecombiningTreeOptionPricer__

#include <stdio.h>
#include <boost/make_shared.hpp>
#include "OptionPricer.h"

namespace QuantPricer
{
    namespace PricingEngine
    {
        class RecombiningTreeOptionPricer : public OptionPricer<double, double>
        {
        public:
            RecombiningTreeOptionPricer(double sigma, double rf, double div, double T);
            RecombiningTreeOptionPricer(TreePtr treeptr);
            virtual ~RecombiningTreeOptionPricer();
            virtual double GetPrice(boost::function<double(double)> payoff);
            virtual double GetPrice(std::vector<std::pair<double, boost::function<double(double)>>> payoffs);
        };
    }
}



#endif /* defined(__QuantPricer__VanillaOptionPricer__) */
