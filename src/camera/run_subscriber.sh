#!/bin/sh

export CLASSPATH="`pkg-config --variable=classpath test_codes`:."

javac -cp camera_log.jar:lcm.jar:. CameraSubscriber.java 
java -cp camera_log:lcm.jar:. CameraSubscriber


