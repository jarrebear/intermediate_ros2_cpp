#include "geometry_msgs/msg/twist.hpp"
#include "rcl_interfaces/msg/parameter_descriptor.hpp"
#include "rcl_interfaces/msg/set_parameters_result.hpp"
#include "rclcpp/rclcpp.hpp"

#include <algorithm>
#include <chrono>
#include <functional>
#include <string>
#include <vector>

using namespace std::chrono_literals;

class MoveRobot : public rclcpp::Node {
public:
  MoveRobot() : Node("param_vel_node") {
    // Parameter descriptors
    rcl_interfaces::msg::ParameterDescriptor linear_desc;
    linear_desc.description = "Linear speed of the robot in m/s.";

    rcl_interfaces::msg::ParameterDescriptor angular_desc;
    angular_desc.description = "Angular speed of the robot in rad/s.";

    rcl_interfaces::msg::ParameterDescriptor rate_desc;
    rate_desc.description =
        "Rate in Hz at which velocity commands are published.";

    // Declare parameters
    this->declare_parameter<double>("linear_speed", 0.0, linear_desc);
    this->declare_parameter<double>("angular_speed", 0.0, angular_desc);
    this->declare_parameter<double>("publish_rate", 1.0, rate_desc);

    // Read initial values
    linear_speed_ = this->get_parameter("linear_speed").as_double();
    angular_speed_ = this->get_parameter("angular_speed").as_double();
    publish_rate_ = this->get_parameter("publish_rate").as_double();

    publisher_ =
        this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    timer_ = this->create_wall_timer(
        1000ms, std::bind(&MoveRobot::timer_callback, this));

    callback_handle_ = this->add_on_set_parameters_callback(std::bind(
        &MoveRobot::parameters_callback, this, std::placeholders::_1));
  }

private:
  rcl_interfaces::msg::SetParametersResult
  parameters_callback(const std::vector<rclcpp::Parameter> &parameters) {
    rcl_interfaces::msg::SetParametersResult result;
    result.successful = true;
    result.reason = "Parameters accepted.";

    for (const auto &parameter : parameters) {
      if (parameter.get_name() == "linear_speed") {
        if (parameter.as_double() > 1.0) {
          result.successful = false;
          result.reason = "Parameter 'linear_speed' cannot be higher than 1.0";
          RCLCPP_WARN(this->get_logger(),
                      "Parameter 'linear_speed' not changed: value too high.");
          return result;
        }
      }

      if (parameter.get_name() == "angular_speed") {
        if (parameter.as_double() > 2.0) {
          result.successful = false;
          result.reason = "Parameter 'angular_speed' cannot be higher than 1.0";
          RCLCPP_WARN(this->get_logger(),
                      "Parameter 'angular_speed' not changed: value too high.");
          return result;
        }
      }
    }

    RCLCPP_INFO(this->get_logger(), "Parameters changed successfully.");
    return result;
  }

  void timer_callback() {
    // Read parameters at runtime
    linear_speed_ = this->get_parameter("linear_speed").as_double();
    angular_speed_ = this->get_parameter("angular_speed").as_double();
    publish_rate_ = this->get_parameter("publish_rate").as_double();

    RCLCPP_INFO(this->get_logger(),
                "linear_speed=%.2f | angular_speed=%.2f | publish_rate=%.2f",
                linear_speed_, angular_speed_, publish_rate_);

    twist_.linear.x = linear_speed_;
    twist_.angular.z = angular_speed_;

    publisher_->publish(twist_);
  }

  double linear_speed_;
  double angular_speed_;
  double publish_rate_;

  geometry_msgs::msg::Twist twist_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  OnSetParametersCallbackHandle::SharedPtr callback_handle_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MoveRobot>());
  rclcpp::shutdown();
  return 0;
}
