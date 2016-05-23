//
//  AmericanOptionPricer.h
//  QuantPricer
//
//  Created by Swati Mital on 16/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef __QuantPricer__AmericanOptionPricer__
#define __QuantPricer__AmericanOptionPricer__

#include <stdio.h>
#include "RecombiningTreeOptionPricer.h"

namespace QuantPricer
{
    namespace PricingEngine
    {
        class AmericanOptionPricer : public RecombiningTreeOptionPricer
        {
        public:
            AmericanOptionPricer(double sigma, double rf, double div, double T);
            AmericanOptionPricer(TreePtr);
            ~AmericanOptionPricer();
            virtual double GetPrice(boost::function<double(double)> payoff);
            
        };
    }
}


#endif /* defined(__QuantPricer__AmericanOptionPricer__) */
