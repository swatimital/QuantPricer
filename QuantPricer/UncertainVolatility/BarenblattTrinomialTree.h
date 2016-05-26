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
#include "Derivative.h"

namespace QuantPricer
{
    namespace UncertainVolatility
    {
        class BarenblattTrinomialTree : public FiniteDiffMethods::TrinomialTree<double, Equities::BarenblattDerivative>
        {
        public:
            BarenblattTrinomialTree(double S0, double sigma_max, double sigma_min, double rf, double dividend, double T, double steps);
            virtual void InitializeTree();
            double GetSigmaMax() const;
            double GetSigmaMin() const;
            double GetNodeProbLB() const;
            double GetNodeProbUB() const;
            virtual int GetNodeStartFromTimeStep(double time_step);
            
        protected:
            void ComputeAssetPriceFactors();
            virtual boost::shared_ptr<FiniteDiffMethods::Node<double, Equities::BarenblattDerivative>> BuildUnderlyingTree(double val, NodeDir ndir, int tree_level);
            
        private:
            double m_sigma_max;
            double m_sigma_min;
            double m_p_lb;
            double m_p_ub;
        };

    }
}

#endif /* defined(__QuantPricer__BarenblattTrinomialTree__) */
