cmake_minimum_required(VERSION 3.5)
project(ina226_ros2_i2c)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)

add_executable(ina226_ros2_i2c src/controlleri2c.cpp src/ina226ros.cpp  src/ina226driver.cpp)
ament_target_dependencies(ina226_ros2_i2c rclcpp sensor_msgs)
target_link_libraries(ina226_ros2_i2c i2c)
target_include_directories(
  ina226_ros2_i2c PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                       $<INSTALL_INTERFACE:include>)

install(TARGETS ina226_ros2_i2c DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch params DESTINATION share/${PROJECT_NAME}/)
ament_package()
