#ifndef MPU6050DRIVER_H
#define MPU6050DRIVER_H

#include "ina226_ros2_i2c/ina226driver.h"

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "std_msgs/msg/float32.hpp"

class INA226Test : public rclcpp::Node {
 public:
  INA226Test();
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr battery_level_publisher_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr battery_voltage_publisher_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr power_consumption_publisher_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr current_publisher_;
 private:
  std::unique_ptr<INA226_WE> ina226_;
  size_t count_;
  rclcpp::TimerBase::SharedPtr timer_;
  void handleInput();
  void declareParameters();
};

#endif  // MPU6050DRIVER_H