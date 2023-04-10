
import sys

filename = sys.argv[1]

with open(filename, "r") as f:
    file_contents = f.read()

prices = file_contents.split("\n")

last_price = float(prices[0])
for price in prices:
    curr_price = float(price)
    print(((curr_price/last_price) - 1) * 100)
    last_price = curr_price
