#!/bin/bash
apt install -y ros-kinetic-navigation ros-kinetic-map-server ros-kinetic-move-base ros-kinetic-amcl
apt install -y libignition-math2-dev protobuf-compiler
rosdep install gmapping
rosdep -i install turtlebot_gazebo
rosdep -i install turtlebot_teleop