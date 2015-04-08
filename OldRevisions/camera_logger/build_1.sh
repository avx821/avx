#!/bin/sh

lcm-gen -j ../../types/camera_logger_t.lcm

export CLASSPATH="`pkg-config --variable=classpath lcm-java`:."

javac camera_log/*.java
javac CameraSubscriber.java
javac SendMessage.java


