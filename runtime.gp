reset
set ylabel 'time(sec)'
set style fill solid
set title 'perfomance comparison'
set term png enhanced font 'Verdana,10'
set output 'runtime.png'

plot 'output.txt' using 1:1 with histogram title 'iteration', \
'' using 1:2 with histogram title 'recursive'  , \
'' using 1:3 with histogram title 'Harley's Algorithm'  , \
'' using 1:4 with histogram title 'binary search'  , \
'' using 1:5 with histogram title 'byte shift'
