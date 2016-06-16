//
//  MemoryManagement.h
//  QuantPricer
//
//  Created by Swati Mital on 16/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef QuantPricer_MemoryManagement_h
#define QuantPricer_MemoryManagement_h

namespace QuantPricer
{
    namespace common
    {
        template<typename T>
        struct array_deleter
        {
            void operator()(T const* p)
            {
                delete[] p;
            }
        };
        
        struct free_delete
        {
            void operator()(void* x) { free(x); }
        };
    }
}

#endif
