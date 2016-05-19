//
//  BarenblattTrinomialTree.cpp
//  QuantPricer
//
//  Created by Swati Mital on 16/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include "BarenblattTrinomialTree.h"

BarenblattTrinomialTree::BarenblattTrinomialTree(double S0, double sigma_max, double sigma_min, double rf, double dividend, double T, double steps) : TrinomialTree(S0, (sigma_min+sigma_max)*0.5, rf, dividend, T, steps), m_sigma_max(sigma_max), m_sigma_min(sigma_min)
{
}

void BarenblattTrinomialTree::InitializeTree()
{
    if (!m_initialized)
    {
        ComputeAssetPriceFactors();
        ComputeNodeProbabilities(m_up_prob, m_middle_prob, m_down_prob);
        m_initialized = true;
    }
}

void BarenblattTrinomialTree::ComputeAssetPriceFactors()
{
    m_up_factor = exp(m_sigma_max*sqrt(m_dt)+m_dt);
    m_middle_factor = exp(m_rf_rate*m_dt);
    m_root = BuildUnderlyingTree(m_S0, NodeDir::Middle, 0);
    BreadthFirstTraversal(m_root);
    m_down_factor = 1/m_up_factor;
}

double BarenblattTrinomialTree::GetSigmaMax() const { return m_sigma_max; }
double BarenblattTrinomialTree::GetSigmaMin() const { return m_sigma_min; }

boost::shared_ptr<Node<double, std::tuple<double, double>>> BarenblattTrinomialTree::BuildUnderlyingTree(double val, NodeDir ndir, int tree_level)
{
    boost::shared_ptr<Node<double, std::tuple<double, double>>> nd;
    
    if (tree_level > m_steps)
        return nullptr;
    
    if (ndir == NodeDir::Up) {
        nd = boost::make_shared<Node<double, std::tuple<double, double>>>(m_up_factor*val, std::make_tuple<double, double>(-1.0, -1.0));
    } else if(ndir == NodeDir::Down) {
        nd = boost::make_shared<Node<double, std::tuple<double, double>>>(m_down_factor*val, std::make_tuple<double, double>(-1.0, -1.0));
    } else {
        nd = boost::make_shared<Node<double, std::tuple<double, double>>>(m_middle_factor*val, std::make_tuple<double, double>(-1.0, -1.0));
    }
    
    if (tree_level == 0) {
        nd->up = BuildUnderlyingTree(std::get<0>(nd->values), NodeDir::Up, tree_level+1);
        nd->middle = BuildUnderlyingTree(std::get<0>(nd->values), NodeDir::Middle, tree_level+1);
        nd->down = BuildUnderlyingTree(std::get<0>(nd->values), NodeDir::Down, tree_level+1);
    } else {
        if (ndir == NodeDir::Up) {
            nd->up = BuildUnderlyingTree(std::get<0>(nd->values), NodeDir::Up, tree_level+1);
            nd->middle = BuildUnderlyingTree(std::get<0>(nd->values), NodeDir::Middle, tree_level+1);
        } else if (ndir == NodeDir::Middle){
            nd->middle = BuildUnderlyingTree(std::get<0>(nd->values), NodeDir::Middle, tree_level+1);
        } else if (ndir == NodeDir::Down) {
            nd->middle = BuildUnderlyingTree(std::get<0>(nd->values), NodeDir::Middle, tree_level+1);
            nd->down = BuildUnderlyingTree(std::get<0>(nd->values), NodeDir::Down, tree_level+1);
        }
    }
    
    return nd;
}
