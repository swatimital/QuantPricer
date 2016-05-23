//
//  main.cpp
//  QuantPricer
//
//  Created by Swati Mital on 06/03/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>
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

void ComputeCallSpreadBounds()
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
    bsb_file << "Stock Price, Call Spread UB, Call Spread LB, Call Spread MID, BSB Call Spread UB, BSB Call Spread LB\n";
    
    for (auto S = 0; S < stock_prices.size(); S++)
    {
        double price_1 = option_pricer_max.GetPrice(boost::bind(OptionPayoffs::VanillaCallOption, _1, stock_prices[S], K_low));
        double price_2 = option_pricer_min.GetPrice(boost::bind(OptionPayoffs::VanillaCallOption, _1, stock_prices[S], K_high));
        
        double price_3 = option_pricer_min.GetPrice(boost::bind(OptionPayoffs::VanillaCallOption, _1, stock_prices[S], K_low));
        double price_4 = option_pricer_max.GetPrice(boost::bind(OptionPayoffs::VanillaCallOption, _1,stock_prices[S], K_high));
        
        double price_5 = option_pricer_mid.GetPrice(boost::bind(OptionPayoffs::VanillaCallOption, _1,stock_prices[S], K_low));
        double price_6 = option_pricer_mid.GetPrice(boost::bind(OptionPayoffs::VanillaCallOption, _1,stock_prices[S], K_high));
        
        BarenblattDerivative bsb_prices = bsb_pricer.GetPrice(boost::bind(OptionPayoffs::BullCallSpread, _1,stock_prices[S], K_low, K_high));
       
        bsb_file << stock_prices[S] << "," << price_1-price_2 << "," << price_3-price_4 << "," << price_5-price_6 << "," << std::get<0>(bsb_prices.value) << "," << std::get<1>(bsb_prices.value) <<  "\n";
    }
    
    bsb_file.close();
}

void ComputeCalendarSpreadBounds()
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
    
    boost::shared_ptr<BarenblattTrinomialTree> bsb_tree_T_low = boost::make_shared<BarenblattTrinomialTree>(1.0, sigma_max, sigma_min, rf, div, T_low, 100.0);
    boost::shared_ptr<BarenblattTrinomialTree> bsb_tree_T_high = boost::make_shared<BarenblattTrinomialTree>(1.0, sigma_max, sigma_min, rf, div, T_high, 100.0);
    
    bsb_tree_T_low->InitializeTree();
    bsb_tree_T_high->InitializeTree();
    
    BarenblattDerivativePricer bsb_pricer_T_high(bsb_tree_T_high);
    BarenblattDerivativePricer bsb_pricer_T_low(bsb_tree_T_low);
    
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
        
        //BarenblattDerivative bsb_prices = bsb_pricer.GetPrice();

        
        bsb_file << stock_prices[S] << "," << price_1-price_2 << "," << price_3-price_4 << "," << price_5-price_6 << "\n";
    }
    
    bsb_file.close();
}

void ComputeCallPricesAsFunctionOfK()
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
    
    call_prices_file << "Strike, Prices" << "\n";
    for(auto K = 0; K < strike_rates.size(); K++)
    {
        call_prices_file << strike_rates[K] << "," << BlackScholesOptionPricer::BSPrice(200.0, strike_rates[K], 0, T, sigma_mid, rf, div) << "\n";
    }
    
    call_prices_file.close();
}


int main(int argc, const char * argv[])
{
    ComputeCallSpreadBounds();
    
    ComputeCalendarSpreadBounds();
    
    ComputeCallPricesAsFunctionOfK();
    
}
