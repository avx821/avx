#!/bin//bash
echo "chose an exposure bias between 0 and 24 ( default is 12)"
read bias
v4l2-ctl --set-ctrl auto_exposure_bias=$bias;
