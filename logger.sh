#!/bin/bash
sudo modprobe bcm2835-v4l2
../Downloads/lcm/lcm-logger/lcm-logger& 
ant
