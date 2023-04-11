#include <algorithm>
#include <iostream>
#include <future>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

float run_simulation(std::vector<float> prices, float buy_threshold_percent, float sell_threshold_percent, bool verbose = false) {
	std::vector<std::tuple<float, float, int>> bspairs; //buy threshold, sell threshold, quantity
	
	float buy_threshold = buy_threshold_percent / 100;
	float sell_threshold = sell_threshold_percent / 100;
	
	float initial_cash = 100000;
	float cash = initial_cash;

	int max_bspairs = 100;

	//Variables needed in the loop
	float last_price = prices.at(0); //Start last price at first price
	float percent_change;
	float current_price;
	std::tuple<float, float, int> new_tuple;
	int num_stocks_per_buy = 1;

	//Variables needed for bspairs loop (and main loop) inside main loop
	float buy_price;
	float sell_price;
	int quantity;

	for (int i = 0; i < prices.size(); i++) { //Iterate through prices
		current_price = prices.at(i);
	
		if (verbose) 
			std::cout << "Current Price: " << current_price << " ";
		

		percent_change = (current_price / last_price) - 1;

		if (verbose)
			std::cout << " Percent change: " << percent_change << " ";

		if (percent_change <= buy_threshold && bspairs.size() < max_bspairs) {
			//Then we want to buy
			buy_price = current_price;
			sell_price = current_price * (sell_threshold+1);
			quantity = num_stocks_per_buy;
			
			if (cash > buy_price * quantity) {
				//Now we know we CAN buy, we have enough money
				if (verbose)
					std::cout << " ->Buy";
				new_tuple = {buy_price, sell_price, quantity};
				cash -= quantity * buy_price;

				bspairs.push_back(new_tuple);
			}
		}

		//Now check to see if any bspairs are ripe
		for (int bsp = 0; bsp < bspairs.size(); bsp++) {
			buy_price = std::get<0>(bspairs.at(bsp));
			sell_price = std::get<1>(bspairs.at(bsp));
			quantity = std::get<2>(bspairs.at(bsp));

			if (sell_price >= current_price) {
				//Then we want to sell
				if (verbose)
					std::cout << " ->Sell";
				cash += current_price * quantity;
				bspairs.erase(bspairs.begin() + bsp);
			}
		}
		if (verbose)
			std::cout << std::endl;
		//moving last price
		last_price = current_price;
	}

	//Determining money held in stock
	float stock_money = 0;
	for (int bsp = 0; bsp < bspairs.size(); bsp++) {
		quantity = std::get<2>(bspairs.at(bsp));
		stock_money += quantity * last_price;
	}

	float final_percent_change = (((cash + stock_money) / initial_cash) - 1) * 100;

	if (false) {
		std::cout << "Cash remaining on hand: " << cash << std::endl;
		std::cout << "Value of stocks: " << stock_money << std::endl; 
		std::cout << "Combined value: " << cash + stock_money << std::endl;
		std::cout << "Percent change: " << final_percent_change << "%" << std::endl; 

		std::cout << "Buy threshold setting: " << buy_threshold_percent << std::endl;
		std::cout << "Sell threshold setting: " << sell_threshold_percent << std::endl << std::endl;
	}

	return final_percent_change;
}


