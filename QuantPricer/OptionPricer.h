//
//  OptionPricer.h
//  QuantPricer
//
//  Created by Swati Mital on 18/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef __QuantPricer__OptionPricer__
#define __QuantPricer__OptionPricer__

#include <stdio.h>
#include <boost/shared_ptr.hpp>
#include "TrinomialTree.h"

enum OptionType {Call, Put};

template<class Underlying, class Derivative>
class OptionPricer
{
public:
    typedef boost::shared_ptr<TrinomialTree<Underlying, Derivative>> TreePtr;
    OptionPricer() {};
    virtual ~OptionPricer() {};
    virtual Derivative GetPrice(double S0, double K, OptionType opt) = 0;
    
    
protected:
    TreePtr m_treeptr;
};

#endif /* defined(__QuantPricer__OptionPricer__) */
