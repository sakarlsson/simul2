#!/bin/bash -x

mkdir -p test
cd test
../simul2 -c ../testdata/car.bonneville -t ../testdata/circuit.bonneville > output

cat > plot-cmd <<EOF
set multiplot layout 3,2
plot "output" using 1:5 with lines title "speed"
plot "output" using 1:9 with lines title "power"
plot "output" using 1:8 with lines title "drag"
plot "output" using 1:7 with lines title "gear"
plot "output" using 1:6 with lines title "rps"
pause -1
EOF

gnuplot plot-cmd
