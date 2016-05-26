//
//  RecombiningTrinomialTree.h
//  QuantPricer
//
//  Created by Swati Mital on 15/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef __QuantPricer__RecombiningTrinomialTree__
#define __QuantPricer__RecombiningTrinomialTree__

#include <stdio.h>
#include "TrinomialTree.h"

namespace QuantPricer
{
    namespace FiniteDiffMethods
    {
        class RecombiningTrinomialTree : public TrinomialTree<double, double>
        {
        public:
            RecombiningTrinomialTree(double S0, double sigma, double rf, double dividend, double T, double steps=100.0);
            virtual void InitializeTree();
            virtual int GetNodeStartFromTimeStep(double time_step);
            
        protected:
            virtual boost::shared_ptr<Node<double, double>> BuildUnderlyingTree(double val, NodeDir ndir, int tree_level);
        };
    }
}



#endif /* defined(__QuantPricer__RecombiningTrinomialTree__) */
