#!/usr/bin/env python
import sys
import statistics
import json

print("Genre\t\tMean\t\tMedian\t\tStandard deviation")
for line in sys.stdin:
    line = line.strip()
    genre, ratingString = line.split("\t", 1)
    ratings = json.loads(ratingString)
    
    rating_mean = sum(ratings) / len(ratings)
    rating_median = statistics.median(ratings)
    rating_stdev = statistics.stdev(ratings)
    print ("%s\t\t%.4f\t\t%s\t\t%.4f" % (genre, rating_mean, rating_median, rating_stdev))  
    