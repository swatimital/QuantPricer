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
#include "VanillaOptionPricer.h"

class AmericanOptionPricer : public VanillaOptionPricer
{
public:
    AmericanOptionPricer(double sigma, double rf, double div, double T);
    ~AmericanOptionPricer();
    virtual double GetOptionPrice(double S0, double K, OptionType opt);
    
};

#endif /* defined(__QuantPricer__AmericanOptionPricer__) */
