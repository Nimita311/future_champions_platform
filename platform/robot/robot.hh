#pragma once

namespace fcp::platform {

/**
 * @brief Singleton abstraction of the robot.
 *
 * Get an instance to interact with available robot hardware:
 * @code {.cpp}
 *   auto& robot = Robot::instance();
 * @endcode
 *
 * Board connections:
 * PB0: LD1 (LED green, active high)
 * PE1: LD2 (LED yellow, active high)
 * PB14: LD3 (LED red, active high)
 * PC13: B1 (switch blue, active high)
 */
class Robot {
 public:
  Robot(const Robot&) = delete;
  void operator=(const Robot&) = delete;

  /**
   * @brief Gets the instance of this singleton class.
   *
   * @return Robot instance.
   */
  static Robot& instance();

 private:
  Robot();
};

}  // namespace fcp::platform
