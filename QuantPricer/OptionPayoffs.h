//
//  OptionPayoffs.h
//  QuantPricer
//
//  Created by Swati Mital on 21/05/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef QuantPricer_OptionPayoffs_h
#define QuantPricer_OptionPayoffs_h

#include <boost/assert.hpp>

namespace OptionPayoffs {
   
    inline double VanillaCallOption(double prop_ST, double St, double K)
    {
        return std::max<double>(prop_ST*St - K, 0);
    }
    
    inline double VanillaPutOption(double prop_ST, double St, double K)
    {
        return std::max<double>(K - prop_ST*St, 0);
    }
    
    inline double BullCallSpread(double prop_ST, double St, double K_low, double K_high)
    {
        BOOST_ASSERT_MSG(K_low <= K_high, "K_low must be lower than K_high");
        double ST = prop_ST*St;
        if (ST < K_low) return 0.0;
        else if (ST >= K_low && ST <= K_high) return (ST-K_low);
        else return K_high - K_low;
    }
    
    inline double BearCallSpread(double prop_ST, double St, double K_low, double K_high)
    {
        BOOST_ASSERT_MSG(K_low <= K_high, "K_low must be lower than K_high");
        double ST = prop_ST*St;
        if (ST < K_low) return K_high - K_low;
        else if (ST >= K_low && ST <= K_high) return (K_high - ST);
        else return 0.0;
    }
    
    inline double ButterflySpread(double prop_ST, double St, double K_low, double K_mid, double K_high)
    {
        BOOST_ASSERT_MSG(K_low <= K_mid, "K_low must be lower than K_mid");
        BOOST_ASSERT_MSG(K_mid <= K_high, "K_mid must be lower than K_high");
        double ST = prop_ST*St;
        if (ST < K_low) return 0.0;
        else if (ST >= K_low && ST <= K_mid) return (ST - K_low);
        else if (ST > K_mid && ST <= K_high) return (K_high - ST);
        else return 0.0;
        
    }
}


#endif
