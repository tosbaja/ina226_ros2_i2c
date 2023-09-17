#include "ina226_ros2_i2c/controlleri2c.h"

extern "C" {
#include <fcntl.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
}

#include <iostream>

ControllerI2C::ControllerI2C(int bus_number, uint8_t device_address)
{
  // TODO: make char append cleaner
  filename_[9] = *std::to_string(bus_number).c_str();
  DEVICE_ADDRESS = device_address;
  std::cout << filename_ << std::endl;
  file_ = open(filename_, O_RDWR);
  if (file_ < 0) {
    std::cerr << "Failed to open file descriptor! Check your bus number! Errno: "
              << strerror(errno);
    exit(1);
  }
  if (ioctl(file_, I2C_SLAVE, DEVICE_ADDRESS) < 0) {
    std::cerr << "Failed to find device address! Check device address!";
    exit(1);
  }
}

ControllerI2C::~ControllerI2C() { close(file_); }

int16_t ControllerI2C::SwapMSBandLSB(uint16_t val_16)
{
  return (val_16 << 8) + (val_16 >> 8);
}

uint8_t ControllerI2C::ReadByte(uint8_t reg) {return i2c_smbus_read_byte_data(file_, reg);}

uint16_t ControllerI2C::ReadWord(uint8_t reg) {
  return SwapMSBandLSB(i2c_smbus_read_word_data(file_, reg));
 }

void ControllerI2C::WirteByte(uint8_t reg, uint8_t val) {i2c_smbus_write_byte_data(file_, reg, val);}

void ControllerI2C::WriteWord(uint8_t reg, uint16_t val) {
  i2c_smbus_write_word_data(file_, reg, SwapMSBandLSB(val));
}

