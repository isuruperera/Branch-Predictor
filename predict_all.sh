#!/bin/bash

gcc -o predictor main.c predictorlib.c predictors.c linkedlist.c -I.

for FILE in `ls traces/*.gz`
do
  TRACENAME=`basename $FILE .gz`
  FILENAME=traces/$TRACENAME.trace
  gunzip -c $FILE > $FILENAME
  ./predictor -i $FILENAME -p 1
  ./predictor -i $FILENAME -p 2
  ./predictor -i $FILENAME -p 3
  ./predictor -i $FILENAME -p 4
  rm $FILENAME
done