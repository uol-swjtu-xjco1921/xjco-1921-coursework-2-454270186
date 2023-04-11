#!/bin/bash

# Set variables
data_file="edge.dat"
output_file="map.png"
x_label="Latitude"
y_label="Longitude"

# Set default values for x_range and y_range
default_x_range="-1.565:-1.54"
default_y_range="53.801:53,812"
if [ -f "range_x.txt" ]; then
    x_range=$(cat range_x.txt)
else
    x_range=$default_x_range
fi
if [ -f "range_y.txt" ]; then
    y_range=$(cat range_y.txt)
else
    y_range=$default_y_range
fi

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