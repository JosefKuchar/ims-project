#!/bin/bash

for i in {20..40}
do
  ./model --log-file diameter.csv --epochs 36000 --roundabout-diameter "$i"
done
