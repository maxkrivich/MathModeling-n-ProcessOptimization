#!/bin/bash

gnuplot <<- EOF
    set xlabel "k"
    set ylabel "P(k)"
    set title "Birthday paradox"
    set term png
    set grid
    set output "plot.png"
    plot "$1"
EOF