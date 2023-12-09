#!/bin/bash

./model --log-file approach-count.csv --epochs 36000 --approach-count 3 --roundabout-diameter 15
./model --log-file approach-count.csv --epochs 36000 --approach-count 4 --roundabout-diameter 20
./model --log-file approach-count.csv --epochs 36000 --approach-count 5 --roundabout-diameter 25
./model --log-file approach-count.csv --epochs 36000 --approach-count 6 --roundabout-diameter 30

