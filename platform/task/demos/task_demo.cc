#include <FreeRTOS.h>
#include <task.h>

#include <utility>

#include "platform/robot/robot.hh"
#include "platform/task/periodic_task.hh"
#include "platform/time/time.hh"

/**
 * Green LED flashes. Press the button and hold, the green LED will stop
 * flashing. Flashing resumes when the button is released.
 *
 * One task handles button pressed and two periodic tasks turns on and off the
 * LED. They communicate via a simple shared state which has one writer and two
 * readers.
 */
extern "C" __attribute__((weak)) void main_task(const void*) {
  volatile bool should_toggle = true;
  const auto& robot = fcp::platform::Robot::instance();
  const auto button_task = fcp::platform::Task{
      fcp::platform::TaskConfig{
          .name = "button_timer",
          .stack_depth = 128,
          .priority = 3,
      },
      [&button = robot.button, &should_toggle]() {
        const auto state = button.state();
        if (state.current_width_ms == 0) {
          fcp::platform::delay_ms(100);
          return;
        }
        // stop toggle when button pressed
        should_toggle = false;
        // wait until release
        while (button.state().total_pulse_count == state.total_pulse_count) {
          fcp::platform::delay_ms(100);
        }
        // enable toggle
        should_toggle = true;
      },
  };
  const auto led_on_task = fcp::platform::PeriodicTask{
      fcp::platform::PeriodicTaskConfig{{
                                            .name = "led_on",
                                            .stack_depth = 128,
                                            .priority = 3,
                                        },
                                        .period_ms = 500,
                                        .phase_ms = 0},
      [&led = robot.led_green,
       &should_toggle = std::as_const(should_toggle)]() {
        if (should_toggle) {
          led.on();
        }
      }};
  const auto led_off_task = fcp::platform::PeriodicTask{
      fcp::platform::PeriodicTaskConfig{{
                                            .name = "led_off",
                                            .stack_depth = 128,
                                            .priority = 3,
                                        },
                                        .period_ms = 500,
                                        .phase_ms = 100},
      [&led = robot.led_green,
       &should_toggle = std::as_const(should_toggle)]() {
        if (should_toggle) {
          led.off();
        }
      }};

  while (true) {
    vTaskDelay(10U);
  }
}
