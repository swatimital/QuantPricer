//
//  VanillaOptionPricer.h
//  QuantPricer
//
//  Created by Swati Mital on 15/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef __QuantPricer__VanillaOptionPricer__
#define __QuantPricer__VanillaOptionPricer__

#include <stdio.h>
#include <boost/make_shared.hpp>
#include "OptionPricer.h"


class VanillaOptionPricer : public OptionPricer<double, double>
{
public:
    VanillaOptionPricer(double sigma, double rf, double div, double T);
    VanillaOptionPricer(TreePtr treeptr);
    virtual ~VanillaOptionPricer();
    virtual double GetPrice(double S0, double K, OptionType opt);
};

#endif /* defined(__QuantPricer__VanillaOptionPricer__) */
