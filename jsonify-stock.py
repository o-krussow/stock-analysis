#!/usr/local/bin/python

import yfinance as yf
import json
import sys

stock_name = sys.argv[1]

stock = yf.Ticker(stock_name)

hist = stock.history(period="3mo", interval="60m")

listized = hist["Close"].values.tolist()

print("Number of points:", len(listized))

with open(stock_name+"-59d-5m.json", "w") as f:
    json.dump(listized, f, ensure_ascii=False)


