#pragma once

#include "platform/digital_input/pulse_counter.hh"
#include "platform/digital_output/pulse_generator.hh"
#include "platform/pwm_output/pwm_output.hh"

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
 *
 * Peripherals:
 * TIM3 left aligned PWM @ 5kHz
 * PA6: CH1
 * PC7: CH2
 * PC8: CH3
 * PB1: CH4
 */
class Robot {
 public:
  Robot(const Robot&) = delete;
  void operator=(const Robot&) = delete;
  Robot(Robot&&) = delete;
  void operator=(Robot&&) = delete;

  const PulseGenerator<> led_green;
  const PulseGenerator<> led_yellow;
  const PulseGenerator<> led_red;

  const PulseCounter button;

  const PWMOutput pwm_1;
  const PWMOutput pwm_2;
  const PWMOutput pwm_3;
  const PWMOutput pwm_4;

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
