//
//  main.cpp
//  QuantPricer
//
//  Created by Swati Mital on 06/03/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include <iostream>
#include <boost/function.hpp>
#include <map>
#include <vector>
#include "VanillaOptionPricer.h"

int main(int argc, const char * argv[])
{
    VanillaOptionPricer option_pricer(0.5, 0.1, 0.0, 0.5);
    double call_price = option_pricer.GetOptionPrice(100.0, 90.0, OptionType::Call);
    double put_price = option_pricer.GetOptionPrice(100.0, 90.0, OptionType::Put);
    
    std::cout << "Call Option Price is " << call_price << std::endl;
    std::cout << "Put Option Price is " << put_price << std::endl;
}
