#!/usr/local/bin/python

import yfinance as yf
import json
import sys

stock_name = sys.argv[1]

stock = yf.Ticker(stock_name)

hist = stock.history(period="59d", interval="5m")

listized = hist["Close"].values.tolist()

with open(stock_name+"-59d-5m.json", "w") as f:
    json.dump(listized, f, ensure_ascii=False)


