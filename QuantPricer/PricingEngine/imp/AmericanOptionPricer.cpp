//
//  AmericanOptionPricer.cpp
//  QuantPricer
//
//  Created by Swati Mital on 16/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include "AmericanOptionPricer.h"

namespace QuantPricer
{
    namespace PricingEngine
    {
        AmericanOptionPricer::AmericanOptionPricer(double sigma, double rf, double div, double T) :
        RecombiningTreeOptionPricer(sigma, rf, div, T)
        {
        }
        
        AmericanOptionPricer::AmericanOptionPricer(TreePtr ptr) : RecombiningTreeOptionPricer(ptr)
        {
        }
        
        AmericanOptionPricer::~AmericanOptionPricer()
        {}
        
        double AmericanOptionPricer::GetPrice(boost::function<double(double)> payoff)
        {
            auto nodes = m_treeptr->GetBreadthFirstNodeValues();
            auto n = m_treeptr->GetLevel();
            auto dt = m_treeptr->GetMaturity()/n;
            auto rf = m_treeptr->GetRiskFreeRate();
            auto end = nodes.size();
            auto start = nodes.size() - (2*n+1);
            double up_prob, mid_prob, down_prob;
            m_treeptr->ComputeNodeProbabilities(up_prob, mid_prob, down_prob);
            
            // Set the option values on the leaf nodes
            for (auto i = start; i < end; i++)
            {
                std::get<1>(nodes[i]->values) = payoff(std::get<0>(nodes[i]->values));
            }
            
            while (start >= 1)
            {
                end = start;
                start = start - (2*(--n)+1);
                auto j = end;
                for (auto i = start; i < end; i++,j++)
                {
                    
                    std::get<1>(nodes[i]->values) = std::max(1.0, exp(-rf*dt)*(up_prob*std::get<1>(nodes[j]->values) +
                                                                               mid_prob*std::get<1>(nodes[j+1]->values) +
                                                                               down_prob*std::get<1>(nodes[j+2]->values)));
                }
                
            }
            
            return std::get<1>(nodes[0]->values);
        }
        
    }
}
