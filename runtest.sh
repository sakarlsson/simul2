#!/bin/bash

rm -rf test
mkdir -p test
cd test
../simul2 -c ../testdata/car.bonneville -t ../testdata/cicuit.bonneville 
