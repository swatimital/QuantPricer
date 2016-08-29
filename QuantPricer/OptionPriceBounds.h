//
//  OptionPriceBounds.h
//  QuantPricer
//
//  Created by Swati Mital on 13/06/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#ifndef QuantPricer_OptionPriceBounds_h
#define QuantPricer_OptionPriceBounds_h


#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/optional/optional_io.hpp>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>

#include "PricingEngine/RecombiningTreeOptionPricer.h"
#include "FiniteDifferenceMethods/RecombiningTrinomialTree.h"
#include "UncertainVolatility/BarenblattTrinomialTree.h"
#include "PricingEngine/BarenblattDerivativePricer.h"
#include "PricingEngine/BlackScholesOptionPricer.h"
#include "Products/Equities/OptionPayoffs.h"

using namespace QuantPricer::PricingEngine;
using namespace QuantPricer::FiniteDiffMethods;
using namespace QuantPricer::UncertainVolatility;
using namespace QuantPricer::Equities;

namespace QuantPricer
{
    namespace OptionPriceBounds
    {
        inline void ComputeCallBounds()
        {
            double sigma_max = 0.4;
            double sigma_min = 0.1;
            double sigma_mid = 0.5*(sigma_max+sigma_min);
            double T = 1.0;
            double div = 0.0;
            double rf = 0.05;
            
            
            std::ofstream bsb_file;
            bsb_file.open("/Users/swatimital/GitHub/QuantPricer/Results/CallBounds.csv");
            
            boost::shared_ptr<BarenblattTrinomialTree> bsb_tree = boost::make_shared<BarenblattTrinomialTree>(1.0, sigma_max, sigma_min, rf, div, T, 100.0);
            
            bsb_tree->InitializeTree();
            
            BarenblattDerivativePricer bsb_pricer(bsb_tree);
            
            int num_stocks = 195/5;
            std::vector<double> stock_prices;
            stock_prices.push_back(0.0);
            for (int i = 1; i <= num_stocks; i++)
            {
                stock_prices.push_back(stock_prices[i-1]+5.0);
            }
            
            double K = 90.0;
            
            bsb_file << "Stock Price, Call UB, Call LB, Call MID, BSB Call UB, BSB Call LB\n";
            
            double t = 0.0;
            
            for (auto S = 0; S < stock_prices.size(); S++)
            {
                double price_1 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K, t, T, sigma_max, rf, div);
                
                double price_2 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K, t, T, sigma_min, rf, div);
                
                
                double price_3 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K, t, T, sigma_mid, rf, div);
                
                std::vector<std::pair<double, boost::function<double(double)>>> payoffs;
                payoffs.push_back(std::make_pair(T, boost::bind(OptionPayoffs::LongVanillaCallOption, _1, stock_prices[S], K)));
                
                BarenblattDerivative bsb_prices = bsb_pricer.GetPrice(payoffs);
                
