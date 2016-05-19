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

#include "OptionPricer.h"

class BarenblattDerivativePricer : public OptionPricer<double, std::tuple<double, double>>
{
public:
    BarenblattDerivativePricer(double sigma_max, double sigma_min, double rf, double div, double T);
    BarenblattDerivativePricer(TreePtr ptr);
    
    virtual std::tuple<double, double> GetPrice(double S0, double K, OptionType opt);
};

#endif /* defined(__QuantPricer__BarenblattDerivativePricer__) */
