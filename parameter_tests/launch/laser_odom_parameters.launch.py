from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    config_file = os.path.join(
        get_package_share_directory('parameter_tests'),
        'config',
        'params_laser_odom.yaml'
    )

    return LaunchDescription([
        Node(
            package='parameter_tests',
            executable='laser_odom_node',
            name='robot_sensor_node',
            output='screen',
            emulate_tty=True,
            parameters=[config_file]
        )
    ])