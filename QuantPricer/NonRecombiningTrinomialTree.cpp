//
//  NonRecombiningTrinomialTree.cpp
//  QuantPricer
//
//  Created by Swati Mital on 15/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include "NonRecombiningTrinomialTree.h"

NonRecombiningTrinomialTree::NonRecombiningTrinomialTree(double S0, double sigma, double rf_rate, double dividend, double T, double steps)
: TrinomialTree(S0, sigma, rf_rate, dividend, T, steps)
{
    m_root = BuildUnderlyingTree(S0, NodeDir::Middle, 0);
    BreadthFirstTraversal(m_root);
}


NodePtr NonRecombiningTrinomialTree::BuildUnderlyingTree(double val, NodeDir ndir, int tree_level)
{
    NodePtr nd;
    
    if (tree_level > m_steps)
        return nullptr;
    
    if (ndir == NodeDir::Up) {
        nd = boost::make_shared<Node>(m_up_factor*val);
    } else if(ndir == NodeDir::Down) {
        nd = boost::make_shared<Node>(m_down_factor*val);
    } else {
        nd = boost::make_shared<Node>(val);
    }
    
    
    nd->middle = BuildUnderlyingTree(std::get<0>(nd->values), NodeDir::Middle, tree_level+1);
    nd->up = BuildUnderlyingTree(std::get<0>(nd->values), NodeDir::Up, tree_level+1);
    nd->down = BuildUnderlyingTree(std::get<0>(nd->values), NodeDir::Down, tree_level+1);
    
    return nd;
}
