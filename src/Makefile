# This Makefile was tested with GNU Make
CXX=g++

# Use pkg-config to lookup the proper compiler and linker flags for LCM
CFLAGS=`pkg-config --cflags lcm`
LDFLAGS=`pkg-config --libs lcm`

msg_types=../lcmtypes/*.lcm

all: imu_pub\
     gps_pub

imu_pub: imu_pub.o 
	$(CXX) -o $@ $^ $//home/pi/Navio/C++/Navio/MPU9250.cpp $(LDFLAGS)

gps_pub: gps_pub.o
	$(CXX) -o $@ $^ $//home/pi/Navio/C++/Navio/Ublox.cpp $(LDFLAGS)

