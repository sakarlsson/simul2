#!/bin/bash -x

mkdir -p test
cd test
../simul2 -c ../testdata/car.bonneville -t ../testdata/circuit.bonneville > output

cat > plot-cmd <<EOF
set multiplot layout 3,1
plot "output" using 1:5 with lines title "speed"
plot "output" using 1:8 with lines title "drag"
plot "output" using 1:7 with lines title "gear"
pause -1
EOF

gnuplot plot-cmd
