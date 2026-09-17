from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, LogInfo
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():

    linear_speed_arg = DeclareLaunchArgument(
        'linear_speed',
        default_value='0.2',
        description='Linear speed of the robot'
    )

    angular_speed_arg = DeclareLaunchArgument(
        'angular_speed',
        default_value='0.8',
        description='Angular speed of the robot'
    )

    publish_rate_arg = DeclareLaunchArgument(
        'publish_rate',
        default_value='10.0',
        description='Rate at which commands are published'
    )

    linear_speed = LaunchConfiguration('linear_speed')
    angular_speed = LaunchConfiguration('angular_speed')
    publish_rate = LaunchConfiguration('publish_rate')

    log_linear = LogInfo(msg=['linear_speed: ', linear_speed])
    log_angular = LogInfo(msg=['angular_speed: ', angular_speed])
    log_rate = LogInfo(msg=['publish_rate: ', publish_rate])

    move_robot_node = Node(
        package='launch_args_example_pkg',
        executable='arguments_examples_demo',
        output='screen',
        emulate_tty=True,
        parameters=[{
            'linear_speed': linear_speed,
            'angular_speed': angular_speed,
            'publish_rate': publish_rate,
        }],
    )

    return LaunchDescription([
        linear_speed_arg,
        angular_speed_arg,
        publish_rate_arg,
        log_linear,
        log_angular,
        log_rate,
        move_robot_node,
    ])
