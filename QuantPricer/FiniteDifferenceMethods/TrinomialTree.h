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
#include "TreeNode.h"

enum NodeDir {Up, Middle, Down};

namespace QuantPricer
{
    namespace FiniteDiffMethods
    {
        template<typename UnderlyingT, typename DerivativeT>
        class TrinomialTree
        {
        public:
            TrinomialTree()
            {}
            
            TrinomialTree(double S0, double sigma, double rf, double dividend, double T, double steps)
            :m_S0(S0), m_sigma(sigma), m_T(T), m_steps(steps), m_dividend(dividend), m_rf_rate(rf), m_initialized(false)
            {
                m_dt = m_T/m_steps;
            }
            
            
            virtual void InitializeTree()
            {
                if (!m_initialized)
                {
                    ComputeAssetPriceFactors();
                    ComputeNodeProbabilities(m_up_prob, m_middle_prob, m_down_prob);
                }
            }
            
            virtual double GetLevel()
            {
                return m_steps;
            }
            
            virtual double GetMaturity()
            {
                return m_T;
            }
            
            virtual double GetRiskFreeRate()
            {
                return m_rf_rate;
            }
            
            std::vector<boost::shared_ptr<Node<UnderlyingT, DerivativeT>>> GetBreadthFirstNodeValues() const
            {
                return m_bf_nodes;
            }
            
            virtual void ComputeNodeProbabilities(double& pu, double& pm, double& pd)
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
            
            virtual ~TrinomialTree() {}
            
        protected:
            virtual void BreadthFirstTraversal(boost::shared_ptr<Node<UnderlyingT, DerivativeT>> nd)
            {
                std::queue<boost::shared_ptr<Node<UnderlyingT, DerivativeT>>> nodes;
                nodes.push(nd);
                
                while (!nodes.empty())
                {
                    boost::shared_ptr<Node<UnderlyingT, DerivativeT>> n = nodes.front();
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
            
            virtual boost::shared_ptr<Node<UnderlyingT, DerivativeT>> BuildUnderlyingTree(double val, NodeDir ndir, int tree_level) = 0;
            virtual void ComputeAssetPriceFactors()
            {
                m_up_factor = exp(m_sigma*sqrt(2.0*m_dt));
                m_down_factor = 1/m_up_factor;
                m_middle_factor = 1.0;
            }
            
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
            boost::shared_ptr<Node<UnderlyingT, DerivativeT>> m_root;
            std::vector<boost::shared_ptr<Node<UnderlyingT, DerivativeT>>> m_bf_nodes;
            bool m_initialized;
            
        };

    }
}


#endif /* defined(__QuantPricer__TrinomialTree__) */
