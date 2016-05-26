//
//  BarenblattDerivativePricer.cpp
//  QuantPricer
//
//  Created by Swati Mital on 17/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include "BarenblattDerivativePricer.h"
#include "BarenblattTrinomialTree.h"
#include <map>

namespace QuantPricer
{
    namespace PricingEngine
    {
        BarenblattDerivativePricer::BarenblattDerivativePricer(double sigma_max, double sigma_min, double rf, double div, double T) :
        OptionPricer()
        {
            m_treeptr = boost::make_shared<UncertainVolatility::BarenblattTrinomialTree>(UncertainVolatility::BarenblattTrinomialTree(1.0, sigma_max, sigma_min, rf, div, T, 100.0));
        }
        
        BarenblattDerivativePricer::BarenblattDerivativePricer(TreePtr ptr) :
        OptionPricer()
        {
            m_treeptr = ptr;
        }
        
        Equities::BarenblattDerivative BarenblattDerivativePricer::GetPrice(std::vector<std::pair<double, boost::function<double(double)>>> payoffs)
        {
            auto m_bb_treeptr = boost::dynamic_pointer_cast<UncertainVolatility::BarenblattTrinomialTree>(m_treeptr);
            m_bb_treeptr->InitializeTree();
            
            std::map<int, boost::function<double(double)>> starttime_payoff_map;
            for (int j = 0; j < payoffs.size(); j++)
            {
                starttime_payoff_map.insert(std::make_pair(m_bb_treeptr->GetNodeStartFromTimeStep(payoffs[j].first), payoffs[j].second));
            }
            
            auto nodes = m_treeptr->GetBreadthFirstNodeValues();
            auto n = m_bb_treeptr->GetLevel();
            auto dt = m_bb_treeptr->GetMaturity()/n;
            auto rf = m_bb_treeptr->GetRiskFreeRate();
            auto sigma_max = m_bb_treeptr->GetSigmaMax();
            auto end = nodes.size();
            auto start = nodes.size() - (2*n+1);
            auto p_lb = m_bb_treeptr->GetNodeProbLB();
            auto p_ub = m_bb_treeptr->GetNodeProbUB();
            auto map_it = starttime_payoff_map.find(start);
            
            // Set the option values on the leaf nodes
            for (auto i = start; i < end; i++)
            {
                Equities::BarenblattDerivative& option_bounds = std::get<1>(nodes[i]->values);
                std::get<0>(option_bounds.value) = (map_it != starttime_payoff_map.end()) ? map_it->second(std::get<0>(nodes[i]->values)) : 0.0;
                std::get<1>(option_bounds.value) = std::get<0>(option_bounds.value);
            }
            
            while (start >= 1)
            {
                end = start;
                start = start - (2*(--n)+1);
                auto j = end;
                auto map_it = starttime_payoff_map.find(start);

                for (auto i = start; i < end; i++,j++)
                {
                    Equities::BarenblattDerivative& option_bounds = std::get<1>(nodes[i]->values);
                    Equities::BarenblattDerivative next_up = std::get<1>(nodes[j]->values);
                    Equities::BarenblattDerivative next_mid = std::get<1>(nodes[j+1]->values);
                    Equities::BarenblattDerivative next_down = std::get<1>(nodes[j+2]->values);
                    
                    auto gamma0 = (1 - (sigma_max*sqrt(dt)*0.5)) * std::get<0>(next_up.value) + (1 + (sigma_max*sqrt(dt)*0.5)) * std::get<0>(next_down.value) - 2.0*std::get<0>(next_mid.value);
                    
                    auto gamma1 = (1 - (sigma_max*sqrt(dt)*0.5)) * std::get<1>(next_up.value) + (1 + (sigma_max*sqrt(dt)*0.5)) * std::get<1>(next_down.value) - 2.0*std::get<1>(next_mid.value);
                    
                    auto cashflow = 0.0;
                    if (map_it != starttime_payoff_map.end())
                    {
                        cashflow = map_it->second(std::get<0>(nodes[i]->values));
                    }
                    
                    std::get<0>(option_bounds.value) = cashflow + ((gamma0 >= 0) ? exp(-rf*dt)*(std::get<0>(next_mid.value) + p_ub*gamma0) :
                    exp(-rf*dt)*(std::get<0>(next_mid.value) + p_lb*gamma0));
                    
                    std::get<1>(option_bounds.value) = cashflow + ((gamma1 < 0) ? exp(-rf*dt)*(std::get<1>(next_mid.value) + p_ub*gamma1) :exp(-rf*dt)*(std::get<1>(next_mid.value) + p_lb*gamma1));
                    
                    option_bounds.greeks = Equities::Greeks();
                    option_bounds.greeks->gamma = gamma0;
                }
            }
            
            return std::get<1>(nodes[0]->values);
            
        }
        
        Equities::BarenblattDerivative BarenblattDerivativePricer::GetPrice(boost::function<double(double)> payoff)
        {
            std::vector<std::pair<double, boost::function<double(double)>>> payoffs;
            payoffs.push_back(std::make_pair(m_treeptr->GetMaturity(), payoff));
            return GetPrice(payoffs);
        }
    }
}
