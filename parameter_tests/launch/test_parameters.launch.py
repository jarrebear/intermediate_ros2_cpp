from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    config_file = os.path.join(
        get_package_share_directory('parameter_tests'),
        'config',
        'params.yaml'
    )

    return LaunchDescription([
        Node(
            package='parameter_tests',
            executable='param_tests_node',
            name='param_tests_node',
            output='screen',
            emulate_tty=True,
            parameters=[config_file]
        )
    ])


# from launch import LaunchDescription
# from launch_ros.actions import Node


# def generate_launch_description():
#     return LaunchDescription([
#         Node(
#             package='parameter_tests',
#             executable='param_vel_node',
#             name='param_vel_node',
#             output='screen',
#             emulate_tty=True,
#             parameters=[
#                 {'angular_speed': 0.5},
#                 {'linear_speed': 0.5},
#                 {'publish_rate': 1.0}
#             ]
#         )
#     ])
