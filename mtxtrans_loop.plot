set grid
set xlabel "Matrix Size (m * n)"
set ylabel "Run Time (milliseconds)"

set key top left

set terminal postscript landscape enhanced color lw 1 "Helvetica" 16
set output "mtxtrans_loop_m1.ps"

plot "mtxtrans_loop.dat" using 1:2 title "Cache Ignorant Algorithm" with lines lt 1 lw 3 linecolor rgb "violet",\
     "mtxtrans_loop.dat" using 1:3 title "Cache Oblivious Algorithm" with lines lt 1 lw 3 linecolor rgb "blue"

set terminal postscript eps enhanced color lw 1 "Helvetica" 16
set output "mtxtrans_loop_m1.eps"
replot

set logscale x
set xlabel "Log - Matrix Size (m * n)"
set logscale y
set ylabel "Log - Run Time (milliseconds)"

set terminal postscript landscape enhanced color lw 1 "Helvetica" 16
set output "mtxtrans_loop_logxy_m1.ps"
replot

set terminal postscript eps enhanced color lw 1 "Helvetica" 16
set output "mtxtrans_loop_logxy_m1.eps"
replot
