#include "ina226_ros2_i2c/ina226driver.h"
#include "ina226_ros2_i2c/ina226ros.h"

#include <chrono>
#include <memory>

using namespace std::chrono_literals;

INA226Test::INA226Test()
    : Node("ina226test"), ina226_{std::make_unique<INA226_WE>(0x40,3)}
{
  ina226_->init();
  ina226_->setResistorRange(0.005, 10.0); // choose resistor 0.1 Ohm and gain range up to 1.3A 
  ina226_->setCorrectionFactor(0.647);
  ina226_->waitUntilConversionCompleted(); //if you comment this line the first data might be zero
  this->declare_parameter<int>("address", 0x0);
  this->declare_parameter<int>("connected_bus", 0x0);
  std::chrono::duration<int64_t, std::milli> frequency =
      1000ms / 1;
  timer_ = this->create_wall_timer(frequency, std::bind(&INA226Test::handleInput, this));
}

void INA226Test::handleInput()
{
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

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<INA226Test>());
  rclcpp::shutdown();
  return 0;
}