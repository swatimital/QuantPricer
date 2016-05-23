//
//  TreeNode.h
//  QuantPricer
//
//  Created by Swati Mital on 21/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef QuantPricer_TreeNode_h
#define QuantPricer_TreeNode_h

namespace QuantPricer
{
    namespace FiniteDiffMethods
    {
        template<typename UnderlyingT, typename DerivativeT>
        struct Node
        {
            std::tuple<UnderlyingT, DerivativeT> values;
            boost::shared_ptr<Node> up;
            boost::shared_ptr<Node> middle;
            boost::shared_ptr<Node> down;
            
            Node(){}
            Node(UnderlyingT underlying, DerivativeT derivative):values(std::make_tuple(underlying, derivative)){}
        };
    }
}


#endif
