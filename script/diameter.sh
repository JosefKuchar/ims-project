#!/bin/bash

for i in {20..60}
do
  ./model --log-file diameter.csv --epochs 36000 --roundabout-diameter "$i"
done
