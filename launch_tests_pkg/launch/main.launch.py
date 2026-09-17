#!/usr/bin/env python3
import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

def generate_launch_description():
    pkg_share = get_package_share_directory('launch_tests_pkg')
    launch_dir = os.path.join(pkg_share, 'launch')

    move_robot = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(launch_dir, 'move.launch.py')),
    )

    start_rviz = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(launch_dir, 'start_rviz.launch.py')),
    )

    return LaunchDescription([move_robot, start_rviz])
