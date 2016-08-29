//
//  BaseFFTMethod.cpp
//  QuantPricer
//
//  Created by Swati Mital on 20/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include <stdio.h>
#include <utility>
#include "boost/bind.hpp"
#include "BaseFFTMethod.h"
#include "Maths.h"

namespace QuantPricer
{
    namespace FFTMethods
    {
        BaseFFTMethod::BaseFFTMethod() :
            m_fft_eta(0.25),
            m_fft_N(12),
            m_alpha(1.5),
            m_fft_transform(new FFT::FastFourierTransform(m_fft_N))
        {
            m_fft_transform->Initialize();
        }
        
        std::vector<std::pair<double, double>> BaseFFTMethod::FFTEuropeanOptionPrices(
                    boost::function<std::complex<double>(std::complex<double>)> characteristic_fn,
                    double rf_rate,
                    double dividend,
                    double T,
                    bool at_the_money,
                    bool call) const
        {
            // Perform Inverse DFT by first computing the input vector of Fourier Transform
            // of the modified call price
            std::vector<std::complex<double>> fft_out = at_the_money ?
            FFTOutputVector(boost::bind(boost::mem_fn(&BaseFFTMethod::PsiFunction), this, _1,_2, _3, _4), characteristic_fn, rf_rate, T) : 
            FFTOutputVector(boost::bind(boost::mem_fn(&BaseFFTMethod::GammaFunction), this, _1, _2, _3, _4), characteristic_fn, rf_rate,
                                T);
            
            // Length of the FFT Array
            double N = m_fft_transform->GetFFTArrayLength();
            // Lambda controls the spacing for FFT
            double lambda = (2*M_PI)/(N*m_fft_eta);
            // Log Strike Levels range from (-b, b)
            double b = 0.5*N*lambda;
            // Starting value of Log Strike
            double ku = -b;
            
            // Output vector of option prices
            std::vector<std::pair<double, double>> option_prices(N);
            for (int i = 0; i < N; i++)
            {
                // Compute the dampener depending on whether we want atm option price or otm option price
                double dampener = at_the_money ? exp(-m_alpha*ku) : 1.0/sinh(m_alpha*ku);
                double call_price = dampener*fft_out[i].real()/M_PI; 
                option_prices[i] = call ? std::make_pair(exp(ku), call_price) : 
                                          std::make_pair(exp(ku), 
                                                         (call_price - 1.0 + exp(ku)*exp(-rf_rate*T)));
                ku += lambda;
            }
            
            return option_prices;
        }
        
        std::vector<std::complex<double>>BaseFFTMethod::FFTOutputVector(
                             boost::function<std::complex<double>(
                                  double, 
                                  boost::function<std::complex<double>(std::complex<double>)>,
                                  double, double)> fourier_transform,
                             boost::function<std::complex<double>(std::complex<double>)> characteristic_fn,
                             double rf_rate, double T) const
        {
            return (m_fft_transform->Execute(FFTInputVector(fourier_transform, characteristic_fn, rf_rate, T)));
        }
        
               
        
        std::vector<std::complex<double>> BaseFFTMethod::FFTInputVector(
                             boost::function<std::complex<double>(
                                    double,
                                    boost::function<std::complex<double>(std::complex<double>)>,
                                    double, 
                                    double)> fourier_transform,
                                    boost::function<std::complex<double>(std::complex<double>)> characteristic_fn,
                                    double rf_rate, double T
                                ) const
        {
            int N = m_fft_transform->GetFFTArrayLength();
            std::vector<std::complex<double>> in_vector(N);
            
            double lambda = (2*M_PI)/(N*m_fft_eta);
            double b = 0.5*N*lambda;
            double nu;
            for (int j = 0; j < N; j++)
            {
                nu = j*m_fft_eta;
                std::complex<double> psi = fourier_transform(nu, characteristic_fn, rf_rate, T);
                double simpson_weight = (m_fft_eta/3.0)*(3.0 + pow(-1.0,j+1)-QuantPricer::common::Maths::kronecker_delta(j));
                in_vector[j] = exp(std::complex<double>(0,1)*nu*b)*simpson_weight*psi;
            }
            
            return in_vector;
        }
        
        std::complex<double> BaseFFTMethod::PsiFunction(double u,
                                                        boost::function<std::complex<double>(std::complex<double>)> characteristic_fn,
                                                        double rf_rate,
                                                        double T) const
        {
            return (
                    exp(-rf_rate*T)*
                    characteristic_fn(std::complex<double>(u, -(m_alpha+1))) /
                    (m_alpha*m_alpha + m_alpha - u*u + (2*m_alpha+1)*u*
                     std::complex<double>(0,1)));
            
        }
        
        std::complex<double> BaseFFTMethod::GammaFunction(double u,
                                                          boost::function<std::complex<double>(std::complex<double>)> characteristic_fn,
                                                          double rf_rate,
                                                          double T) const
        {   
            return (
                    std::complex<double>(0.5,0)*
                    (ZetaFunction(std::complex<double>(u,-m_alpha), characteristic_fn, rf_rate, T) - 
                     ZetaFunction(std::complex<double>(u,m_alpha), characteristic_fn, rf_rate, T)));
            
        }

        
        std::complex<double> BaseFFTMethod::ZetaFunction(std::complex<double> u,
                                                         boost::function<std::complex<double>(std::complex<double>)> characteristic_fn,
                                                         double rf_rate,
                                                         double T) const
        { 
            auto img = std::complex<double>(0,1);
            auto real = std::complex<double>(1,0);
            
            auto aux1 = real/(real+img*u);
            auto aux2 = std::complex<double>(exp(rf_rate*T),0)/(img*u);
            auto aux3 = characteristic_fn(u-img)/(u*u-img*u);
            return (exp(-rf_rate*T)*(aux1-aux2-aux3));
        }
        
                
    }
}