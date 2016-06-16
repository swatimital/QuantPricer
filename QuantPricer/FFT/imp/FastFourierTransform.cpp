//
//  FastFourierTransform.cpp
//  QuantPricer
//
//  Created by Swati Mital on 13/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//


#include "FastFourierTransform.h"
#include <math.h>
#include "fftw3.h"
#include <boost/make_shared.hpp>

namespace QuantPricer
{
    namespace FFT
    {
        FastFourierTransform::FastFourierTransform(int idx) :
            m_indices(int(pow(double(2),int(idx))))
        {
        }
        
        FastFourierTransform::~FastFourierTransform()
        {
            fftw_destroy_plan(m_fftw_plan);
            fftw_free(m_in);
            fftw_free(m_out);
        }
        
        std::vector<std::complex<double>> FastFourierTransform::Execute(const
                                    std::vector<std::complex<double>>& inarr)
        {
            //Populate input vector for FFT operation
            for (int i=0;i<m_indices;i++)
            {
                m_in[i][0]=inarr[i].real();
                m_in[i][1]=inarr[i].imag();
            }
            
            fftw_execute(m_fftw_plan);
            std::vector<std::complex<double>> output(m_indices);
            
            //Populate output vector
            for (int i=0;i<m_indices;i++)
            {
                output[i] = std::complex<double>(m_out[i][0], m_out[i][1]);
            }
            
            return output;
        }
        
        void FastFourierTransform::Initialize()
        {
            //Allocate memory for FFT variables
            m_in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * m_indices);
            m_out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * m_indices);
            m_fftw_plan = fftw_plan_dft_1d(m_indices, m_in, m_out, FFTW_FORWARD, FFTW_ESTIMATE);
            
        }
        
    }
}