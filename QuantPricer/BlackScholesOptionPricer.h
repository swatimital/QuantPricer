//
//  BlackScholesOptionPricer.h
//  QuantPricer
//
//  Created by Swati Mital on 19/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef __QuantPricer__BlackScholesOptionPricer__
#define __QuantPricer__BlackScholesOptionPricer__

#include <stdio.h>
#include <math.h>

// Ref: http://www.johndcook.com/blog/cpp_phi/

static double Phi(double x, double mean = 0.0, double sigma = 1.0)
{
    // constants
    double a1 =  0.254829592;
    double a2 = -0.284496736;
    double a3 =  1.421413741;
    double a4 = -1.453152027;
    double a5 =  1.061405429;
    double p  =  0.3275911;
    
    // Save the sign of x
    int sign = 1;
    if (x < 0)
        sign = -1;
    x = fabs(x)/sqrt(2.0);
    
    // A&S formula 7.1.26
    double t = 1.0/(1.0 + p*x);
    double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);
    
    return 0.5*(1.0 + sign*y);
}

static double BSPrice(double St, double K, double t, double T, double sigma, double rf, double div, bool call = true)
{
    double d1 = (log(St/K) + (rf+sigma*sigma*0.5)*(T-t)) / (sigma*sqrt(T-t));
    double d2 = d1 - sigma*sqrt(T-t);
    return (call ? (St*Phi(d1) - exp(-rf*(T-t))*K*Phi(d2)) : (exp(-rf*(T-t))*K*Phi(-d2) - St*Phi(-d1)));
}

#endif /* defined(__QuantPricer__BlackScholesOptionPricer__) */
