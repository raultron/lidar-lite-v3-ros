# Lidar Lite V3 ROS

## Requirements:

Aruino UNO or better and a PC with ROS.


## Instructions

1. Install the following libraries:

* [LIDAR-Lite_v3 Library](https://github.com/garmin/LIDARLite_v3_Arduino_Library).
* [The ROS serial library (ros_lib)](http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup).

2. Open the lidar-lite-v3-ros.ino file in Arduino IDE and program the Arduino.

3. Connect the Lidar Lite to the Arduino using the example configuration shown on page 3 of the [manual](http://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf).

4. Launch a roscore

`roscore`

5. Run the rosserial client application that forwards your Arduino messages to the rest of ROS. Make sure to use the correct serial port:

`rosrun rosserial_python serial_node.py /dev/ttyUSB0`
