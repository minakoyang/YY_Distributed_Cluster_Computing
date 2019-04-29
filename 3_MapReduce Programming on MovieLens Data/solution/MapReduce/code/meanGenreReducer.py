#!/usr/bin/env python
import sys

current_genre = None
current_rating_sum = 0
current_rating_count = 0
print("Genre\t\tMean")

for line in sys.stdin:
    line = line.strip()
    genre, rating = line.split("\t", 1)

    if current_genre == genre:
        try:
            current_rating_sum += float(rating)
            current_rating_count += 1
        except ValueError:
            continue    
    else:
        if current_genre:
            rating_average = current_rating_sum / current_rating_count
            print ("%s\t\t%.4f" % (current_genre, rating_average))    
        current_genre = genre
        try:
            current_rating_sum = float(rating)
            current_rating_count = 1
        except ValueError:
            continue

if current_genre == genre:
    rating_average = current_rating_sum / current_rating_count
    print ("%s\t\t%.4f" % (current_genre, rating_average))  