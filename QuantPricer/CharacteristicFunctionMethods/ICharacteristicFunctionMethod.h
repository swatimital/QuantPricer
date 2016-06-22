//
//  ICharacteristicFunctionMethod.h
//  QuantPricer
//
//  Created by Swati Mital on 21/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef QuantPricer_ICharacteristicFunctionMethod_h
#define QuantPricer_ICharacteristicFunctionMethod_h

#include <complex>

namespace QuantPricer
{
    namespace CharacteristicFunctionMethods
    {
        class ICharacteristicFunctionMethod
        {
        public:
            virtual std::complex<double> CharacteristicFunction(std::complex<double> u) = 0;
        };
    }
}


#endif
