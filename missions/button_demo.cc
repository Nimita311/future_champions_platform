#include <FreeRTOS.h>
#include <stm32h743xx.h>
#include <task.h>

#include "platform/digital_input/digital_input.hh"
#include "platform/digital_input/pulse_counter.hh"
#include "platform/digital_output/digital_output.hh"

extern "C" __attribute__((weak)) void main_task(const void*) {
  using fcp::platform::DigitalOutput;
  const auto led_green = DigitalOutput{GPIOB, GPIO_PIN_0};
  const auto led_yellow = DigitalOutput{GPIOE, GPIO_PIN_1};
  const auto led_red = DigitalOutput{GPIOB, GPIO_PIN_14};
  const auto button = fcp::platform::DigitalInput{GPIOC, GPIO_PIN_13};
  const auto pc = fcp::platform::PulseCounter{GPIOC, GPIO_PIN_13};

  auto led_yellow_state_latch = uint32_t{100};
  auto led_red_state_latch = uint32_t{100};

  while (true) {
    if (button.is_active()) {
      led_green.on();
    } else {
      led_green.off();
    }

    const auto state = pc.state();
    if (state.consecutive_pulse_count == 1) {
      led_yellow_state_latch = state.total_pulse_count;
    }
    if (state.consecutive_pulse_count == 0 &&
        state.total_pulse_count == led_yellow_state_latch) {
      led_yellow.toggle();
      led_yellow_state_latch = 0;
    }

    if (state.consecutive_pulse_count == 2) {
      led_red_state_latch = state.total_pulse_count;
    }
    if (state.consecutive_pulse_count == 0 &&
        state.total_pulse_count == led_red_state_latch) {
      led_red.toggle();
      led_red_state_latch = 0;
    }

    vTaskDelay(10U);
  }
}
