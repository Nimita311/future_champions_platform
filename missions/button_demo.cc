#include <FreeRTOS.h>
#include <stm32h743xx.h>
#include <task.h>

#include "platform/digital_input/digital_input.hh"
#include "platform/digital_output/digital_output.hh"

extern "C" void main_task(const void*) {
  const auto led_green = fcp::platform::DigitalOutput{GPIOB, GPIO_PIN_0};
  const auto button = fcp::platform::DigitalInput{GPIOC, GPIO_PIN_13};
  while (true) {
    if (button.is_active()) {
      led_green.on();
    } else {
      led_green.off();
    }
    vTaskDelay(10U);
  }
}
