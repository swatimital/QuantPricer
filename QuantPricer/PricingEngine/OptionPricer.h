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
#include <boost/function.hpp>
#include "TrinomialTree.h"

namespace QuantPricer
{
    namespace PricingEngine
    {
        template<class UnderlyingT, class DerivativeT>
        class OptionPricer
        {
        public:
            typedef boost::shared_ptr<FiniteDiffMethods::TrinomialTree<UnderlyingT, DerivativeT>> TreePtr;
            OptionPricer() {};
            virtual ~OptionPricer() {};
            virtual DerivativeT GetPrice(boost::function<double(double)> payoff) = 0;
            virtual DerivativeT GetPrice(std::vector<std::pair<double, boost::function<double(double)>>> payoffs) = 0;
            
        protected:
            TreePtr m_treeptr;
        };
    }
}


#endif /* defined(__QuantPricer__OptionPricer__) */
