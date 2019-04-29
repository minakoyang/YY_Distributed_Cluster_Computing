#!/usr/bin/env python

import sys
import csv
import json

mostRatingUser = None
mostRatingCount = 0
mostRatingInfo = None

for line in sys.stdin:
    line = line.strip()
    user, genreString = line.split("\t", 1)
    genreInfo = json.loads(genreString)

    if not mostRatingUser or len(genreInfo) > mostRatingCount:
        mostRatingUser = user
        mostRatingCount = len(genreInfo)
        mostRatingInfo = genreInfo
        
# print(mostRatingCount)

genreDict = {}
for genreList in mostRatingInfo:
    for genre in genreList:    
        if genre not in genreDict:
            genreDict[genre] = 0
        genreDict[genre] += 1 

mostRatedCount = 0
mostRatedGenre = None

for genre, count in genreDict.items():
    if count > mostRatedCount:
        mostRatedCount = count
        mostRatedGenre = genre
    
print("%s -- Total Rating Counts: %d -- Most Rated Genre: %s - %d" % (mostRatingUser, mostRatingCount, mostRatedGenre, mostRatedCount))