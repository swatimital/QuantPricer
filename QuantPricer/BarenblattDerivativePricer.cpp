//
//  BarenblattDerivativePricer.cpp
//  QuantPricer
//
//  Created by Swati Mital on 17/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include "BarenblattDerivativePricer.h"
#include "BarenblattTrinomialTree.h"

BarenblattDerivativePricer::BarenblattDerivativePricer(double sigma_max, double sigma_min, double rf, double div, double T) :
OptionPricer()
{
    m_treeptr = boost::make_shared<BarenblattTrinomialTree>(BarenblattTrinomialTree(1.0, sigma_max, sigma_min, rf, div, T, 100.0));
}

BarenblattDerivativePricer::BarenblattDerivativePricer(TreePtr ptr) :
OptionPricer()
{
    m_treeptr = ptr;
}

std::tuple<double, double> BarenblattDerivativePricer::GetPrice(double S0, double K, OptionType opt)
{
    auto m_bb_treeptr = boost::dynamic_pointer_cast<BarenblattTrinomialTree>(m_treeptr);
    m_bb_treeptr->InitializeTree();
    
    auto nodes = m_treeptr->GetBreadthFirstNodeValues();
    auto n = m_bb_treeptr->GetLevel();
    auto dt = m_bb_treeptr->GetMaturity()/n;
    auto rf = m_bb_treeptr->GetRiskFreeRate();
    auto sigma_max = m_bb_treeptr->GetSigmaMax();
    auto sigma_min = m_bb_treeptr->GetSigmaMin();
    auto end = nodes.size();
    auto start = nodes.size() - (2*n+1);
    
    // Set the option values on the leaf nodes
    for (auto i = start; i < end; i++)
    {
        std::tuple<double, double> option_bounds = std::get<1>(nodes[i]->values);
        if(opt == OptionType::Call)
        {
            std::get<0>(option_bounds) = (std::get<0>(nodes[i]->values)*S0 - K) >= 0 ? std::get<0>(nodes[i]->values)*S0 - K : 0;
            std::get<1>(option_bounds) = std::get<0>(option_bounds);
        }else{
            
            std::get<1>(option_bounds) = (K - std::get<0>(nodes[i]->values)*S0) >= 0 ? K - std::get<0>(nodes[i]->values)*S0: 0;
            std::get<1>(option_bounds) = std::get<0>(option_bounds);
        }
    }
    
    while (start >= 1)
    {
        end = start;
        start = start - (2*(--n)+1);
        auto j = end;
        for (auto i = start; i < end; i++,j++)
        {
            std::tuple<double, double> option_bounds = std::get<1>(nodes[i]->values);
            std::tuple<double, double> next_up = std::get<1>(nodes[j]->values);
            std::tuple<double, double> next_mid = std::get<1>(nodes[j+1]->values);
            std::tuple<double, double> next_down = std::get<1>(nodes[j+2]->values);
            
            auto gamma0 = (1 - (sigma_max*sqrt(dt)*0.5)) * std::get<0>(next_up) + (1 + (sigma_max*sqrt(dt)*0.5)) * std::get<0>(next_down) - std::get<0>(next_mid);
            
            auto gamma1 = (1 - (sigma_max*sqrt(dt)*0.5)) * std::get<1>(next_up) + (1 + (sigma_max*sqrt(dt)*0.5)) * std::get<1>(next_down) - std::get<1>(next_mid);
            
            if (gamma0 >= 0)
            {
                std::get<0>(option_bounds) = exp(-rf*dt)*(std::get<0>(next_mid) + 0.5*gamma0);
            }
            else
            {
                std::get<0>(option_bounds) = exp(-rf*dt)*(std::get<0>(next_mid) + (sigma_min*sigma_min/(2*sigma_max*sigma_max))*gamma0);
            }
            
            if (gamma1 < 0)
            {
                std::get<1>(option_bounds) = exp(-rf*dt)*(std::get<1>(next_mid) + 0.5*gamma1);
            }
            else
            {
                std::get<1>(option_bounds) = exp(-rf*dt)*(std::get<1>(next_mid) + (sigma_min*sigma_min/(2*sigma_max*sigma_max))*gamma1);
            }
        }
    }
    
    return std::get<1>(nodes[0]->values);

}