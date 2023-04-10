#!/bin/bash

# Set variables
data_file="edge.dat"
output_file="map.png"
x_label="Latitude"
y_label="Longitude"
x_range="0:20"
y_range="0:20"

# Start Gnuplot
gnuplot << EOF

# Set labels and ranges
set terminal png
set output "$output_file"
set xlabel "$x_label"
set ylabel "$y_label"
set xrange [$x_range]
set yrange [$y_range]

# Plot data
plot "$data_file" title 'Route' with linespoints

# End Gnuplot
EOF