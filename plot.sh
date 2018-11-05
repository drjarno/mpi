#!/bin/bash

spike_temperature=100
dt=0.05
dx=0.1
num_iterations=50000
save_every=100

count=$((num_iterations / save_every - 1))
#count=1

mkdir -p frames
for i in `seq 0 ${count}`
do
  gnuplot -e "set terminal pngcairo enhanced size 640,480; set output sprintf('frames/frame_%04d.png', $i); set title sprintf('Time: %.2f s', $dt * $i); set yrange[0:1.05*${spike_temperature}]; set xlabel 'Position (a.u.)'; set ylabel 'Temperature (a.u.)'; plot 'heat_${i}.dat' using (\$0*${dx}):1 with lines notitle"
done

ffmpeg -i frames/frame_%04d.png -r 12 animation.webm -y && rm -r frames/
