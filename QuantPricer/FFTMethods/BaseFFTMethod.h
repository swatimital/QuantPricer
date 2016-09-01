//
//  BaseFFTMethod.h
//  QuantPricer
//
//  Created by Swati Mital on 16/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef QuantPricer_BaseFFTMethod_h
#define QuantPricer_BaseFFTMethod_h

#include <complex>
#include "boost/shared_ptr.hpp"
#include "boost/function.hpp"
#include "FastFourierTransform.h"

namespace QuantPricer
{
    namespace FFTMethods
    {
        class BaseFFTMethod
        {
        public:
            
            BaseFFTMethod();
            virtual ~BaseFFTMethod() {};
            
            /** @brief FFTEuropeanOptionPrices Given closed-form characteristic function this function computes a pair of Strikes and Put/Call Prices using Carr and Madan's FFT technique.
             * @param characteristic_fn: Characteristic Function of the stochastic Process
             * @param risk_free_rate: Risk Free Rate
             * @param dividend: Dividend Rate
             * @param time_to_maturity: Time to Maturity
             * @param at_the_money: whether the strike is at the money or not  
             * @param call: true if we want to compute call prices, false for put prices
             * @return: A pair of strikes and call prices at those strikes
             */
            virtual std::vector<std::pair<double, double>> FFTEuropeanOptionPrices(
                     boost::function<std::complex<double>(std::complex<double>)> characteristic_fn,
                     double St,
                     double risk_free_rate,
                     double dividend,
                     double time_to_maturity,
                     bool at_the_money,
                     bool call) const;
            
            
            /** @brief FFTOutputVector Given closed-form characteristic function of the underlying and a closed form Fourier Transform of the modified call price, this function performs a 1D Inverse Discrete Fourier Transform in order to compute the call prices. The technique implemented is from paper by Carr and Madan.            
             * @param fourier_transform: Closed form Fourier Transform of the modified call price            
             * @param characteristic_fn: Characteristic Function of the stochastic Process
             * @param log_strike_level: Log Strike Level centered around At the Money Log Price
             * @param risk_free_rate: Risk Free Rate
             * @param time_to_maturity: Time to Maturity of the option
             * @return: Inverse Fourier Transform to the get the modified call prices.
             */
            virtual std::vector<std::complex<double>> FFTOutputVector(
                  boost::function<std::complex<double>(
                          double, 
                          boost::function<std::complex<double>(std::complex<double>)>,
                          double, 
                          double)> fourier_transform, 
                  boost::function<std::complex<double>(std::complex<double>)> characteristic_fn,
                  double log_strike_level,
                  double risk_free_rate,
                  double time_to_maturity) const;
            
            /** @brief Given closed-form characteristic function of the underlying and a closed form Fourier Transform of the modified call price, this function performs a 1D Inverse Discrete Fourier Transform in order to compute the call prices. The technique implemented is from paper by Carr and Madan.            
             * @param fourier_transform: Closed form Fourier Transform of the modified call price            
             * @param characteristic_fn: Characteristic Function of the stochastic Process
             * @param log_strike_level: Log Strike Level centered around At the Money Log Price
             * @param risk_free_rate: Risk Free Rate
             * @param time_to_maturity: Time to Maturity of the option
             * @return: Inverse Fourier Transform to the get the modified call prices.
             */
            virtual std::vector<std::complex<double>> FFTInputVector(
                  boost::function<std::complex<double>(
                       double,
                       boost::function<std::complex<double>(std::complex<double>)>, 
                       double, 
                       double)> fourier_transform, 
                  boost::function<std::complex<double>(std::complex<double>)> characteristic_fn,
                  double log_strike_level,
                  double risk_free_rate,
                  double time_to_maturity) const;
            
            
        protected:
            /** @brief Compute the Psi Function that is the Fourier Transform for the at the money modified call option prices
             * @param u: Parameter where Gamma Function needs to be computed at
             * @param characteristic_fn: Characteristic Function of the underlying 
             * @param risk_free_rate: Risk Free Rate
             * @param time_to_maturity: Time to Maturity
             * @return: Returns the output from the Gamma Funtion
             */
            virtual std::complex<double> PsiFunction(double u,
                                                     boost::function<std::complex<double>(std::complex<double>)> characteristic_fn,
                                                     double risk_free_rate,
                                                     double time_to_maturity) const;
            
            /** @brief Compute the Gamma Function that is the fourier transform for the time value of the modified call prices.
             * @param u: Parameter where Gamma Function needs to be computed at
             * @param characteristic_fn: Characteristic Function of the underlying 
             * @param risk_free_rate: Risk Free Rate
             * @param time_to_maturity: Time to Maturity
             * @return: Returns the output from the Gamma Funtion
             */
            virtual std::complex<double> GammaFunction(double u,
                                                       boost::function<std::complex<double>(std::complex<double>)> characteristic_fn,
                                                       double risk_free_rate,
                                                       double time_to_maturity) const;
            
            /** @brief Compute the Zeta Function that is used for the computation of the Gamma Function.
             * @param u: Complex Parameter where Zeta Function needs to be computed at
             * @param characteristic_fn: Characteristic Function of the underlying 
             * @param risk_free_rate: Risk Free Rate
             * @param time_to_maturity: Time to Maturity
             * @return: Returns the output from the Zeta Funtion
             */
            virtual std::complex<double> ZetaFunction(std::complex<double> u,
                                                      boost::function<std::complex<double>(std::complex<double>)> characteristic_fn,
                                                      double risk_free_rate,
                                                      double time_to_maturity) const;
            
            /** @brief  Computes an additional integrand as described in this MSc thesis:
             * http://ta.twi.tudelft.nl/mf/users/oosterle/oosterlee/ng.pdf by the function g(.) on Page 26.
             * @param log_strike: Log Strike of the call option
             * @param fourier_transform: Fourier Transform (analytic) of the modified call price
             * @param characteristic_fn: Characteristic Function of the underlying 
             * @param risk_free_rate: Risk Free Rate
             * @param time_to_maturity: Time to Maturity
             * @return: Returns the output of g_N
             */
            std::complex<double> IntegrandSum(double log_strike,
             boost::function<std::complex<double>(double,
                                                  boost::function<std::complex<double>(std::complex<double>)>,
                                                  double, 
                                                  double)> fourier_transform,
             boost::function<std::complex<double>(std::complex<double>)> characteristic_fn,
             double risk_free_rate, double time_to_maturity) const;
            
            /** @brief Simple sum of the inverse fourier transform. Uses N^2 steps
             * @param fourier_transform: Fourier Transform (analytic) of the modified call price
             * @param characteristic_fn: Characteristic Function of the underlying 
             * @param risk_free_rate: Risk Free Rate
             * @param time_to_maturity: Time to Maturity
             * @param log_strike: Log Strike of the Option
             * @return: Returns the call price
             */

            double NaiveImplementation(
                                       boost::function<std::complex<double>(
                                        double,
                                        boost::function<std::complex<double>(std::complex<double>)>,
                                        double, 
                                        double)> fourier_transform,
                                       boost::function<std::complex<double>(std::complex<double>)> characteristic_fn,
                                       double risk_free_rate, double time_to_maturity,
                                       double log_strike) const;
            
            
        protected:
            double m_fft_N;
            double m_fft_eta;
            double m_alpha;
            boost::shared_ptr<FFT::FastFourierTransform> m_fft_transform;
        };
    }
}

#endif
