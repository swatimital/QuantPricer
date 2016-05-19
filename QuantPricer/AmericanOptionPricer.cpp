//
//  AmericanOptionPricer.cpp
//  QuantPricer
//
//  Created by Swati Mital on 16/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include "AmericanOptionPricer.h"

AmericanOptionPricer::AmericanOptionPricer(double sigma, double rf, double div, double T) :
VanillaOptionPricer(sigma, rf, div, T)
{
}

AmericanOptionPricer::AmericanOptionPricer(TreePtr ptr) : VanillaOptionPricer(ptr)
{
}

AmericanOptionPricer::~AmericanOptionPricer()
{}

double AmericanOptionPricer::GetPrice(double S0, double K, OptionType opt)
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
        if(opt == OptionType::Call)
        {
            std::get<1>(nodes[i]->values) = (std::get<0>(nodes[i]->values)*S0 - K) >= 0 ? std::get<0>(nodes[i]->values)*S0 - K : 0;
        }else{
            
            std::get<1>(nodes[i]->values) = (K - std::get<0>(nodes[i]->values)*S0) >= 0 ? K - std::get<0>(nodes[i]->values)*S0: 0;
        }
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