int main(int argc, char *argv[]) {
	std::ifstream csv_file(argv[1]);
	std::string line;
	
	std::vector<float> price_history;

	float line_float;

	//Reading the input file
	while (getline(csv_file, line)) {
		line_float = std::stof(line);	
		price_history.push_back(line_float);
	}


	if (true) {
		//Finding ideal parameters
		//Vector of tuples <return_percentage, buy_threshold_percent, sell_threshold_percent>
		std::vector<std::tuple<float, float, float>> parameters;
		float current_return;

		float r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12;

		int iterations = 0;

		//Yes this is kind of an ugly not amazing way of doing multithreading, I'll probably fix it later
		for (float buy_threshold_percent = -0.001; buy_threshold_percent > -2; buy_threshold_percent -= 0.0012) {
			for (float sell_threshold_percent = 0.001; sell_threshold_percent < 3; sell_threshold_percent += 0.0012) {
				//making new threads, each one offset by the + so obviously they're doing different things		
				auto t1 = std::async(run_simulation, price_history, buy_threshold_percent, sell_threshold_percent, false);
				auto t2 = std::async(run_simulation, price_history, buy_threshold_percent-0.0001, sell_threshold_percent+0.0001, false);
				auto t3 = std::async(run_simulation, price_history, buy_threshold_percent-0.0002, sell_threshold_percent+0.0002, false);
				auto t4 = std::async(run_simulation, price_history, buy_threshold_percent-0.0003, sell_threshold_percent+0.0003, false);
				auto t5 = std::async(run_simulation, price_history, buy_threshold_percent-0.0004, sell_threshold_percent+0.0004, false);
				auto t6 = std::async(run_simulation, price_history, buy_threshold_percent-0.0005, sell_threshold_percent+0.0005, false);
				auto t7 = std::async(run_simulation, price_history, buy_threshold_percent-0.0006, sell_threshold_percent+0.0006, false);
				auto t8 = std::async(run_simulation, price_history, buy_threshold_percent-0.0007, sell_threshold_percent+0.0007, false);
				auto t9 = std::async(run_simulation, price_history, buy_threshold_percent-0.0008, sell_threshold_percent+0.0008, false);
				auto t10 = std::async(run_simulation, price_history, buy_threshold_percent-0.0009, sell_threshold_percent+0.0009, false);
				auto t11 = std::async(run_simulation, price_history, buy_threshold_percent-0.0010, sell_threshold_percent+0.0010, false);
				auto t12 = std::async(run_simulation, price_history, buy_threshold_percent-0.0011, sell_threshold_percent+0.0011, false);
		
				//Getting the results from each thread
				r1 = t1.get();
				r2 = t2.get();
				r3 = t3.get();
				r4 = t4.get();
				r5 = t5.get();
				r6 = t6.get();
				r7 = t7.get();
				r8 = t8.get();
				r9 = t9.get();
				r10 = t10.get();
				r11 = t11.get();

				//std::cout << "Return: " << r1 << " buy threshold: " << buy_threshold_percent << " sell threshold: " << sell_threshold_percent << std::endl;

				//Adding the results from each thread to our parameter vector
				parameters.push_back({r1, buy_threshold_percent, sell_threshold_percent});
				parameters.push_back({r2, buy_threshold_percent-0.0001, sell_threshold_percent+0.0001});
				parameters.push_back({r3, buy_threshold_percent-0.0002, sell_threshold_percent+0.0002});
				parameters.push_back({r4, buy_threshold_percent-0.0003, sell_threshold_percent+0.0003});
				parameters.push_back({r5, buy_threshold_percent-0.0004, sell_threshold_percent+0.0004});
				parameters.push_back({r6, buy_threshold_percent-0.0005, sell_threshold_percent+0.0005});
				parameters.push_back({r7, buy_threshold_percent-0.0006, sell_threshold_percent+0.0006});
				parameters.push_back({r8, buy_threshold_percent-0.0007, sell_threshold_percent+0.0007});
				parameters.push_back({r9, buy_threshold_percent-0.0008, sell_threshold_percent+0.0008});
				parameters.push_back({r10, buy_threshold_percent-0.0009, sell_threshold_percent+0.0009});
				parameters.push_back({r11, buy_threshold_percent-0.0010, sell_threshold_percent+0.0010});
				parameters.push_back({r12, buy_threshold_percent-0.0011, sell_threshold_percent+0.0011});

				iterations++;

				if (iterations % 1000 == 0) { //Printing the current iteration if its a multiple so we aren't bottlenecked by terminal
					std::cout << "Iteration: " << iterations << std::endl;
				}
			}
		}

		//sorting the parameters
		std::sort(parameters.begin(), parameters.end());

		//Printing the sorted parameter vector
		for (int pi = 0; pi < parameters.size(); pi++) {
			std::cout << "Return: " << std::get<0>(parameters.at(pi)) << " Buy thresh: " << std::get<1>(parameters.at(pi)) << " Sell thresh: " << std::get<2>(parameters.at(pi)) << std::endl;
		}
	}

	//run_simulation(price_history, -0.001, 0.0350, true);

	return 0;
}





