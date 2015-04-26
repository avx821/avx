#!/bin/bash

javac -cp lcm.jar:. avionics/*.java
jar cvf avionics.jar avionics/*.class
javac -cp lcm.jar:avionics.jar:. *.java
echo "starting imu publishing"
./../../../imu_pub &
echo "running GPS Nav code"
java -cp lcm.jar:avionics.jar:. GPS_Navigation 60.000 12.0001 20.0
