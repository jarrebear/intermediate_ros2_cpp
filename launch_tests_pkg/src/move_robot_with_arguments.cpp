#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include <chrono>
#include <string>

using namespace std::chrono_literals;

class MoveRobot : public rclcpp::Node {
public:
  MoveRobot() : Node("move_robot_node"), mode_("turning") {
    // Declare parameters with default values
    this->declare_parameter<double>("turning_speed", 1.0);
    this->declare_parameter<double>("forward_speed", 1.0);

    // Read parameter values
    turning_speed_ = this->get_parameter("turning_speed").as_double();
    forward_speed_ = this->get_parameter("forward_speed").as_double();

    RCLCPP_INFO(this->get_logger(), "turning_speed: %.2f", turning_speed_);
    RCLCPP_INFO(this->get_logger(), "forward_speed: %.2f", forward_speed_);

    // Use relative topic name so namespaces from launch files are respected
    publisher_ =
        this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    timer_ = this->create_wall_timer(
        1000ms, std::bind(&MoveRobot::timer_callback, this));
  }

private:
  void timer_callback() {
    if (mode_ == "turning") {
      go_forward();
      mode_ = "go_forward";
    } else {
      turn();
      mode_ = "turning";
    }
  }

  void turn() {
    twist_.linear.x = 0.0;
    twist_.angular.z = turning_speed_;
    RCLCPP_INFO(this->get_logger(), "TURNING AT SPEED ==> %.2f",
                turning_speed_);
    publisher_->publish(twist_);
  }

  void go_forward() {
    twist_.linear.x = forward_speed_;
    twist_.angular.z = 0.0;
    RCLCPP_INFO(this->get_logger(), "GOING FORWARD AT SPEED ==> %.2f",
                forward_speed_);
    publisher_->publish(twist_);
  }

  std::string mode_;
  double turning_speed_;
  double forward_speed_;
  geometry_msgs::msg::Twist twist_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MoveRobot>());
  rclcpp::shutdown();
  return 0;
}
