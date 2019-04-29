#!/usr/bin/env python
import sys
import statistics

current_genre = None
ratings = []
print("Genre\t\tStandard Deviation Sample")

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
            rating_stdev = statistics.stdev(ratings)
            print ("%s\t%s" % (current_genre, rating_stdev))    
        current_genre = genre
        try:
            ratings = [float(rating)]
        except ValueError:
            continue

if current_genre == genre:
        rating_stdev = statistics.stdev(ratings)
        print ("%s\t%s" % (current_genre, rating_stdev))