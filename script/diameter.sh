#!/bin/bash

for i in {25..41}
do
  ./model --log-file diameter.csv --epochs 36000 --roundabout-diameter "$i"
done
