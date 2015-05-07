
#!/bin/bash

#javac -cp lcm.jar:. avionics/*.java
#jar cvf avionics.jar avionics/*.class
javac -cp lcm.jar:avionics.jar:. *.java &&
echo "starting imu publishing"
./../../imu_pub &
#echo "running GPS Nav code"
#./../../gps_pub &
#./../../../../Downloads/lcm/lcm-logger
java -cp lcm.jar:avionics.jar:. GPS_Navigation 0.0001 0.00 0.0
