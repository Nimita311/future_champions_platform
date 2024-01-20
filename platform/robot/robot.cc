#include "platform/robot/robot.hh"

namespace fcp::platform {

Robot& Robot::instance() {
  static auto robot = Robot{};
  return robot;
}

Robot::Robot() {
  // empty
}

}  // namespace fcp::platform
