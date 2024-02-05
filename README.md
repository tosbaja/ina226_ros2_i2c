
# INA226_ROS2_I2C

Ros2 driver for ina226 sensor. Publishes voltage and current readings from the sensor. Works with single board computers like raspberry and orange pi. The main  skeleton of code for interacting with ina226 is completely ported from [**INA226_WE**](https://github.com/wollewald/INA226_WE) library, the  only change in the code
is the communication layer i2c implemented for working with SBC like mentioned above and adding ros2 support.
### Dependencies

Works with ros2 humble and you will need to install **libi2c-dev**. You can also run the rosdep command below to install it.

```
rosdep install --from-paths src -y --ignore-src
```

### Params
#### address
I2c adress of the sensor normally it is 0x40 but it can be different. You can install i2c-tools and use the i2c detect command.
```
sudo apt install i2c-tools
i2cdetect -y 1
```
#### connected_bus
I2c bus of the computer. To find which bus the sensor is on you can run the command above and change the number. The number point to the bus number.
```
i2cdetect -y <bus number you want to check 0 1 2 ... n>
```
#### resistor_ohms
Ohm value of the current sensing resistor that is connected to the in- pin of the sensor.

#### current
Expected maximum current.

#### correction_factor
Multiplier to psudo calibrate the sensor, you can check the values with a multimeter and increase or decrease this value to match multimeters readings.

### Example launch
```
ros2 launch ina226_ros2_i2c ina226_sensor_launch.py
```
This launch file will read the parameters from ina226.yaml file. The defaults params are set to work with the tosbaja bot.