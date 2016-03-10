#!/usr/bin/python

import sys
import matplotlib.pyplot as plt
import csv
import pandas
from collections import Counter

if (len(sys.argv)) < 2:
	print "[!] Usage: sys.argv[0] filename1.csv [filename2.csv ...]"
	exit()

files = sys.argv[1:]

for file in files:
	with open('/home/user/Documents/github/ctattack/' + file, 'rb') as f:
		reader = csv.reader(f)
		data = list(reader)

	lst = []

	for item in data:
		lst.append(item[0])

	data = map(int, lst)

	binwidth = 10

	plt.hist(data, bins=range(min(data), max(data) + binwidth, binwidth), alpha=0.5, label=file)


plt.legend(loc='upper right')






#counts = Counter(data[0])

#df = pandas.DataFrame.from_dict(counts, orient='index')

#df.plot(kind='bar')

plt.show()

#print data


