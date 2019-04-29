#!/usr/bin/env python
import sys
import json

for line in sys.stdin:
    line = line.strip()
    genre, ratingString = line.split("\t", 1)
    ratingInfo = json.loads(ratingString)
    
    rating_sum = ratingInfo["total_rating"]
    rating_count = ratingInfo["total_count"]

    rating_average = rating_sum / rating_count
    print ("%s\t%s" % (genre, rating_average))