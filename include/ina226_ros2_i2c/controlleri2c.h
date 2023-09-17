#ifndef ControllerI2C_H
#define ControllerI2C_H

#include <string>
#include <unordered_map>

class ControllerI2C {
 public:
  ControllerI2C(int bus_number = 1, uint8_t device_address = 0x40);
  ~ControllerI2C();
  uint8_t ReadByte(uint8_t reg);
  uint16_t ReadWord(uint8_t reg);

  void WirteByte(uint8_t reg, uint8_t val);
  void WriteWord(uint8_t reg, uint16_t val);
 private:
  int16_t SwapMSBandLSB(uint16_t val_16);
  int file_;
  uint8_t DEVICE_ADDRESS;
  char filename_[10] = "/dev/i2c-";
};



#endif  // ControllerI2C_H
