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
#include <boost/scoped_ptr.hpp>
#include "RecombiningTrinomialTree.h"
#include <boost/make_shared.hpp>

typedef boost::scoped_ptr<TrinomialTree> TreePtr;
enum OptionType {Call, Put};

class VanillaOptionPricer
{
public:
    VanillaOptionPricer(double sigma, double rf, double div, double T);
    ~VanillaOptionPricer();
    virtual double GetOptionPrice(double S0, double K, OptionType opt);
    
protected:
    TreePtr m_treeptr;
    double m_S0, m_K, m_sigma, m_rf, m_div, m_T;
    double m_N;
    double m_dt;
    
};

#endif /* defined(__QuantPricer__VanillaOptionPricer__) */
