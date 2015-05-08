
#!/bin/bash

#javac -cp lcm.jar:. avionics/*.java
#jar cvf avionics.jar avionics/*.class
javac -cp lcm.jar:avionics.jar:. *.java &&
#./../../../../Downloads/lcm/lcm-logger
java -cp lcm.jar:avionics.jar:. GPS_Navigation 42.360 -71.0943 0.0
