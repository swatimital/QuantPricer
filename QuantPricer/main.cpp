//
//  main.cpp
//  QuantPricer
//
//  Created by Swati Mital on 06/03/16.
//  Copyright (c) 2016 Swati Mital. All rights reserved.
//

#include <iostream>
#include <boost/function.hpp>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include "VanillaOptionPricer.h"
#include "RecombiningTrinomialTree.h"
#include "BarenblattTrinomialTree.h"
#include "BarenblattDerivativePricer.h"

int main(int argc, const char * argv[])
{
    std::ofstream myfile;
    myfile.open("CallPriceBounds.csv");
    
    double sigma_max = 0.4;
    double sigma_min = 0.1;
    double sigma_mid = 0.5*(sigma_max + sigma_min);
    double T = 0.5;
    double div = 0.0;
    double rf = 0.1;
    
    boost::shared_ptr<RecombiningTrinomialTree> tree_sigma_max = boost::make_shared<RecombiningTrinomialTree>(1.0, sigma_max, rf, div, T);
    boost::shared_ptr<RecombiningTrinomialTree> tree_sigma_min = boost::make_shared<RecombiningTrinomialTree>(1.0, sigma_min, rf, div, T);
    boost::shared_ptr<RecombiningTrinomialTree> tree_sigma_mid = boost::make_shared<RecombiningTrinomialTree>(1.0, sigma_mid, rf, div, T);
    
    tree_sigma_max->InitializeTree();
    tree_sigma_min->InitializeTree();
    
    VanillaOptionPricer option_pricer_max(tree_sigma_max);
    VanillaOptionPricer option_pricer_min(tree_sigma_min);
    VanillaOptionPricer option_pricer_mid(tree_sigma_mid);
    
    int num_stocks = 200/5;
    std::vector<double> stock_prices;
    stock_prices.push_back(0.0001);
    for (int i = 1; i <= num_stocks; i++)
    {
        stock_prices.push_back(stock_prices[i-1]+5.0);
    }
    
    double K_low = 90.0;
    double K_high = 100.0;
    myfile << "Stock Price, Call Spread UB, Call Spread LB, Call Spread MID\n";
    
    for (auto S = 0; S < stock_prices.size(); S++)
    {
        double price_1 = option_pricer_max.GetPrice(stock_prices[S], K_low, OptionType::Call);
        double price_2 = option_pricer_min.GetPrice(stock_prices[S], K_high, OptionType::Call);
        
        double price_3 = option_pricer_min.GetPrice(stock_prices[S], K_low, OptionType::Call);
        double price_4 = option_pricer_max.GetPrice(stock_prices[S], K_high, OptionType::Call);
        
        double price_5 = option_pricer_mid.GetPrice(stock_prices[S], K_low, OptionType::Call);
        double price_6 = option_pricer_mid.GetPrice(stock_prices[S], K_high, OptionType::Call);
        
        myfile << stock_prices[S] << "," << price_1-price_2 << "," << price_3-price_4 << "," << price_5-price_6 << ",\n";
    }
    
    
    boost::shared_ptr<BarenblattTrinomialTree> bsb_tree = boost::make_shared<BarenblattTrinomialTree>(1.0, sigma_max, sigma_min, rf, div, T, 100.0);
    bsb_tree->InitializeTree();
    
    BarenblattDerivativePricer bsb_pricer(bsb_tree);
    
    //Needs testing
    bsb_pricer.GetPrice(stock_prices[1], K_low, OptionType::Call);
    
    myfile.close();
}
