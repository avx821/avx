
#!/bin/bash

#javac -cp lcm.jar:. avionics/*.java
#jar cvf avionics.jar avionics/*.class
javac -cp lcm.jar:avionics.jar:. motorSend.java &&
#echo "running motor listener"
#sudo ./../../motor/motor_controler &
java -cp lcm.jar:avionics.jar:. motorSend
