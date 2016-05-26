//
//  VanillaOptionPricer.cpp
//  QuantPricer
//
//  Created by Swati Mital on 15/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include "RecombiningTreeOptionPricer.h"
#include "RecombiningTrinomialTree.h"
#include <map>

namespace QuantPricer
{
    namespace PricingEngine
    {
        RecombiningTreeOptionPricer::RecombiningTreeOptionPricer(double sigma, double rf, double div, double T) : OptionPricer()
        {
            m_treeptr = boost::make_shared<FiniteDiffMethods::RecombiningTrinomialTree>(FiniteDiffMethods::RecombiningTrinomialTree(1.0, sigma, rf, div, T, 100.0));
        }
        
        RecombiningTreeOptionPricer::RecombiningTreeOptionPricer(TreePtr treePtr) : OptionPricer()
        {
            m_treeptr = treePtr;
        }
        
        RecombiningTreeOptionPricer::~RecombiningTreeOptionPricer()
        {}
        
        double RecombiningTreeOptionPricer::GetPrice(boost::function<double(double)> payoff)
        {
            std::vector<std::pair<double, boost::function<double(double)>>> payoffs;
            payoffs.push_back(std::make_pair(m_treeptr->GetMaturity(), payoff));
            return GetPrice(payoffs);
        }
        
        double RecombiningTreeOptionPricer::GetPrice(std::vector<std::pair<double, boost::function<double(double)>>> payoffs)
        {
            std::map<int, boost::function<double(double)>> starttime_payoff_map;
            for (int j = 0; j < payoffs.size(); j++)
            {
                starttime_payoff_map.insert(std::make_pair(m_treeptr->GetNodeStartFromTimeStep(payoffs[j].first), payoffs[j].second));
            }
            
            m_treeptr->InitializeTree();
            auto nodes = m_treeptr->GetBreadthFirstNodeValues();
            auto n = m_treeptr->GetLevel();
            auto dt = m_treeptr->GetMaturity()/n;
            auto rf = m_treeptr->GetRiskFreeRate();
            auto end = nodes.size();
            auto start = nodes.size() - (2*n+1);
            double up_prob, mid_prob, down_prob;
            m_treeptr->ComputeNodeProbabilities(up_prob, mid_prob, down_prob);
            auto map_it = starttime_payoff_map.find(start);
            
            auto found = true;
            if (map_it == starttime_payoff_map.end())
                found = false;
            
            // Set the option values on the leaf nodes
            for (auto i = start; i < end; i++)
            {
                std::get<1>(nodes[i]->values) = (map_it != starttime_payoff_map.end()) ? (map_it->second)(std::get<0>(nodes[i]->values)) : 0;
            }
            
            while (start >= 1)
            {
                end = start;
                start = start - (2*(--n)+1);
                auto j = end;
                auto map_it = starttime_payoff_map.find(start);
                
                for (auto i = start; i < end; i++,j++)
                {
                    
                    std::get<1>(nodes[i]->values) = (map_it != starttime_payoff_map.end()) ? (map_it->second)(std::get<0>(nodes[i]->values)) + exp(-rf*dt)*(up_prob*std::get<1>(nodes[j]->values) +
                                                                 mid_prob*std::get<1>(nodes[j+1]->values) +
                                                                                                                                                            down_prob*std::get<1>(nodes[j+2]->values)) : exp(-rf*dt)*(up_prob*std::get<1>(nodes[j]->values) +
                                                                                                                                                                                                                      mid_prob*std::get<1>(nodes[j+1]->values) +
                                                                                                                                                                                                                      down_prob*std::get<1>(nodes[j+2]->values));
                }
                
            }
            
            return std::get<1>(nodes[0]->values);
            
        }
    }
}


