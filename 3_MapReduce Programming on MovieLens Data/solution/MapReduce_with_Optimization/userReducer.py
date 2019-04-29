#!/usr/bin/env python

import sys
import csv
import json

mostRatingUser = None
mostRatingCount = 0
genreDict = None

for line in sys.stdin:
    line = line.strip()
    user, genreString = line.split("\t", 1)
    genreInfo = json.loads(genreString)

    if not mostRatingUser or genreInfo["count"] > mostRatingCount:
        mostRatingUser = user
        mostRatingCount = genreInfo["count"] 
        genreDict = genreInfo["genre"]
        
# print(genreDict)

mostRatedCount = 0
mostRatedGenre = None

for genre, count in genreDict.items():
    if count > mostRatedCount:
        mostRatedCount = count
        mostRatedGenre = genre
    
print("%s -- Total Rating Counts: %d -- Most Rated Genre: %s - %d" % (mostRatingUser, mostRatingCount, mostRatedGenre, mostRatedCount))