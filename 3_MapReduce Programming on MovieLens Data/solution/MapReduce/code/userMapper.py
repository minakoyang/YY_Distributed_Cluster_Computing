#!/usr/bin/env python

import sys
import csv
import json

movieFile = "./movielens/movies.csv"
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
        
        if user not in userDict:
            userDict[user] = []
        userDict[user].append(movieGenre)  
    except ValueError:
        continue
        
for user in userDict:
    print("%s\t%s" % (user, json.dumps(userDict[user])))