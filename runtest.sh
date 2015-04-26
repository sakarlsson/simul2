#!/bin/bash -x

mkdir -p test
cd test
../simul2 -c ../testdata/car.bonneville -t ../testdata/circuit.bonneville > output

cat > plot-cmd <<EOF
set terminal png size 1650,940
set output 'plot.png'
set multiplot layout 3,2
plot "output" using 1:5 with lines title "km/h"
plot "output" using 1:9 with lines title "power"
plot "output" using 1:8 with lines title "drag"
plot "output" using 1:7 with lines title "gear"
plot "output" using 1:6 with lines title "rpm"
plot "output" using 1:10 with lines title "force"
EOF

gnuplot plot-cmd

display plot.png
