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

class BarenblattDerivativePricer : public OptionPricer<double, BSBDerivative>
{
public:
    BarenblattDerivativePricer(double sigma_max, double sigma_min, double rf, double div, double T);
    BarenblattDerivativePricer(TreePtr ptr);
    
    virtual BSBDerivative GetPrice(boost::function<double(double)> payoff);
 
};

#endif /* defined(__QuantPricer__BarenblattDerivativePricer__) */
