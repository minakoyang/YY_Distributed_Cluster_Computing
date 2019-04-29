#!/usr/bin/env python

import sys
import csv
import json

movieFile = "movies.csv"
movieList = {}

with open(movieFile, mode = 'r') as infile:
    reader = csv.reader(infile)
    for row in reader:
        movieList[row[0]] = {}
        movieList[row[0]]["title"] = row[1]
        movieList[row[0]]["genre"] = row[2].strip().split("|")

userDict = {}
        
for oneMovie in sys.stdin:
    oneMovie = oneMovie.strip()
    ratingInfo = oneMovie.split(",")
    try:
        user = int(ratingInfo[0])
        movieTitle = movieList[ratingInfo[1]]["title"]
        movieGenre = movieList[ratingInfo[1]]["genre"]
        rating = float(ratingInfo[2])
        
        if user in userDict:
            userDict[user]["count"] += 1            
        else:
            userDict[user] = {}
            userDict[user]["count"] = 1
            userDict[user]["genre"] = {}
        for genre in movieGenre:
            if genre not in userDict[user]["genre"]:
                userDict[user]["genre"][genre] = 0
            userDict[user]["genre"][genre] += 1
    except ValueError:
        continue
        
for user in userDict:
    print("%s\t%s" % (user, json.dumps(userDict[user])))