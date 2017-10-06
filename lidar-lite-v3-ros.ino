/*------------------------------------------------------------------------------

  This Code is based on the  LIDARLite Arduino Library  GetDistanceI2c

  This code shows how to initialize, configure, and read distance from a
  LIDAR-Lite connected over the I2C interface and publish the data in a ROS topic.

  Connections:
  LIDAR-Lite 5 Vdc (red) to Arduino 5v
  LIDAR-Lite I2C SCL (green) to Arduino SCL
  LIDAR-Lite I2C SDA (blue) to Arduino SDA
  LIDAR-Lite Ground (black) to Arduino GND
  
  (Capacitor recommended for stability during fast operation)
  680uF capacitor (+) to Arduino 5v
  680uF capacitor (-) to Arduino GND
  
  See the Operation Manual for wiring diagrams and more information:
  http://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf

------------------------------------------------------------------------------*/

#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>
#include <Wire.h>
#include <LIDARLite.h>

LIDARLite myLidarLite;

//ROS node handle
ros::NodeHandle  nh;

// ROS Serial Range message definition
sensor_msgs::Range range_msg;

// definition of the ROS publisher for the range data
ros::Publisher pub_range( "range_data", &range_msg);
unsigned long range_timer;

// Frame ID used in the ROS topics
char frameid[] = "/laser_lite_v3";

void setup()
{
  Serial.begin(115200); // Initialize serial connection to display distance readings

  /*
    begin(int configuration, bool fasti2c, char lidarliteAddress)
  
    Starts the sensor and I2C.
  
    Parameters
    ----------------------------------------------------------------------------
    configuration: Default 0. Selects one of several preset configurations.
    fasti2c: Default 100 kHz. I2C base frequency.
      If true I2C frequency is set to 400kHz.
    lidarliteAddress: Default 0x62. Fill in new address here if changed. See
      operating manual for instructions.
  */
  myLidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz

  /*
    configure(int configuration, char lidarliteAddress)
  
    Selects one of several preset configurations.
  
    Parameters
    ----------------------------------------------------------------------------
    configuration:  Default 0.
      0: Default mode, balanced performance.
      1: Short range, high speed. Uses 0x1d maximum acquisition count.
      2: Default range, higher speed short range. Turns on quick termination
          detection for faster measurements at short range (with decreased
          accuracy)
      3: Maximum range. Uses 0xff maximum acquisition count.
      4: High sensitivity detection. Overrides default valid measurement detection
          algorithm, and uses a threshold value for high sensitivity and noise.
      5: Low sensitivity detection. Overrides default valid measurement detection
          algorithm, and uses a threshold value for low sensitivity and noise.
    lidarliteAddress: Default 0x62. Fill in new address here if changed. See
      operating manual for instructions.
  */
  myLidarLite.configure(0); // Change this number to try out alternate configurations
  
  /* ROS related */
  nh.initNode();
  nh.advertise(pub_range);
  range_msg.radiation_type = sensor_msgs::Range::INFRARED;
  range_msg.header.frame_id =  frameid;
  range_msg.field_of_view = 0.001;
  range_msg.min_range = 0.01;
  range_msg.max_range = 40.0;
  
}

void loop()
{
  /*
    distance(bool biasCorrection, char lidarliteAddress)
  
    Take a distance measurement and read the result.
  
    Parameters
    ----------------------------------------------------------------------------
    biasCorrection: Default true. Take aquisition with receiver bias
      correction. If set to false measurements will be faster. Receiver bias
      correction must be performed periodically. (e.g. 1 out of every 100
      readings).
    lidarliteAddress: Default 0x62. Fill in new address here if changed. See
      operating manual for instructions.
  */

  // Take a measurement with receiver bias correction and print to serial terminal
  //Serial.println(myLidarLite.distance());

  // Take 99 measurements without receiver bias correction and print to serial terminal
  //for(int i = 0; i < 99; i++)
  //{
  //  Serial.println(myLidarLite.distance(false));
  //}

  // read and convert measurements to meters to comply with ROS
  range_msg.range = myLidarLite.distance()/100.0;
  range_msg.header.stamp = nh.now();
  pub_range.publish(&range_msg);
  range_timer =  millis();
  nh.spinOnce();  
}

