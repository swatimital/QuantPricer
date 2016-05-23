//
//  Derivative.h
//  QuantPricer
//
//  Created by Swati Mital on 21/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef QuantPricer_Derivative_h
#define QuantPricer_Derivative_h

#include <boost/Optional.hpp>

struct Greeks
{
    boost::optional<double> delta;
    boost::optional<double> vega;
    boost::optional<double> gamma;
    boost::optional<double> rho;
};

template<typename ValueT>
struct Derivative
{
    ValueT value;
    boost::optional<Greeks> greeks;
    Derivative(ValueT val) : value(val) {}
};


#endif
