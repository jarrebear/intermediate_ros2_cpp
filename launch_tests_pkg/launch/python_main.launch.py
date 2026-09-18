import launch
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, GroupAction, IncludeLaunchDescription, LogInfo
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import PushRosNamespace
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():

    package_name = 'launch_tests_pkg'

    # Declare launch arguments (can be overridden from the command line)
    turning_speed_arg = DeclareLaunchArgument('turning_speed', default_value='0.2')
    forward_speed_arg = DeclareLaunchArgument('forward_speed', default_value='0.1')
    rviz_config_file_name_arg = DeclareLaunchArgument('rviz_config_file_name', default_value='launch_part.rviz')
    custom_namespace_arg = DeclareLaunchArgument('custom_namespace', default_value='mazinger')
    
    turning_speed = LaunchConfiguration('turning_speed')
    forward_speed = LaunchConfiguration('forward_speed')
    rviz_config_file_name = LaunchConfiguration('rviz_config_file_name')
    custom_namespace = LaunchConfiguration('custom_namespace')

    # Print launch arguments (for debugging / teaching)
    log_args = [
        LogInfo(msg=['turning_speed: ', turning_speed]),
        LogInfo(msg=['forward_speed: ', forward_speed]),
        LogInfo(msg=['rviz_config_file_name: ', rviz_config_file_name]),
        LogInfo(msg=['custom_namespace: ', custom_namespace]),
    ]

    # Include another launch file: RViz
    start_rviz_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([
                FindPackageShare(package_name),
                'launch',
                'start_rviz_with_arguments.launch.py',
            ])
        ),
        launch_arguments={
            'rviz_config_file_name': rviz_config_file_name,
        }.items(),
    )

    # Include another launch file: move robot (pushed into a namespace)
    move_robot_with_namespace_launch = GroupAction(
        actions=[
            PushRosNamespace(custom_namespace),
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(
                    PathJoinSubstitution([
                        FindPackageShare(package_name),
                        'launch',
                        'move_with_arguments.launch.py',
                    ])
                ),
                launch_arguments={
                    'turning_speed': turning_speed,
                    'forward_speed': forward_speed,
                }.items(),
            ),
        ],
    )

    return LaunchDescription([
        turning_speed_arg,
        forward_speed_arg,
        rviz_config_file_name_arg,
        custom_namespace_arg,
        *log_args,
        start_rviz_launch,
        move_robot_with_namespace_launch,
    ])
