#include "ina226_ros2_i2c/ina226driver.h"
#include "ina226_ros2_i2c/ina226ros.h"

#include <chrono>
#include <memory>

using namespace std::chrono_literals;

INA226Test::INA226Test()
    : Node("ina226")
{
  declareSetParameters();
  ina226_ = std::make_unique<INA226_WE>(adress_parameter_,bus_parameter_);
  ina226_->init();
  ina226_->setResistorRange(resistor_parameter_, current_parameter_); // choose resistor 0.1 Ohm and gain range up to 1.3A 
  ina226_->setCorrectionFactor(correction_factor_parameter_);
  ina226_->waitUntilConversionCompleted(); //if you comment this line the first data might be zero
  battery_level_publisher_ = this->create_publisher<std_msgs::msg::Float32>("battery_level", 10);
  battery_voltage_publisher_ = this->create_publisher<std_msgs::msg::Float32>("battery_voltage_V", 10);
  power_consumption_publisher_ = this->create_publisher<std_msgs::msg::Float32>("power_consumption_mW", 10);
  current_publisher_ = this->create_publisher<std_msgs::msg::Float32>("current_mA", 10);
  std::chrono::duration<int64_t, std::milli> frequency =
      1000ms / 1;
  timer_ = this->create_wall_timer(frequency, std::bind(&INA226Test::handleInput, this));
}

void INA226Test::handleInput()
{
  std_msgs::msg::Float32 floatMsg;
  float shuntVoltage_mV = 0.0;
  float loadVoltage_V = 0.0;
  float busVoltage_V = 0.0;
  float current_mA = 0.0;
  float power_mW = 0.0;
 
  ina226_->readAndClearFlags();
  shuntVoltage_mV = ina226_->getShuntVoltage_mV();
  busVoltage_V = ina226_->getBusVoltage_V();
  current_mA = ina226_->getCurrent_mA();
  power_mW = ina226_->getBusPower();
  loadVoltage_V  = busVoltage_V + (shuntVoltage_mV / 1000);

  floatMsg.data = busVoltage_V;
  battery_voltage_publisher_->publish(floatMsg);


  floatMsg.data = ((busVoltage_V - 10.5) / 2.1) * 100;
  battery_level_publisher_->publish(floatMsg);


  floatMsg.data = power_mW;
  power_consumption_publisher_->publish(floatMsg);


  floatMsg.data = current_mA;
  current_publisher_->publish(floatMsg);
 
  std::cout << "Shunt Voltage [mV]: " <<  shuntVoltage_mV << std::endl;
  std::cout << "Bus Voltage [V]: " <<  busVoltage_V << std::endl;
  std::cout << "Load Voltage [V]: " << loadVoltage_V << std::endl;
  std::cout << "Current[mA]: " << current_mA << std::endl;
  std::cout << "Bus Power [mW]: " <<  power_mW << std::endl;
  if (! ina226_->overflow)
  {
    std::cout << "Values OK - no overflow" << std::endl;
  }
  else
  {
    std::cout << "Overflow! Choose higher current range" << std::endl;
  }
}

void INA226Test::declareSetParameters()
{
  this->declare_parameter("address", 0x40);
  this->declare_parameter("connected_bus", 1);
  this->declare_parameter("resistor_ohms", 0.005);
  this->declare_parameter("current", 10.0);
  this->declare_parameter("correction_factor", 0.647);

  this->get_parameter("address", adress_parameter_);
  this->get_parameter("connected_bus", bus_parameter_);
  this->get_parameter("resistor_ohms", resistor_parameter_);
  this->get_parameter("current", current_parameter_);
  this->get_parameter("correction_factor", correction_factor_parameter_);
}

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<INA226Test>());
  rclcpp::shutdown();
  return 0;
}