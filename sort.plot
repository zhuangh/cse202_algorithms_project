set grid
set xlabel "Input Size (array elements)"
set ylabel "Run Time (milliseconds)"

set key top left

set terminal postscript landscape enhanced color lw 1 "Helvetica" 16
set output "sort_m1.ps"

# plot "mergesort.dat" using 1:2 title "Cache Ignorant Algorithm" with lines lt 1 lw 3 linecolor rgb "violet",\
#      "funnelsort.dat" using 1:2 title "Cache Oblivious Algorithm" with lines lt 1 lw 3 linecolor rgb "blue"
plot "funnelsort.dat" using 1:2 title "Merge Sort Algorithm" with linespoints lt 1 lw 3 linecolor rgb "violet",\
     "fast-mergesort.dat" using 1:2 title "Funnel Sort Algorithm" with linespoints lt 1 lw 3 linecolor rgb "blue"

set terminal postscript eps enhanced color lw 1 "Helvetica" 16
set output "sort_m1.eps"
replot

# set logscale x
# set xlabel "Log - Matrix Size (m * n)"
# set logscale y
# set ylabel "Log - Run Time (milliseconds)"

# set terminal postscript landscape enhanced color lw 1 "Helvetica" 16
# set output "funnelsort_logxy_m1.ps"
# replot

# set terminal postscript eps enhanced color lw 1 "Helvetica" 16
# set output "funnelsort_logxy_m1.eps"
# replot
