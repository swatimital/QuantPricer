//
//  ImpliedVolatility.h
//  QuantPricer
//
//  Created by Swati Mital on 15/08/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef QuantPricer_ImpliedVolatility_h
#define QuantPricer_ImpliedVolatility_h


#include <boost/date_time/gregorian/gregorian.hpp>
#include <vector>

namespace QuantPricer
{
    namespace Calibration
    {
        class ImpliedVolatility
        {
        public:
            ImpliedVolatility();
            
            bool PopulateData(std::vector<double> strikes, 
                              std::vector<double> maturities,
                              std::vector<double> vol_data);
            
            bool PopulateData(boost::gregorian::date date, 
                              std::istream& file);
            
            
            std::vector<double> GetMaturities() const { return m_maturities; }
            std::vector<double> GetStrikes() const { return m_strikes; }
            std::vector<double> GetVolData() const { return m_vol_data; }
            
            virtual ~ImpliedVolatility();
            
            
        private:
            std::vector<double> m_strikes;
            std::vector<double> m_maturities;
            std::vector<double> m_vol_data;
            std::vector<double> m_price_data;
        };
    }
}

#endif
