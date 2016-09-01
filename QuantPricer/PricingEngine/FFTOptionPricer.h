//
//  FFTOptionPricer.h
//  QuantPricer
//
//  Created by Swati Mital on 21/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef __QuantPricer__FFTOptionPricer__
#define __QuantPricer__FFTOptionPricer__

#include <stdio.h>
#include "boost/shared_ptr.hpp"
#include "ICharacteristicFunctionMethod.h"
#include "BaseFFTMethod.h"

namespace QuantPricer
{
    namespace PricingEngine
    {
        class FFTOptionPricer
        {
        public:
            FFTOptionPricer() {};
            virtual ~FFTOptionPricer() {};
            
            /** @brief GetPutCallPrice Computes price of put or call options using the Fast Fourier Inversion technique as described in paper by Carr and Madan. This is method DOES NOT cache the precomputed option prices for different strikes selected by the function. If you want to recompute option price for the same underlying and same time to maturity then you can call "PrecomputeATMFFTOptionPrices" and "PrecomputeOTMFFTOptionPrices" followed by "LinearInterpolateStrike".
             * @param St: Spot price of the stock
             * @param K: Strike price of the stock
             * @param risk_free_rate: Risk Free Rate
             * @param dividend: Dividend Rate
             * @param time_to_maturity: Time to Maturity
             * @param call: true for call options and false for put options
             * @param char_fn_method: Characteristic Function of the Underlying
             * @param fft_method: FFT Method that should be used to calculate the call prices   
             * @return: Put/Call Price(St,K,rf,q,T)
             */
            double GetPutCallPrice(double St,
                                   double K,
                                   double risk_free_rate,
                                   double dividend,
                                   double time_to_maturity,
                                   bool call,
                                   boost::shared_ptr<CharacteristicFunctionMethods::ICharacteristicFunctionMethod> char_fn_method, 
                                   boost::shared_ptr<FFTMethods::BaseFFTMethod> fft_method) const;
            
            
            /** @brief PrecomputeFFTOptionPrices Computes option prices for Puts or Calls using the Fast Fourier Inversion technique as described in paper by Carr and Madan. The model used for the dynamics of the underlying is determined by the characteristic function. The Strikes are selected to be in an interval around the current spot.
             * @param St: Spot price of the stock
             * @param risk_free_rate: Risk Free Rate
             * @param dividend: Dividend Rate
             * @param time_to_maturity: Time to Maturity
             * @param at_the_money: true for at the money technique and false for out of the money technique
             * @param call: true for call options and false for put options
             * @param char_fn_method: Characteristic Function of the Underlying
             * @param fft_method: FFT Method that should be used to calculate the call prices   
             * @return: Vector of pair of Strikes and the option values.
             */
            std::vector<std::pair<double, double>> PrecomputeFFTOptionPrices(
                                    double St,
                                    double risk_free_rate,
                                    double dividend,
                                    double time_to_maturity,
                                    bool at_the_money,
                                    bool call,
                                    boost::shared_ptr<CharacteristicFunctionMethods::ICharacteristicFunctionMethod> char_fn_method, 
                                    boost::shared_ptr<FFTMethods::BaseFFTMethod> fft_method) const;
        
            /** @brief LinearInterpolateStrike Linear Interpolation to get the price of the option at strike K
             * @param K: Strike of the option
             * @param strike_option_price_pair: Pairs of Strikes and Option Prices
             * @return: Price of the option
             */
            double LinearInterpolateStrike(double K, 
                                           std::vector<std::pair<double, double>> 
                                                strike_option_price_pair) const;
            
        };
        
    }
}


#endif /* defined(__QuantPricer__FFTOptionPricer__) */
