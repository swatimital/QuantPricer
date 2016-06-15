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
   
    inline double LongVanillaCallOption(double prop_ST, double St, double K)
    {
        return std::max<double>(prop_ST*St - K, 0);
    }
    
    inline double ShortVanillaCallOption(double prop_ST, double St, double K)
    {
        return -1.0*LongVanillaCallOption(prop_ST, St, K);
    }
    
    inline double LongVanillaPutOption(double prop_ST, double St, double K)
    {
        return std::max<double>(K - prop_ST*St, 0);
    }
    
    inline double ShortVanillaPutOption(double prop_ST, double St, double K)
    {
        return -1.0*LongVanillaPutOption(prop_ST, St, K);
    }
    
    inline double LongStraddle(double prop_ST, double St, double K_put, double K_call)
    {
        BOOST_ASSERT_MSG(K_put <= K_call, "K_put must be lower than K_call");
        double ST = prop_ST*St;
        if (ST < K_put) return K_put - ST;
        else if (ST >= K_put && ST < K_call) return 0;
        else return ST - K_call;
    }
    
    inline double ShortStraddle(double prop_ST, double St, double K_call, double K_put)
    {
        BOOST_ASSERT_MSG(K_call <= K_put, "K_call must be lower than K_put");
        double ST = prop_ST*St;
        if (ST < K_call) return ST - K_call;
        else if (ST >= K_call && ST < K_put) return 0;
        else return K_put - ST;
    }
    
    inline double LongStrangle(double prop_ST, double St, double K)
    {
        double ST = prop_ST*St;
        if (ST < K) return K - ST;
        else return ST - K;
    }
    
    inline double ShortStrangle(double prop_ST, double St, double K)
    {
        return -1*LongStrangle(prop_ST, St, K);
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
    
    inline double LongButterflySpread(double prop_ST, double St, double K_low, double K_mid, double K_high)
    {
        BOOST_ASSERT_MSG(K_low <= K_mid, "K_low must be lower than K_mid");
        BOOST_ASSERT_MSG(K_mid <= K_high, "K_mid must be lower than K_high");
        double ST = prop_ST*St;
        if (ST < K_low) return 0.0;
        else if (ST >= K_low && ST <= K_mid) return (ST - K_low);
        else if (ST > K_mid && ST <= K_high) return (K_high - ST);
        else return 0.0;
    }
    
    
    inline double ShortButterflySpread(double prop_ST, double St, double K_low, double K_mid, double K_high)
    {
        BOOST_ASSERT_MSG(K_low <= K_mid, "K_low must be lower than K_mid");
        BOOST_ASSERT_MSG(K_mid <= K_high, "K_mid must be lower than K_high");
        return -1*LongButterflySpread(prop_ST, St, K_low, K_mid, K_high);
    }
}


#endif