                bsb_file << stock_prices[S] << "," << price_1 << "," << price_2 << "," << price_3 << "," << std::get<0>(bsb_prices.value) << "," << std::get<1>(bsb_prices.value) << "\n";
            }
            
            bsb_file.close();
            
        }
        
        inline void ComputeStraddleBounds()
        {
            double sigma_max = 0.4;
            double sigma_min = 0.1;
            double sigma_mid = 0.5*(sigma_max+sigma_min);
            double T = 1.0;
            double div = 0.0;
            double rf = 0.05;
            
            
            std::ofstream bsb_file;
            bsb_file.open("/Users/swatimital/GitHub/QuantPricer/Results/StraddleBounds.csv");
            
            boost::shared_ptr<BarenblattTrinomialTree> bsb_tree = boost::make_shared<BarenblattTrinomialTree>(1.0, sigma_max, sigma_min, rf, div, T, 100.0);
            
            bsb_tree->InitializeTree();
            
            BarenblattDerivativePricer bsb_pricer(bsb_tree);
            
            int num_stocks = 195/5;
            std::vector<double> stock_prices;
            stock_prices.push_back(0.0);
            for (int i = 1; i <= num_stocks; i++)
            {
                stock_prices.push_back(stock_prices[i-1]+5.0);
            }
            
            double K_low = 90.0;
            double K_high = 110.0;
            
            bsb_file << "Stock Price, Call Spread UB, Call Spread LB, Call Spread MID, BSB Call Spread UB, BSB Call Spread LB\n";
            
            double t = 0.0;
            
            for (auto S = 0; S < stock_prices.size(); S++)
            {
                double price_1 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_low, T-t, sigma_max, rf, div, false);
                double price_2 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_high, T-t, sigma_max, rf, div);
                
                double price_3 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_low, T-t, sigma_mid, rf, div, false);
                double price_4 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_high, T-t, sigma_mid, rf, div);
                
                double price_5 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_low, T-t, sigma_min, rf, div, false);
                double price_6 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_high, T-t, sigma_min, rf, div);
                
                std::vector<std::pair<double, boost::function<double(double)>>> payoffs;
                payoffs.push_back(std::make_pair(T, boost::bind(OptionPayoffs::LongStraddle, _1,stock_prices[S], K_low, K_high)));
                
                BarenblattDerivative bsb_prices = bsb_pricer.GetPrice(payoffs);
                
                bsb_file << stock_prices[S] << "," << price_1 + price_2 << "," << price_3 + price_4 << "," << price_5 + price_6 << "," << std::get<0>(bsb_prices.value) << "," << std::get<1>(bsb_prices.value) << "\n";
            }
            
            bsb_file.close();
        }
        
        
        inline void ComputeButterflyBounds()
        {
            double sigma_max = 0.4;
            double sigma_min = 0.1;
            double sigma_mid = 0.5*(sigma_max+sigma_min);
            double T = 1.0;
            double div = 0.0;
            double rf = 0.05;
            
            
            std::ofstream bsb_file;
            bsb_file.open("/Users/swatimital/GitHub/QuantPricer/Results/ButterflyBounds.csv");
            
            boost::shared_ptr<BarenblattTrinomialTree> bsb_tree = boost::make_shared<BarenblattTrinomialTree>(1.0, sigma_max, sigma_min, rf, div, T, 100.0);
            
            bsb_tree->InitializeTree();
            
            BarenblattDerivativePricer bsb_pricer(bsb_tree);
            
            int num_stocks = 300/5;
            std::vector<double> stock_prices;
            stock_prices.push_back(1.0);
            for (int i = 1; i <= num_stocks; i++)
            {
                stock_prices.push_back(stock_prices[i-1]+5.0);
            }
            
            double K_low = 80.0;
            double K_mid = 100.0;
            double K_high = 120.0;
            
            bsb_file << "Stock Price, Call Spread UB, Call Spread LB, Call Spread MID, BSB Call Spread UB, BSB Call Spread LB\n";
            
            
            
            for (auto S = 0; S < stock_prices.size(); S++)
            {
                double price_1 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_low, 0, T, sigma_max, rf, div);
                double price_2 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_mid, 0, T, sigma_min, rf, div);
                double price_3 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_high, 0, T, sigma_max, rf, div);
                
                
                double price_4 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_low, 0, T, sigma_mid, rf, div);
                double price_5 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_mid, 0, T, sigma_mid, rf, div);
                double price_6 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_high, 0, T, sigma_mid, rf, div);
                
                
                double price_7 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_low, 0, T, sigma_min, rf, div);
                double price_8 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_mid, 0, T, sigma_max, rf, div);
                double price_9 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_high, 0, T, sigma_min, rf, div);
                
                std::vector<std::pair<double, boost::function<double(double)>>> payoffs;
                payoffs.push_back(std::make_pair(T, boost::bind(OptionPayoffs::LongButterflySpread, _1,stock_prices[S], K_low, K_mid, K_high)));
                
                BarenblattDerivative bsb_prices = bsb_pricer.GetPrice(payoffs);
                
                bsb_file << stock_prices[S] << "," << price_1 - 2*price_2 + price_3 << "," << price_4 - 2*price_5 + price_6 << "," << price_7 - 2*price_8 + price_9 << "," << std::get<0>(bsb_prices.value) << "," << std::get<1>(bsb_prices.value) << "\n";
            }
            
            bsb_file.close();
            
        }
        
        inline void ComputeCallSpreadBounds()
        {
            std::ofstream bsb_file;
            bsb_file.open("/Users/swatimital/GitHub/QuantPricer/Results/CallPriceBounds.csv");
            
            double sigma_max = 0.4;
            double sigma_min = 0.1;
            double sigma_mid = 0.5*(sigma_max+sigma_min);
            double T = 0.5;
            double div = 0.0;
            double rf = 0.05;
            
            boost::shared_ptr<RecombiningTrinomialTree> tree_sigma_max = boost::make_shared<RecombiningTrinomialTree>(1.0, sigma_max, rf, div, T);
            boost::shared_ptr<RecombiningTrinomialTree> tree_sigma_min = boost::make_shared<RecombiningTrinomialTree>(1.0, sigma_min, rf, div, T);
            boost::shared_ptr<RecombiningTrinomialTree> tree_sigma_mid = boost::make_shared<RecombiningTrinomialTree>(1.0, sigma_mid, rf, div, T);
            
            tree_sigma_max->InitializeTree();
            tree_sigma_min->InitializeTree();
            
            RecombiningTreeOptionPricer option_pricer_max(tree_sigma_max);
            RecombiningTreeOptionPricer option_pricer_min(tree_sigma_min);
            RecombiningTreeOptionPricer option_pricer_mid(tree_sigma_mid);
            
            boost::shared_ptr<BarenblattTrinomialTree> bsb_tree = boost::make_shared<BarenblattTrinomialTree>(1.0, sigma_max, sigma_min, rf, div, T, 100.0);
            bsb_tree->InitializeTree();
            
            BarenblattDerivativePricer bsb_pricer(bsb_tree);
            
            int num_stocks = 200/5;
            std::vector<double> stock_prices;
            stock_prices.push_back(5.0);
            for (int i = 1; i <= num_stocks; i++)
            {
                stock_prices.push_back(stock_prices[i-1]+5.0);
            }
            
            double K_low = 90.0;
            double K_high = 100.0;
            bsb_file << "Stock Price, Call Spread UB, Call Spread LB, Call Spread MID, BSB Call Spread UB, BSB Call Spread LB, Gamma\n";
            
            for (auto S = 0; S < stock_prices.size(); S++)
            {
                double price_1 = option_pricer_max.GetPrice(boost::bind(OptionPayoffs::LongVanillaCallOption, _1, stock_prices[S], K_low));
                double price_2 = option_pricer_min.GetPrice(boost::bind(OptionPayoffs::LongVanillaCallOption, _1, stock_prices[S], K_high));
                
                double price_3 = option_pricer_min.GetPrice(boost::bind(OptionPayoffs::LongVanillaCallOption, _1, stock_prices[S], K_low));
                double price_4 = option_pricer_max.GetPrice(boost::bind(OptionPayoffs::LongVanillaCallOption, _1,stock_prices[S], K_high));
                
                double price_5 = option_pricer_mid.GetPrice(boost::bind(OptionPayoffs::LongVanillaCallOption, _1,stock_prices[S], K_low));
                double price_6 = option_pricer_mid.GetPrice(boost::bind(OptionPayoffs::LongVanillaCallOption, _1,stock_prices[S], K_high));
                
                BarenblattDerivative bsb_prices = bsb_pricer.GetPrice(boost::bind(OptionPayoffs::BullCallSpread, _1,stock_prices[S], K_low, K_high));
                
                bsb_file << stock_prices[S] << "," << price_1-price_2 << "," << price_3-price_4 << "," << price_5-price_6 << "," << std::get<0>(bsb_prices.value) << "," << std::get<1>(bsb_prices.value) <<  "," << bsb_prices.greeks->gamma << "\n";
            }
            
            bsb_file.close();
        }
        
        inline void ComputeCalendarSpreadBounds()
        {
            double sigma_max = 0.4;
            double sigma_min = 0.1;
            double sigma_mid = 0.5*(sigma_max+sigma_min);
            double T_low = 0.5;
            double T_high = 1.0;
            double div = 0.0;
            double rf = 0.05;
            
            
            std::ofstream bsb_file;
            bsb_file.open("/Users/swatimital/GitHub/QuantPricer/Results/CalendarSpreadBounds.csv");
            
            boost::shared_ptr<BarenblattTrinomialTree> bsb_tree = boost::make_shared<BarenblattTrinomialTree>(1.0, sigma_max, sigma_min, rf, div, T_high, 100.0);
            
            bsb_tree->InitializeTree();
            
            BarenblattDerivativePricer bsb_pricer(bsb_tree);
            
            int num_stocks = 200/5;
            std::vector<double> stock_prices;
            stock_prices.push_back(5.0);
            for (int i = 1; i <= num_stocks; i++)
            {
                stock_prices.push_back(stock_prices[i-1]+5.0);
            }
            
            double K_low = 90.0;
            double K_high = 100.0;
            bsb_file << "Stock Price, Call Spread UB, Call Spread LB, Call Spread MID, BSB Call Spread UB, BSB Call Spread LB\n";
            
            
            
            for (auto S = 0; S < stock_prices.size(); S++)
            {
                double price_1 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_low, 0, T_high, sigma_max, rf, div);
                double price_2 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_high, 0, T_low, sigma_min, rf, div);
                
                double price_3 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_low, 0, T_high, sigma_min, rf, div);
                double price_4 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_high, 0, T_low, sigma_max, rf, div);
                
                double price_5 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_low, 0, T_high, sigma_mid, rf, div);
                double price_6 = BlackScholesOptionPricer::BSPrice(stock_prices[S], K_high, 0, T_low, sigma_mid, rf, div);
                
                std::vector<std::pair<double, boost::function<double(double)>>> payoffs;
                payoffs.push_back(std::make_pair(T_low, boost::bind(OptionPayoffs::ShortVanillaCallOption, _1,stock_prices[S], K_high)));
                payoffs.push_back(std::make_pair(T_high, boost::bind(OptionPayoffs::LongVanillaCallOption, _1,stock_prices[S], K_low)));
                
                BarenblattDerivative bsb_prices = bsb_pricer.GetPrice(payoffs);
                
                bsb_file << stock_prices[S] << "," << price_1-price_2 << "," << price_3-price_4 << "," << price_5-price_6 << "," << std::get<0>(bsb_prices.value) << "," << std::get<1>(bsb_prices.value) << "\n";
            }
            
            bsb_file.close();
        }
        
        inline void ComputeCallPricesAsFunctionOfK()
        {
            double sigma_max = 0.4;
            double sigma_min = 0.1;
            double sigma_mid = 0.5*(sigma_max+sigma_min);
            double T = 0.5;
            double div = 0.0;
            double rf = 0.1;
            
            // Get Call Prices as function of strikes
            std::ofstream call_prices_file;
            call_prices_file.open("/Users/swatimital/GitHub/QuantPricer/Results/CallPricesFixedStock.csv");
            
            std::vector<double> strike_rates;
            strike_rates.push_back(0.0);
            
            for (int i = 1; i <= 100; i++)
            {
                strike_rates.push_back(strike_rates[i-1] + 5.0);
            }
            
            call_prices_file << "Strike, Long Call Prices, Long Put Prices" << "\n";
            for(auto K = 0; K < strike_rates.size(); K++)
            {
                call_prices_file << strike_rates[K] << "," << BlackScholesOptionPricer::BSPrice(200.0, strike_rates[K], 0, T, sigma_mid, rf, div) << "," << BlackScholesOptionPricer::BSPrice(200.0, strike_rates[K], T, sigma_mid, rf, div, false) << "\n";
            }
            
            call_prices_file.close();
        }

    }
}

#endif
