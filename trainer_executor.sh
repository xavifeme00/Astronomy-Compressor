#!/bin/bash

mkdir -p experiments

./bin/trainer ./INT.txt 4 0.001 0.001 60 > experiments/INT_cm4.txt
./bin/trainer ./GTC.txt 4 0.001 0.001 60 > experiments/GTC_cm4.txt
./bin/trainer ./JKT.txt 4 0.001 0.001 60 > experiments/JKT_cm4.txt
./bin/trainer ./WHT.txt 4 0.001 0.001 60 > experiments/WHT_cm4.txt
./bin/trainer ./LCO.txt 4 0.001 0.001 60 > experiments/LCO_cm4.txt
./bin/trainer ./VLT.txt 4 0.001 0.001 60 > experiments/VLT_cm4.txt
./bin/trainer ./TJO.txt 4 0.001 0.001 60 > experiments/TJO_cm4.txt
