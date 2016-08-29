//
//  Maths.h
//  QuantPricer
//
//  Created by Swati Mital on 28/08/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef QuantPricer_Maths_h
#define QuantPricer_Maths_h

namespace QuantPricer
{
    namespace common
    {
        namespace Maths
        {
            inline double kronecker_delta(int j)
            {
                return (j==0 ? 1 : 0);
            }
        }
    }
}

#endif
