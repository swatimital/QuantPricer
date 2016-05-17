//
//  BarenblattTrinomialTree.cpp
//  QuantPricer
//
//  Created by Swati Mital on 16/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include "BarenblattTrinomialTree.h"

BarenblattTrinomialTree::BarenblattTrinomialTree(double S0, double sigma, double rf, double dividend, double T, double steps) : RecombiningTrinomialTree(S0, sigma, rf, dividend, T, steps)
{
    m_sigma_max = 2.0*m_sigma;
}

void BarenblattTrinomialTree::InitializeTree()
{
    ComputeAssetPriceFactors();
    ComputeNodeProbabilities(m_up_prob, m_middle_prob, m_down_prob);
}

void BarenblattTrinomialTree::ComputeAssetPriceFactors()
{
    m_up_factor = exp(m_sigma_max*sqrt(m_dt)+m_dt);
    m_middle_factor = exp(m_rf_rate*m_dt);
    m_down_factor = 1/m_up_factor;
}

