//
//  TrinomialTree.cpp
//  QuantPricer
//
//  Created by Swati Mital on 11/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include "TrinomialTree.h"

TrinomialTree::TrinomialTree()
{}

TrinomialTree::TrinomialTree(double S0, double sigma, double rf_rate, double dividend, double T, double steps)
:m_sigma(sigma), m_T(T), m_steps(steps), m_dividend(dividend), m_rf_rate(rf_rate)
{
    m_dt = m_T/m_steps;
    m_up_factor = exp(sigma*sqrt(2.0*m_dt));
    m_down_factor = 1/m_up_factor;
    ComputeNodeProbabilities(m_up_prob, m_middle_prob, m_down_prob);
}

TrinomialTree::~TrinomialTree()
{}

void TrinomialTree::ComputeNodeProbabilities(double& pu, double& pm, double& pd)
{
    double r_factor = exp((m_rf_rate-m_dividend)*m_dt*0.5);
    double pu_factor = exp(m_sigma*sqrt(0.5*m_dt));
    double pd_factor = 1/pu_factor;
    pu = (r_factor-pd_factor)/(pu_factor-pd_factor);
    pu *= pu;
    pd = (pu_factor-r_factor)/(pu_factor-pd_factor);
    pd *= pd;
    pm = 1 - (pu+pd);
}


std::vector<NodePtr> TrinomialTree::GetBreadthFirstNodeValues() const
{
    return m_bf_nodes;
}

void TrinomialTree::BreadthFirstTraversal(NodePtr nd)
{
    std::queue<NodePtr> nodes;
    nodes.push(nd);
    
    while (!nodes.empty())
    {
        NodePtr n = nodes.front();
        nodes.pop();
        m_bf_nodes.push_back(n);
        if (n->up != nullptr)
            nodes.push(n->up);
        if (n->middle != nullptr)
            nodes.push(n->middle);
        if (n->down != nullptr)
            nodes.push(n->down);
        
    }
    
}


