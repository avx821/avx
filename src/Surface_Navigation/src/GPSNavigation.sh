
#!/bin/bash

#javac -cp lcm.jar:. avionics/*.java
#jar cvf avionics.jar avionics/*.class
javac -cp lcm.jar:avionics.jar:. *.java &&
echo "starting imu publishing"
./../../imu_pub &
#echo "running GPS Nav code"
./../../gps_pub &
#echo "running motor listener"
sudo ./../../motor/motor_controler & 
java -cp lcm.jar:avionics.jar:. GPS_Navigation 42.359 -72.09 60.0 42.359 -71.5000
