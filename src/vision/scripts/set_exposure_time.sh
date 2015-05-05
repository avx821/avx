#!/bin//bash
echo "chose an absoulte exposure time between 1 and 10^4. Default is 10^3"
read time
v4l2-ctl --set-ctrl exposure_time_absolute=$time
