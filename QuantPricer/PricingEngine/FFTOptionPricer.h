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
            
            /** @brief PrecomputeFFTOptionPrices Computes ATM/OTM Put/Call option prices assuming that current spot is 1 using the Fast Fourier Transform technique as described in paper by Carr and Madan
             * @param risk_free_rate: Risk Free Rate
             * @param dividend: Dividend Rate
             * @param time_to_maturity: Time to Maturity
             * @param at_the_money: Use ATM technique or time value technique for option pricing
             * @param call: true for call options and false for put options
             * @param char_fn_method: Characteristic Function of the Underlying
             * @param fft_method: FFT Method that should be used to calculate the call prices   
             * @return: Put/Call Prices vector
             */
            std::vector<std::pair<double, double>> PrecomputeFFTOptionPrices(
                                    double risk_free_rate,
                                    double dividend,
                                    double time_to_maturity,
                                    bool at_the_money,
                                    bool call,
                                    boost::shared_ptr<CharacteristicFunctionMethods::ICharacteristicFunctionMethod> char_fn_method, 
                                    boost::shared_ptr<FFTMethods::BaseFFTMethod> fft_method);
            
            /** @brief GetPutCallPrice Computes put/call price using the Fast Fourier Transform technique as described in paper by Carr and Madan
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
                                boost::shared_ptr<FFTMethods::BaseFFTMethod> fft_method);
            

            /** @brief LinearInterpolateStrike Linear Interpolation to get the price of the option at strike K
             * @param K: Strike of the option
             * @param strike_option_price_pair: Pairs of Strikes and Option Prices
             * @return: Price of the option
             */
            double LinearInterpolateStrike(double K, 
                                           std::vector<std::pair<double, double>> strike_option_price_pair);
            
                   };
        
    }
}


#endif /* defined(__QuantPricer__FFTOptionPricer__) */
