//
//  TrinomialTree.h
//  QuantPricer
//
//  Created by Swati Mital on 11/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef __QuantPricer__TrinomialTree__
#define __QuantPricer__TrinomialTree__

#include <stdio.h>
#include <math.h>
#include <vector>
#include <tuple>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <queue>

enum NodeDir {Up, Middle, Down};

struct Node
{
    std::tuple<double, double> values;
    boost::shared_ptr<Node> up;
    boost::shared_ptr<Node> middle;
    boost::shared_ptr<Node> down;
    
    Node(){}
    Node(double underlying, double derivative=-1.0):values(std::make_tuple(underlying, derivative)){}
};

typedef boost::shared_ptr<Node> NodePtr;

class TrinomialTree
{
public:
    TrinomialTree();
    TrinomialTree(double S0, double sigma, double rf, double dividend, double T, double steps);
    virtual void InitializeTree();
    virtual double GetLevel();
    virtual double GetMaturity();
    virtual double GetRiskFreeRate();
    std::vector<NodePtr> GetBreadthFirstNodeValues() const;
    virtual void ComputeNodeProbabilities(double& pu, double& pm, double& pd);
    virtual ~TrinomialTree();
    
protected:
    virtual void BreadthFirstTraversal(NodePtr nd);
    virtual NodePtr BuildUnderlyingTree(double val, NodeDir ndir, int tree_level) = 0;
    virtual void ComputeAssetPriceFactors();
protected:
    double m_S0;
    double m_sigma;
    double m_rf_rate;
    double m_dividend;
    double m_T;
    double m_steps;
    double m_dt;
    double m_up_factor;
    double m_down_factor;
    double m_middle_factor;
    double m_up_prob, m_middle_prob, m_down_prob;
    NodePtr m_root;
    std::vector<NodePtr> m_bf_nodes;
    bool m_initialized;
    
};

#endif /* defined(__QuantPricer__TrinomialTree__) */
