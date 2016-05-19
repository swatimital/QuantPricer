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
#include "TrinomialTree.h"

class BarenblattTrinomialTree : public TrinomialTree<double, std::tuple<double, double>>
{
public:
    BarenblattTrinomialTree(double S0, double sigma_max, double sigma_min, double rf, double dividend, double T, double steps);
    virtual void InitializeTree();
    double GetSigmaMax() const;
    double GetSigmaMin() const;
    
protected:
    void ComputeAssetPriceFactors();
    virtual boost::shared_ptr<Node<double, std::tuple<double, double>>> BuildUnderlyingTree(double val, NodeDir ndir, int tree_level);
    
private:
    double m_sigma_max;
    double m_sigma_min;
};

#endif /* defined(__QuantPricer__BarenblattTrinomialTree__) */
