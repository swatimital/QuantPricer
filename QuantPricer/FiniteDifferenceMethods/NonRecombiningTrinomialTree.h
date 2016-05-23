//
//  NonRecombiningTrinomialTree.h
//  QuantPricer
//
//  Created by Swati Mital on 15/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef __QuantPricer__NonRecombiningTrinomialTree__
#define __QuantPricer__NonRecombiningTrinomialTree__

#include <stdio.h>
#include "TrinomialTree.h"

namespace QuantPricer
{
    namespace FiniteDiffMethods
    {
        class NonRecombiningTrinomialTree : public TrinomialTree<double, double>
        {
        public:
            NonRecombiningTrinomialTree(double S0, double sigma, double rf, double dividend, double T, double steps=100.0);
            
        protected:
            virtual boost::shared_ptr<Node<double, double>> BuildUnderlyingTree(double val, NodeDir ndir, int tree_level);
            
            
        };
    }
}


#endif /* defined(__QuantPricer__NonRecombiningTrinomialTree__) */
