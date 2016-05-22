//
//  VanillaOptionPricer.cpp
//  QuantPricer
//
//  Created by Swati Mital on 15/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include "VanillaOptionPricer.h"
#include "RecombiningTrinomialTree.h"

VanillaOptionPricer::VanillaOptionPricer(double sigma, double rf, double div, double T) : OptionPricer()
{
    m_treeptr = boost::make_shared<RecombiningTrinomialTree>(RecombiningTrinomialTree(1.0, sigma, rf, div, T, 100.0));
}

VanillaOptionPricer::VanillaOptionPricer(TreePtr treePtr) : OptionPricer()
{
    m_treeptr = treePtr;
}

VanillaOptionPricer::~VanillaOptionPricer()
{}

double VanillaOptionPricer::GetPrice(boost::function<double(double)> payoff)
{
    m_treeptr->InitializeTree();
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
            
            std::get<1>(nodes[i]->values) = exp(-rf*dt)*(up_prob*std::get<1>(nodes[j]->values) +
                                                             mid_prob*std::get<1>(nodes[j+1]->values) +
                                                             down_prob*std::get<1>(nodes[j+2]->values));
        }
        
    }
    
    return std::get<1>(nodes[0]->values);
}


