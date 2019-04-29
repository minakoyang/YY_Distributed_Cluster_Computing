#!/usr/bin/env python
import sys
import statistics
import json

for line in sys.stdin:
    line = line.strip()
    genre, ratingString = line.split("\t", 1)
    ratingInfo = json.loads(ratingString)
    
    rating_stdev = statistics.stdev(ratingInfo)
    print ("%s\t%s" % (genre, rating_stdev))    