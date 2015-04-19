#!/bin/bash -x

mkdir -p test
cd test
../simul2 -c ../testdata/car.bonneville -t ../testdata/circuit.bonneville > output

gnuplot <<EOF
plot "output" using 1:5 with lines title "speed"
pause -1
EOF
