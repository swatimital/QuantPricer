//
//  FastFourierTransform.h
//  QuantPricer
//
//  Created by Swati Mital on 13/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef __QuantPricer__FastFourierTransform__
#define __QuantPricer__FastFourierTransform__

#include <stdio.h>
#include <fftw3.h>
#include <memory>
#include <complex>
#include <vector>


namespace QuantPricer
{
    namespace FFT
    {
        class FastFourierTransform
        {
        public:
            FastFourierTransform(int idx);
            virtual ~FastFourierTransform();
            
            void Initialize();
            std::vector<std::complex<double>> Execute(const std::vector<std::complex<double>>& in);
            int GetFFTArrayLength() const {return m_indices;}
            
        private:
            //std::shared_ptr<fftw_complex> m_in;
            //std::shared_ptr<fftw_complex> m_out;
            fftw_complex* m_in;
            fftw_complex* m_out;
            fftw_plan m_fftw_plan;
            int m_indices;
        };
    }
}

#endif /* defined(__QuantPricer__FastFourierTransform__) */
