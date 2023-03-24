#!/usr/local/bin/python
import sys
import json
import numpy as np

def run_simulation(initial_funds=100000, max_bspairs=5, buy_threshold_percent=-0.00173611111, sell_threshold_percent=0.00694444444, num_stocks_per_buy=1, do_print=True):

    bspairs = [] #tuple (buy_price, to_sell_price, quantity)

    #Tweakable parts
    #initial_funds = 100000
    #max_bspairs = 5
    #buy_threshold_percent = -0.001736111111111111 
    #sell_threshold_percent = 0.006944444444444444
    #num_stocks_per_buy = 1 
    #########

    buy_threshold = buy_threshold_percent / 100
    sell_threshold = sell_threshold_percent / 100
    test_period_days = 59
    bspair_allowance_reached_counter = 0
    funds = initial_funds
    sell_counter = 0
    buy_counter = 0

    last_price = prices[0]
    for price in prices:
        change = 1-price/last_price #change from last price to current price

        if change < buy_threshold:

            if len(bspairs) < max_bspairs: #we only want to buy if we haven't gone over our bspair allowance
                if funds > price*num_stocks_per_buy:
                    ######BUY#####
                    buy_counter += 1

                    funds -= price*num_stocks_per_buy

                    pair = (price, price*(sell_threshold+1), num_stocks_per_buy)
                    
                    bspairs.append(pair)
               
            else:
                bspair_allowance_reached_counter += 1

        for pair in bspairs:
            buy_price = pair[0]
            sell_price = pair[1]
            quantity = pair[2]

            if sell_price < price:
                #####SELL#######
                sell_counter += 1

                funds += sell_price*quantity

                bspairs.remove(pair)


        last_price = price


    stock_money = 0 #finding money stored in stocks, using the last price of the stock
    for pair in bspairs:
        quantity = pair[2]
        stock_money += quantity*last_price

    total_asset_worth = stock_money + funds
    return_percentage = ((total_asset_worth/initial_funds) - 1)*100
    predicted_annual_return = (365 / test_period_days) * return_percentage

    if do_print:
        print()
        print("Debug")
        print("BSpair allowanced reached counter:", bspair_allowance_reached_counter) 
        print("Buy counter:", buy_counter)
        print("Sell counter:", sell_counter)
        print()
        print("Stock money:", stock_money)
        print("Cash:", funds)
        print("Total:", total_asset_worth)
        print("\nReturn stats")
        print("Initial Investment:", initial_funds)
        print("Total return percentage:", return_percentage)
        print("Predicted annual return percentage:", predicted_annual_return)
        print()

    return predicted_annual_return

def average(input_list):
    accumulate = 0
    for num in input_list:
        accumulate += num
    return accumulate / len(input_list)


if __name__ == "__main__":

    train = True

    stock_file_name = sys.argv[1]

    with open(stock_file_name) as f:
        prices = json.load(f)
  
    print("Average stock price:", average(prices))

    if not train:
        print("Not training")
        run_simulation(initial_funds=100000, max_bspairs=5, buy_threshold_percent=-0.00173611111, sell_threshold_percent=0.00694444444, num_stocks_per_buy=1, do_print=True)
    
    else:
        print("Training")
        parameter_run_counter = 0

        return_list = {}
        for _max_bspairs in range(1, 100):
            for _buy_threshold_percent in np.arange(-0.002, -0.001, 0.00001):
                for _sell_threshold_percent in np.arange(-0.002, -0.001, 0.00001):
                    for _num_stocks_per_buy in range(1, 10):
                        test_return_percentage = run_simulation(initial_funds=100000, max_bspairs=_max_bspairs, buy_threshold_percent=_buy_threshold_percent, sell_threshold_percent=_sell_threshold_percent, num_stocks_per_buy=_num_stocks_per_buy, do_print=False)
                        
                        return_list[test_return_percentage] = [_max_bspairs, _buy_threshold_percent, _sell_threshold_percent, _num_stocks_per_buy]
                        parameter_run_counter += 1
                        if parameter_run_counter % 100 == 0:
                            print(parameter_run_counter)

        sorted_dict_order = sorted(return_list)

        print("Finished training")
        for key in sorted_dict_order:
            entry = return_list[key]
            best_max_bspairs = entry[0]
            best_buy_threshold = entry[1]
            best_sell_threshold = entry[2]
            best_num_stocks = entry[3]
            print("Return:", key, "bspairs:", best_max_bspairs, "buy_threshold:", best_buy_threshold, "sell_threshold:", best_sell_threshold, "num_stocks:", best_num_stocks)
   
        print("Combination list length:", len(return_list))

    """
    price_changes = []
    
    last_price = prices[0]
    for price in prices:
        change = 1-price/last_price
        
        price_changes.append(change)
        last_price = price
    
    price_changes.sort()

    print(price_changes)
    """



