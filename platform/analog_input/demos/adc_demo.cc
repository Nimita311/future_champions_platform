#include "platform/robot/robot.hh"

/**
 * Green LED pulses if analog value increases between two measurements and red
 * LED pulses if it drops. If the analog pin is floating, two LEDs will likely
 * pulse randomly.
 */
extern "C" __attribute__((weak)) void main_task(const void*) {
  const auto& robot = fcp::platform::Robot::instance();
  const auto& analog_input = robot.analog_input_3_0;
  const auto& up_led = robot.led_green;
  const auto& down_led = robot.led_red;

  auto value = analog_input.value();
  while (true) {
    const auto new_value = analog_input.value();
    if (new_value > value) {
      up_led.pulse();
    } else {
      down_led.pulse();
    }
    value = new_value;
    vTaskDelay(100U);
  }
}
