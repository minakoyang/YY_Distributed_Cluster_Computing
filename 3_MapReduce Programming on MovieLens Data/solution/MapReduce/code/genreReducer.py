#!/usr/bin/env python

import sys
import statistics
import csv
import json

current_genre = None
ratings = []
print("Genre\t\tMean\t\tMedian\t\tStandard Deviation Sample")

for line in sys.stdin:
    line = line.strip()
    genre, rating = line.split("\t", 1)

    if current_genre == genre:
        try:
            ratings.append(float(rating))
        except ValueError:
            continue    
    else:
        if current_genre:
            rating_mean = sum(ratings) / len(ratings)
            rating_median = statistics.median(ratings)
            rating_stdev = statistics.stdev(ratings) # Sample standard deviation of data.
            print ("%s\t\t%.4f\t\t%s\t\t%.4f" % (current_genre, rating_mean, rating_median, rating_stdev))
        current_genre = genre
        try:
            ratings = [float(rating)]
        except ValueError:
            continue

if current_genre == genre:
    rating_mean = sum(ratings) / len(ratings)
    rating_median = statistics.median(ratings)
    rating_stdev = statistics.stdev(ratings) # Sample standard deviation of data.
    print ("%s\t\t%.4f\t\t%s\t\t%.4f" % (current_genre, rating_mean, rating_median, rating_stdev))
        
    