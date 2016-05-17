//
//  BarenblattTrinomialTree.h
//  QuantPricer
//
//  Created by Swati Mital on 16/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef __QuantPricer__BarenblattTrinomialTree__
#define __QuantPricer__BarenblattTrinomialTree__

#include <stdio.h>
#include "RecombiningTrinomialTree.h"

class BarenblattTrinomialTree : public RecombiningTrinomialTree
{
public:
    BarenblattTrinomialTree(double S0, double sigma, double rf, double dividend, double T, double steps);
    virtual void InitializeTree();
    
protected:
    void ComputeAssetPriceFactors();
    
private:
    double m_sigma_max;
    double m_sigma_min;
};

#endif /* defined(__QuantPricer__BarenblattTrinomialTree__) */
