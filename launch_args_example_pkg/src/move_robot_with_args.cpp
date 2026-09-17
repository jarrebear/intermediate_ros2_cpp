#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include <algorithm>
#include <chrono>

class MoveRobot : public rclcpp::Node {
public:
  MoveRobot() : Node("move_robot_node") {
    // Declare parameters (typed, with defaults)
    this->declare_parameter<double>("linear_speed", 0.2);
    this->declare_parameter<double>("angular_speed", 0.8);
    this->declare_parameter<double>("publish_rate", 10.0);

    // Read parameters once at startup
    linear_speed_ = this->get_parameter("linear_speed").as_double();
    angular_speed_ = this->get_parameter("angular_speed").as_double();
    publish_rate_ = this->get_parameter("publish_rate").as_double();

    publisher_ =
        this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    // Create timer based on publish_rate
    double safe_rate = std::max(0.1, publish_rate_);
    auto period = std::chrono::duration<double>(1.0 / safe_rate);

    timer_ = this->create_wall_timer(
        std::chrono::duration_cast<std::chrono::nanoseconds>(period),
        std::bind(&MoveRobot::timer_callback, this));

    RCLCPP_INFO(this->get_logger(),
                "MoveRobot started | linear=%.2f angular=%.2f rate=%.2f Hz",
                linear_speed_, angular_speed_, publish_rate_);
  }

private:
  void timer_callback() {
    geometry_msgs::msg::Twist twist;
    twist.linear.x = linear_speed_;
    twist.angular.z = angular_speed_;
    publisher_->publish(twist);
  }

  double linear_speed_;
  double angular_speed_;
  double publish_rate_;

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MoveRobot>());
  rclcpp::shutdown();
  return 0;
}
