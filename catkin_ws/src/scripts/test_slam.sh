#!/bin/sh
xterm  -e  " source /opt/ros/kinetic/setup.bash; roscore" & 
sleep 5

xterm  -e  "roslaunch turtlebot_gazebo turtlebot_world.launch" &
sleep 10

xterm  -e  "roslaunch turtlebot_teleop keyboard_teleop.launch" &
xterm  -e  "roslaunch turtlebot_rviz_launchers view_navigation.launch" &
xterm  -e  "roslaunch gmapping gmapping_demo.launch" &
