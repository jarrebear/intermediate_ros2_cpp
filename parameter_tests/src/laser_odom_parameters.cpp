#include "nav_msgs/msg/odometry.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

#include <functional>

class RobotSensors : public rclcpp::Node {
public:
  RobotSensors() : Node("robot_sensor_node") {
    // Parameter descriptions
    rcl_interfaces::msg::ParameterDescriptor laser_desc;
    laser_desc.description =
        "Display the value of the laser right in front of the robot";

    rcl_interfaces::msg::ParameterDescriptor odom_desc;
    odom_desc.description = "Display the value of the [X, Y] of the robot";

    // Declare parameters
    this->declare_parameter<bool>("get_laser_data", false, laser_desc);
    this->declare_parameter<bool>("get_odom_data", false, odom_desc);

    // Read parameters at startup
    laser_bool_ = this->get_parameter("get_laser_data").as_bool();
    odom_bool_ = this->get_parameter("get_odom_data").as_bool();

    // Create subscribers for scan and odometry

    subscriber_laser_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
        "/laser_scan", 10,
        std::bind(&RobotSensors::laserscan_callback, this,
                  std::placeholders::_1));

    subscriber_odom_ = this->create_subscription<nav_msgs::msg::Odometry>(
        "/odom", 10,
        std::bind(&RobotSensors::odometry_callback, this,
                  std::placeholders::_1));

    RCLCPP_INFO(this->get_logger(),
                "RobotSensors Node Odometry and LaserScan Results Ready...");
  }

private:
  void laserscan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg) {
    // Read laser parameter at start of callback, if true print data
    laser_bool_ = this->get_parameter("get_laser_data").as_bool();
    if (laser_bool_) {
      RCLCPP_INFO(this->get_logger(),
                  "LaserScan distance at front of robot: %.2f", msg->ranges[0]);
    }
  }

  void odometry_callback(const nav_msgs::msg::Odometry::SharedPtr msg) {
    // Read odom parameter at start of callback, if true print data
    odom_bool_ = this->get_parameter("get_odom_data").as_bool();
    if (odom_bool_) {
      RCLCPP_INFO(this->get_logger(), "Position [X, Y] of robot: [%.2f, %.2f]",
                  msg->pose.pose.position.x, msg->pose.pose.position.y);
    }
  }

private:
  bool laser_bool_;
  bool odom_bool_;

  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr
      subscriber_laser_;

  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscriber_odom_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<RobotSensors>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}