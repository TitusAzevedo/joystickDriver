#!/bin/bash
pids=()

./joystick /dev/input/js0 & pids+=( "$!" )
./joystick /dev/input/js1 & pids+=( "$!" )
./joystick /dev/input/js2 & pids+=( "$!" )
./joystick /dev/input/js3 & pids+=( "$!" )

for x in 0 1 2 3
do
    echo ${pids[$x]}
done
