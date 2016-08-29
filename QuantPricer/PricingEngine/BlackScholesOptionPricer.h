//
//  BlackScholesOptionPricer.h
//  QuantPricer
//
//  Created by Swati Mital on 19/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//  Recipes from: http://finance.bi.no/~bernt/gcc_prog/recipes/recipes/node7.html
//

#ifndef __QuantPricer__BlackScholesOptionPricer__
#define __QuantPricer__BlackScholesOptionPricer__

#include <stdio.h>
#include <math.h>

// Ref: http://www.johndcook.com/blog/cpp_phi/

namespace QuantPricer
{
    namespace PricingEngine
    {
        namespace BlackScholesOptionPricer
        {
            inline double Phi(double x, double mean = 0.0, double sigma = 1.0)
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

            inline double BSPrice(double St, 
                                  double K, 
                                  double time,
                                  double sigma, 
                                  double rf, 
                                  double div, 
                                  bool call = true)
            {
                double d1 = (log(St/K) + 
                             (rf+sigma*sigma*0.5)*(time)) / (sigma*sqrt(time));
                double d2 = d1 - sigma*sqrt(time);
                return (call ? (St*Phi(d1) - exp(-rf*(time))*K*Phi(d2)) : (exp(-rf*(time))*K*Phi(-d2) - St*Phi(-d1)));
            }
            
            inline double BSDelta(double St, 
                                  double K, 
                                  double time,
                                  double sigma, 
                                  double rf, 
                                  double div, 
                                  bool call=true)
            {
                return 1.0;
            }
            
            inline double BSVega(double St, 
                                 double K, 
                                 double time,
                                 double sigma, 
                                 double rf, 
                                 double div)
            {
                double d1 = (log(St/K) + 
                             (rf+sigma*sigma*0.5)*(time)) / (sigma*sqrt(time));
                return St*sqrt(time)*Phi(d1);
            }
            
            inline double ImpliedVolatility(double Vt, 
                                            double St, 
                                            double K, 
                                            double time,
                                            double rf, 
                                            double div, 
                                            bool call=true)
            {
                double sigma_low = 1e-5;
                double price = BSPrice(St, K, time, sigma_low, rf, div, call);
                if (price > Vt) return 0.0;
                
                const int MAX_ITERATIONS = 100;
                const double ACCURACY = 1.0e-4;
                double sigma = (Vt/St)/(0.398*sqrt(time));
                
                for (int i = 0; i < MAX_ITERATIONS; i++)
                {
                    price = BSPrice(St, K, time, sigma, rf, div, call);
                    double diff = Vt - price;
                    if (fabs(diff) < ACCURACY) return sigma;
                    double vega = BSVega(St, K, time, sigma, rf, div);
                    sigma = sigma + diff/vega;
                }
                
                throw std::runtime_error("The IV calculation did not converge");
            }
        }
    }
}

#endif /* defined(__QuantPricer__BlackScholesOptionPricer__) */
