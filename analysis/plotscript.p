set key autotitle columnhead
set title "Dynamic Density (Sub-sampling Method - skipping 5 frames)"
set xrange [0:400]
set yrange [0:1]
set xlabel "Time (sec)"
set ylabel "Density"
plot "5.txt" title "Dynamic Density" with line
