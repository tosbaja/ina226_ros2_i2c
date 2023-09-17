from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

import os


def generate_launch_description():
    ld = LaunchDescription()
    share_dir = get_package_share_directory('ina226_ros2_i2c')
    parameter_file = LaunchConfiguration('params_file')

    params_declare = DeclareLaunchArgument('params_file',
                                           default_value=os.path.join(
                                               share_dir, 'params', 'ina226.yaml'),
                                           description='Path to the ROS2 parameters file to use.')

    mpu6050driver_node = Node(
        package='ina226_ros2_i2c',
        executable='ina226_ros2_i2c',
        name='ina226_sensor_node',
        output="screen",
        emulate_tty=True,
        parameters=[parameter_file]
    )

    ld.add_action(params_declare)
    ld.add_action(mpu6050driver_node)
    return ld
